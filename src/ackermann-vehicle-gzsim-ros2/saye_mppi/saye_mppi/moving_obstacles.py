from __future__ import annotations

import math
from dataclasses import dataclass
from typing import List

import rclpy
from rclpy.node import Node
import yaml

from geometry_msgs.msg import Pose, Quaternion
from ros_gz_interfaces.msg import Entity
from ros_gz_interfaces.srv import SpawnEntity, SetEntityPose


@dataclass
class Obstacle:
    name: str
    start: List[float]
    end: List[float]
    speed: float
    progress: float = 0.0
    direction: int = 1

    @property
    def distance(self) -> float:
        return float(math.dist(self.start[:2], self.end[:2]))


BOX_TEMPLATE = """<?xml version="1.0" ?>
<sdf version="1.8">
  <model name="{name}">
    <static>false</static>
    <pose>0 0 0 0 0 0</pose>
    <link name="body">
      <pose>0 0 {z_offset} 0 0 0</pose>
      <inertial>
        <mass>5.0</mass>
        <inertia>
          <ixx>0.1</ixx>
          <iyy>0.1</iyy>
          <izz>0.1</izz>
          <ixy>0.0</ixy>
          <ixz>0.0</ixz>
          <iyz>0.0</iyz>
        </inertia>
      </inertial>
      <collision name="collision">
        <geometry>
          <box>
            <size>{size_x} {size_y} {size_z}</size>
          </box>
        </geometry>
      </collision>
      <visual name="visual">
        <geometry>
          <box>
            <size>{size_x} {size_y} {size_z}</size>
          </box>
        </geometry>
        <material>
          <ambient>{color}</ambient>
          <diffuse>{color}</diffuse>
        </material>
      </visual>
    </link>
  </model>
</sdf>
"""


class MovingObstacles(Node):
    """Spawn and animate simple box obstacles using Gazebo Sim services."""

    def __init__(self) -> None:
        super().__init__('moving_obstacles')

        self.declare_parameter('world_name', 'my_world')
        self.declare_parameter('update_rate', 10.0)
        self.declare_parameter('box_size', [0.6, 0.4, 0.6])
        self.declare_parameter('obstacle_specs', '')

        world = self.get_parameter('world_name').get_parameter_value().string_value
        size_param = self.get_parameter('box_size').get_parameter_value().double_array_value
        self._box_size = list(size_param) if size_param else [0.6, 0.4, 0.6]

        update_rate = float(self.get_parameter('update_rate').value)

        specs_param = self.get_parameter('obstacle_specs').value
        if isinstance(specs_param, str) and specs_param.strip():
            try:
                obstacle_specs = yaml.safe_load(specs_param)
            except yaml.YAMLError as exc:
                self.get_logger().error(f'Failed to parse obstacle_specs: {exc}')
                obstacle_specs = []
        else:
            obstacle_specs = [
                {
                    'name': 'crossing_obstacle_1',
                    'start': [12.0, -2.5, 0.0],
                    'end': [12.0, 2.5, 0.0],
                    'speed': 0.5,
                },
                {
                    'name': 'crossing_obstacle_2',
                    'start': [18.0, 2.8, 0.0],
                    'end': [18.0, -2.8, 0.0],
                    'speed': 0.4,
                },
                {
                    'name': 'inline_obstacle_1',
                    'start': [6.0, 0.6, 0.0],
                    'end': [10.0, 0.6, 0.0],
                    'speed': 0.35,
                },
            ]

        if not obstacle_specs:
            self.get_logger().warn('No obstacle specifications provided; the node will remain idle.')

        self._spawn_service = self.create_client(SpawnEntity, f'/world/{world}/spawn_entity')
        self._set_pose_service = self.create_client(SetEntityPose, f'/world/{world}/set_entity_pose')

        for client, name in (
            (self._spawn_service, 'spawn_entity'),
            (self._set_pose_service, 'set_entity_pose'),
        ):
            while not client.wait_for_service(timeout_sec=1.0):
                self.get_logger().info(
                    f'Waiting for /world/{world}/{name} service to become available...')

        self._obstacles: List[Obstacle] = []
        for spec in obstacle_specs:
            try:
                parsed = Obstacle(
                    name=spec['name'],
                    start=list(spec['start']),
                    end=list(spec['end']),
                    speed=float(spec['speed']),
                )
            except KeyError as exc:
                self.get_logger().error(f'Invalid obstacle specification {spec}: {exc}')
                continue
            if parsed.distance < 1e-6:
                self.get_logger().warn(
                    f'Obstacle {parsed.name} has zero travel distance; skipping.')
                continue
            self._spawn_obstacle(parsed)
            self._obstacles.append(parsed)

        self._last_update = self.get_clock().now()
        if self._obstacles:
            self._timer = self.create_timer(1.0 / max(update_rate, 1e-3), self._on_timer)
            self.get_logger().info(f'Activated {len(self._obstacles)} moving obstacles.')
        else:
            self._timer = None

    def _spawn_obstacle(self, obstacle: Obstacle) -> None:
        sdf = BOX_TEMPLATE.format(
            name=obstacle.name,
            size_x=self._box_size[0],
            size_y=self._box_size[1],
            size_z=self._box_size[2],
            z_offset=self._box_size[2] * 0.5,
            color='0.8 0.1 0.1 1.0',
        )
        request = SpawnEntity.Request()
        request.entity_factory.name = obstacle.name
        request.entity_factory.allow_renaming = False
        request.entity_factory.sdf = sdf
        request.entity_factory.pose.position.x = obstacle.start[0]
        request.entity_factory.pose.position.y = obstacle.start[1]
        request.entity_factory.pose.position.z = obstacle.start[2] + self._box_size[2] * 0.5
        request.entity_factory.pose.orientation = Quaternion(x=0.0, y=0.0, z=0.0, w=1.0)

        future = self._spawn_service.call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)
        if future.result() and future.result().success:
            self.get_logger().info(f'Spawned obstacle {obstacle.name}.')
        else:
            raise RuntimeError(f'Failed to spawn obstacle {obstacle.name}. '
                               f'Result: {future.result()}')

    def _on_timer(self) -> None:
        now = self.get_clock().now()
        dt = (now - self._last_update).nanoseconds / 1e9
        self._last_update = now

        for obstacle in self._obstacles:
            travel = obstacle.speed * dt
            if obstacle.distance <= 1e-6:
                continue

            obstacle.progress += obstacle.direction * travel / obstacle.distance
            if obstacle.progress > 1.0:
                obstacle.progress = 1.0 - (obstacle.progress - 1.0)
                obstacle.direction *= -1
            elif obstacle.progress < 0.0:
                obstacle.progress = -obstacle.progress
                obstacle.direction *= -1

            x = obstacle.start[0] + obstacle.progress * (obstacle.end[0] - obstacle.start[0])
            y = obstacle.start[1] + obstacle.progress * (obstacle.end[1] - obstacle.start[1])
            z = obstacle.start[2] + self._box_size[2] * 0.5
            yaw = math.atan2(obstacle.end[1] - obstacle.start[1],
                             obstacle.end[0] - obstacle.start[0])
            quat = Quaternion()
            quat.x = 0.0
            quat.y = 0.0
            quat.z = math.sin(yaw * 0.5)
            quat.w = math.cos(yaw * 0.5)

            request = SetEntityPose.Request()
            request.entity = Entity()
            request.entity.name = obstacle.name
            request.entity.type = Entity.MODEL
            pose = Pose()
            pose.position.x = x
            pose.position.y = y
            pose.position.z = z
            pose.orientation = quat
            request.pose = pose
            self._set_pose_service.call_async(request)


def main(args: List[str] | None = None) -> None:
    rclpy.init(args=args)
    node = MovingObstacles()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
