#!/usr/bin/env python3
"""
Gazebo Model Pose Publisher
Queries model poses from Gazebo state service and publishes to ROS topics.
Works for moving models with TrajectoryFollower plugin.
"""

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Pose
from ros_gz_interfaces.srv import GetEntityState


class GazeboModelPosePublisher(Node):
    def __init__(self):
        super().__init__('gazebo_model_pose_publisher')

        # Parameters
        self.declare_parameter('model_names', ['walker_center', 'walker_east'])
        self.declare_parameter('world_name', 'moving_obstacles_world')
        self.declare_parameter('publish_rate', 10.0)

        self.model_names = self.get_parameter('model_names').value
        self.world_name = self.get_parameter('world_name').value
        self.publish_rate = self.get_parameter('publish_rate').value

        # Create service client to query Gazebo state
        self.state_client = self.create_client(
            GetEntityState,
            f'/world/{self.world_name}/state'
        )

        # Create publishers for each model
        self.model_pubs = {}
        for model_name in self.model_names:
            topic_name = f'/model/{model_name}/pose'
            self.model_pubs[model_name] = self.create_publisher(
                Pose,
                topic_name,
                10
            )
            self.get_logger().info(f'Publishing to: {topic_name}')

        # Timer to query and publish poses
        timer_period = 1.0 / self.publish_rate
        self.timer = self.create_timer(timer_period, self.query_and_publish)

        self.get_logger().info(f'Gazebo Model Pose Publisher initialized')
        self.get_logger().info(f'Querying {len(self.model_names)} models from Gazebo')
        self.get_logger().info(f'Publishing at {self.publish_rate} Hz')

        # Wait for service
        self.get_logger().info('Waiting for Gazebo state service...')
        self.service_available = False

    def query_and_publish(self):
        """Query model poses from Gazebo and publish to ROS topics"""
        # Check service availability
        if not self.service_available:
            if self.state_client.service_is_ready():
                self.service_available = True
                self.get_logger().info('Gazebo state service ready!')
            else:
                return  # Service not ready yet

        for model_name in self.model_names:
            self.query_model_pose(model_name)

    def query_model_pose(self, model_name):
        """Query a single model's pose from Gazebo"""
        request = GetEntityState.Request()
        request.name = model_name
        request.reference_frame = ''  # World frame

        future = self.state_client.call_async(request)
        future.add_done_callback(
            lambda f, name=model_name: self.handle_pose_response(f, name)
        )

    def handle_pose_response(self, future, model_name):
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
                self.model_pubs[model_name].publish(pose_msg)
            else:
                if not hasattr(self, '_warned_models'):
                    self._warned_models = set()
                if model_name not in self._warned_models:
                    self.get_logger().warn(f'Failed to get pose for {model_name}')
                    self._warned_models.add(model_name)
        except Exception as e:
            self.get_logger().error(f'Error querying {model_name}: {str(e)}')


def main(args=None):
    rclpy.init(args=args)
    node = GazeboModelPosePublisher()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
