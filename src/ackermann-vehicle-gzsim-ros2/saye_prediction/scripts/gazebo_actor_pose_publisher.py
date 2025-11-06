#!/usr/bin/env python3
"""
Gazebo Actor Pose Publisher
Queries actor poses from Gazebo and publishes to ROS topics.
This is a workaround since Gazebo Harmonic actors don't auto-publish poses.
"""

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Pose
from ros_gz_interfaces.srv import GetEntityState
import math


class GazeboActorPosePublisher(Node):
    def __init__(self):
        super().__init__('gazebo_actor_pose_publisher')

        # Parameters
        self.declare_parameter('actor_names', ['walker_center', 'walker_east'])
        self.declare_parameter('world_name', 'dynamic_corridor_world')
        self.declare_parameter('publish_rate', 10.0)

        self.actor_names = self.get_parameter('actor_names').value
        self.world_name = self.get_parameter('world_name').value
        self.publish_rate = self.get_parameter('publish_rate').value

        # Create service client to query Gazebo state
        self.state_client = self.create_client(
            GetEntityState,
            '/world/{}/state'.format(self.world_name)
        )

        # Create publishers for each actor
        self.publishers = {}
        for actor_name in self.actor_names:
            topic_name = f'/model/{actor_name}/pose'
            self.publishers[actor_name] = self.create_publisher(
                Pose,
                topic_name,
                10
            )
            self.get_logger().info(f'Publishing to: {topic_name}')

        # Timer to query and publish poses
        timer_period = 1.0 / self.publish_rate
        self.timer = self.create_timer(timer_period, self.query_and_publish)

        self.get_logger().info(f'Gazebo Actor Pose Publisher initialized')
        self.get_logger().info(f'Querying {len(self.actor_names)} actors from Gazebo')
        self.get_logger().info(f'Publishing at {self.publish_rate} Hz')

        # Wait for service
        self.get_logger().info('Waiting for Gazebo state service...')
        self.service_available = False

    def query_and_publish(self):
        """Query actor poses from Gazebo and publish to ROS topics"""
        # Check service availability
        if not self.service_available:
            if self.state_client.service_is_ready():
                self.service_available = True
                self.get_logger().info('Gazebo state service ready!')
            else:
                return  # Service not ready yet

        for actor_name in self.actor_names:
            self.query_actor_pose(actor_name)

    def query_actor_pose(self, actor_name):
        """Query a single actor's pose from Gazebo"""
        request = GetEntityState.Request()
        request.name = actor_name
        request.reference_frame = ''  # World frame

        future = self.state_client.call_async(request)
        future.add_done_callback(
            lambda f, name=actor_name: self.handle_pose_response(f, name)
        )

    def handle_pose_response(self, future, actor_name):
        """Handle the response from Gazebo state service"""
        try:
            response = future.result()
            if response.success:
                # Create Pose message
                pose_msg = Pose()
                pose_msg.position.x = response.state.pose.position.x
                pose_msg.position.y = response.state.pose.position.y
                pose_msg.position.z = response.state.pose.position.z
                pose_msg.orientation.x = response.state.pose.orientation.x
                pose_msg.orientation.y = response.state.pose.orientation.y
                pose_msg.orientation.z = response.state.pose.orientation.z
                pose_msg.orientation.w = response.state.pose.orientation.w

                # Publish
                self.publishers[actor_name].publish(pose_msg)
            else:
                if not hasattr(self, '_warned_actors'):
                    self._warned_actors = set()
                if actor_name not in self._warned_actors:
                    self.get_logger().warn(f'Failed to get pose for {actor_name}')
                    self._warned_actors.add(actor_name)
        except Exception as e:
            self.get_logger().error(f'Error querying {actor_name}: {str(e)}')


def main(args=None):
    rclpy.init(args=args)
    node = GazeboActorPosePublisher()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
