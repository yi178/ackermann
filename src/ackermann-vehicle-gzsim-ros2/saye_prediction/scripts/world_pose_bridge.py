#!/usr/bin/env python3
"""
World Pose Bridge - Extracts walker poses from world pose/info and publishes individually
"""

import rclpy
from rclpy.node import Node
from tf2_msgs.msg import TFMessage
from geometry_msgs.msg import Pose


class WorldPoseBridge(Node):
    def __init__(self):
        super().__init__('world_pose_bridge')

        # Parameters
        self.declare_parameter('walker_names', ['walker_center', 'walker_east'])
        self.declare_parameter('world_name', 'moving_obstacles_world')

        self.walker_names = self.get_parameter('walker_names').value
        self.world_name = self.get_parameter('world_name').value

        # Subscribe to world pose/info
        self.world_pose_sub = self.create_subscription(
            TFMessage,
            f'/world/{self.world_name}/pose/info',
            self.world_pose_callback,
            10
        )

        # Create publishers for each walker
        self.walker_pubs = {}
        for name in self.walker_names:
            topic = f'/model/{name}/pose'
            self.walker_pubs[name] = self.create_publisher(Pose, topic, 10)
            self.get_logger().info(f'Publishing to: {topic}')

        self.get_logger().info(f'World Pose Bridge initialized for {len(self.walker_names)} walkers')

    def world_pose_callback(self, msg: TFMessage):
        """Extract walker poses from TFMessage and republish"""
        for transform in msg.transforms:
            # Check if this transform is for one of our walkers
            frame_id = transform.child_frame_id

            for walker_name in self.walker_names:
                if walker_name in frame_id or frame_id == walker_name:
                    # Create Pose message
                    pose = Pose()
                    pose.position.x = transform.transform.translation.x
                    pose.position.y = transform.transform.translation.y
                    pose.position.z = transform.transform.translation.z
                    pose.orientation.x = transform.transform.rotation.x
                    pose.orientation.y = transform.transform.rotation.y
                    pose.orientation.z = transform.transform.rotation.z
                    pose.orientation.w = transform.transform.rotation.w

                    # Publish
                    self.walker_pubs[walker_name].publish(pose)


def main(args=None):
    rclpy.init(args=args)
    node = WorldPoseBridge()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
