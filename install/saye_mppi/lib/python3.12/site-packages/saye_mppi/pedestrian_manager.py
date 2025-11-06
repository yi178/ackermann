from __future__ import annotations

import math
from dataclasses import dataclass
import time
import os
from typing import List

from ament_index_python.packages import get_package_share_directory
import rclpy
from geometry_msgs.msg import Pose
from rclpy.node import Node
from rclpy.qos import QoSProfile
from ros_gz_interfaces.msg import Entity
from ros_gz_interfaces.srv import SpawnEntity
import yaml


@dataclass
class Waypoint:
    time: float
    pose: List[float]  # [x, y, z, roll, pitch, yaw] in world frame


@dataclass
class ActorSpec:
    name: str
    animation: str
    skin_uri: str
    loop: bool
    auto_start: bool
    initial_pose: List[float]  # world [x, y, z, roll, pitch, yaw]
    waypoints: List[Waypoint]


class PedestrianManager(Node):
    """Spawn walking actors based on YAML-defined trajectories."""

    def __init__(self) -> None:
        super().__init__('pedestrian_manager')
        self.declare_parameter('world_name', 'dynamic_corridor_world')
        self.declare_parameter('spawn_timeout', 5.0)
        self.declare_parameter('actors', [])
        self.declare_parameter('actors_file', '')

        world = self.get_parameter('world_name').get_parameter_value().string_value
        spawn_timeout = float(self.get_parameter('spawn_timeout').value)
        actors_file_param = self.get_parameter('actors_file').get_parameter_value().string_value
        actor_list = None

        if actors_file_param:
            resolved_path = self._resolve_actors_file(actors_file_param)
            self.get_logger().info(f'Loading actor specifications from {resolved_path}')
            try:
                with open(resolved_path, 'r', encoding='utf-8') as fp:
                    actor_list = yaml.safe_load(fp) or []
            except (OSError, yaml.YAMLError) as exc:
                raise ValueError(f'Failed to load actors_file "{resolved_path}": {exc}') from exc
        else:
            raw_actors = self.get_parameter('actors').value
            if isinstance(raw_actors, str):
                try:
                    actor_list = yaml.safe_load(raw_actors)
                except yaml.YAMLError as exc:
                    raise ValueError(f'Failed to parse actors parameter: {exc}') from exc
            else:
                actor_list = raw_actors

        if not actor_list:
            self.get_logger().warn('No actors configured; shutting down.')
            rclpy.shutdown()
            return

        self._actors: List[ActorSpec] = []
        for entry in actor_list:
            spec = self._parse_actor(entry)
            self._actors.append(spec)

        self._spawn_client = self.create_client(
            SpawnEntity,
            f'/world/{world}/spawn_entity',
            qos_profile=QoSProfile(depth=1),
        )

        self.get_logger().info(
            f'Connecting to /world/{world}/spawn_entity to spawn {len(self._actors)} actors.')

        if not self._wait_for_spawn_service(spawn_timeout):
            self.get_logger().error(
                f'spawn_entity service not available after {spawn_timeout} seconds; aborting.')
            rclpy.shutdown()
            return

        for actor in self._actors:
            sdf_string = self._build_actor_sdf(actor)
            request = SpawnEntity.Request()
            request.entity_factory.name = actor.name
            request.entity_factory.sdf = sdf_string
            request.entity_factory.pose = self._list_to_pose(actor.initial_pose)
            request.entity_factory.allow_renaming = False

            future = self._spawn_client.call_async(request)
            rclpy.spin_until_future_complete(self, future, timeout_sec=spawn_timeout)
            if not future.result() or not future.result().success:
                result = future.result()
                raise RuntimeError(f'Failed to spawn actor {actor.name}: {result}')

            self.get_logger().info(f'Spawned actor {actor.name}.')

        # Once actors are spawned the node can exit.
        self.get_logger().info('All configured actors spawned; shutting down.')
        self.create_timer(0.1, self._shutdown_once)
        self._shutdown_requested = False

    def _shutdown_once(self) -> None:
        if not self._shutdown_requested:
            self._shutdown_requested = True
            self.get_logger().debug('Shutting down pedestrian_manager node.')
            rclpy.shutdown()

    def _parse_actor(self, entry) -> ActorSpec:
        try:
            name = entry['name']
            animation = entry.get('animation', 'walking')
            skin_uri = entry.get('skin_uri', 'model://walking_actor/meshes/walk.dae')
            loop = bool(entry.get('loop', True))
            auto_start = bool(entry.get('auto_start', True))
            initial_pose = list(entry.get('initial_pose', [0.0, 0.0, 1.0, 0.0, 0.0, 0.0]))
            waypoints_raw = entry['waypoints']
        except KeyError as exc:
            raise ValueError(f'Missing required actor field: {exc}') from exc

        waypoints = []
        for wp in waypoints_raw:
            if 'pose' not in wp or 'time' not in wp:
                raise ValueError(f'Waypoint must contain time and pose: {wp}')
            pose = list(wp['pose'])
            if len(pose) != 6:
                raise ValueError(f'Waypoint pose must have 6 elements [x, y, z, roll, pitch, yaw]: {wp}')
            waypoints.append(Waypoint(float(wp['time']), pose))

        if waypoints[0].time > 0.0:
            self.get_logger().warn(
                f'First waypoint for actor {name} does not start at t=0. '
                'The trajectory will be shifted to start at zero.')
            offset = waypoints[0].time
            waypoints = [Waypoint(wp.time - offset, wp.pose) for wp in waypoints]

        return ActorSpec(
            name=name,
            animation=animation,
            skin_uri=skin_uri,
            loop=loop,
            auto_start=auto_start,
            initial_pose=initial_pose,
            waypoints=waypoints,
        )

    def _resolve_actors_file(self, path: str) -> str:
        if os.path.isabs(path):
            return path

        try:
            pkg_share = get_package_share_directory('saye_mppi')
        except Exception:
            pkg_share = None

        if pkg_share:
            candidate = os.path.join(pkg_share, 'config', path)
            if os.path.exists(candidate):
                return candidate

        return path

    def _build_actor_sdf(self, actor: ActorSpec) -> str:
        initial = actor.initial_pose
        rel_waypoints = []
        for wp in actor.waypoints:
            rel_pose = self._relative_pose(initial, wp.pose)
            rel_waypoints.append((wp.time, rel_pose))

        waypoint_strs = []
        for time_value, pose in rel_waypoints:
            pose_str = ' '.join(f'{value:.6f}' for value in pose)
            waypoint_strs.append(
                f'''          <waypoint>
            <time>{time_value:.6f}</time>
            <pose>{pose_str}</pose>
          </waypoint>'''
            )

        loop_str = 'true' if actor.loop else 'false'
        auto_start_str = 'true' if actor.auto_start else 'false'

        sdf = f'''<?xml version="1.0" ?>
<sdf version="1.6">
  <actor name="{actor.name}">
    <skin>
      <filename>{actor.skin_uri}</filename>
      <scale>1.0</scale>
    </skin>
    <pose>0 0 0 0 0 0</pose>
    <animation name="{actor.animation}">
      <filename>{actor.skin_uri}</filename>
      <scale>1.0</scale>
      <interpolate_x>true</interpolate_x>
    </animation>
    <script>
      <loop>{loop_str}</loop>
      <delay_start>0.0</delay_start>
      <auto_start>{auto_start_str}</auto_start>
      <trajectory id="0" type="{actor.animation}">
{chr(10).join(waypoint_strs)}
      </trajectory>
    </script>
  </actor>
</sdf>
'''
        return sdf

    def _list_to_pose(self, values: List[float]) -> Pose:
        pose = Pose()
        pose.position.x = values[0]
        pose.position.y = values[1]
        pose.position.z = values[2]
        pose.orientation = self._rpy_to_quaternion(values[3], values[4], values[5])
        return pose

    def _relative_pose(self, initial: List[float], target: List[float]) -> List[float]:
        dx = target[0] - initial[0]
        dy = target[1] - initial[1]
        dz = target[2] - initial[2]
        roll = target[3] - initial[3]
        pitch = target[4] - initial[4]
        yaw = target[5] - initial[5]

        cos_yaw = math.cos(initial[5])
        sin_yaw = math.sin(initial[5])

        rel_x = cos_yaw * dx + sin_yaw * dy
        rel_y = -sin_yaw * dx + cos_yaw * dy

        return [rel_x, rel_y, dz, roll, pitch, yaw]

    @staticmethod
    def _rpy_to_quaternion(roll: float, pitch: float, yaw: float):
        from geometry_msgs.msg import Quaternion
        half_roll = roll * 0.5
        half_pitch = pitch * 0.5
        half_yaw = yaw * 0.5

        sin_r = math.sin(half_roll)
        cos_r = math.cos(half_roll)
        sin_p = math.sin(half_pitch)
        cos_p = math.cos(half_pitch)
        sin_y = math.sin(half_yaw)
        cos_y = math.cos(half_yaw)

        quat = Quaternion()
        quat.x = sin_r * cos_p * cos_y - cos_r * sin_p * sin_y
        quat.y = cos_r * sin_p * cos_y + sin_r * cos_p * sin_y
        quat.z = cos_r * cos_p * sin_y - sin_r * sin_p * cos_y
        quat.w = cos_r * cos_p * cos_y + sin_r * sin_p * sin_y
        return quat

    def _wait_for_spawn_service(self, timeout: float) -> bool:
        """Wait for spawn_entity service to become available.

        Returns True if the service becomes available, False otherwise.
        """
        if timeout is None or timeout <= 0.0:
            while not self._spawn_client.wait_for_service(timeout_sec=1.0):
                self.get_logger().info('Waiting for spawn_entity service to become available...')
            return True

        deadline = time.time() + timeout
        while time.time() < deadline:
            remaining = max(0.1, deadline - time.time())
            if self._spawn_client.wait_for_service(timeout_sec=min(1.0, remaining)):
                return True
            self.get_logger().info('Waiting for spawn_entity service to become available...')
        return False


def main(args=None) -> None:
    rclpy.init(args=args)
    node = None
    try:
        node = PedestrianManager()
        if node is not None:
            rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        if node is not None:
            node.destroy_node()


if __name__ == '__main__':
    main()
