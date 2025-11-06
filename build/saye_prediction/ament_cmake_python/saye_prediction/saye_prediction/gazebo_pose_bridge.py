#!/usr/bin/env python3
"""
Gazebo Actor Pose Bridge
将Gazebo的Pose_V话题转换成TFMessage，供预测器消费。

订阅: /world/*/pose/info (gz.msgs.Pose_V)
发布: /actor_pose_bridge (tf2_msgs/TFMessage)
"""

import threading
from typing import Dict, List

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from tf2_msgs.msg import TFMessage

try:
    from gz.transport13 import Node as GzNode  # pylint: disable=import-error
    from gz.msgs.pose_v_pb2 import Pose_V  # pylint: disable=import-error
except ImportError as exc:  # pragma: no cover - environment dependent
    raise ImportError(
        'Failed to import Gazebo transport Python bindings. '
        'Ensure ros-gz is installed (gz-transport13).'
    ) from exc


class GazeboPoseBridge(Node):
    """Bridge Gazebo Pose_V topic to TFMessage with selected actors."""

    def __init__(self):
        super().__init__('gazebo_pose_bridge')

        self.declare_parameter('pose_topic', '/world/dynamic_corridor_world/pose/info')
        self.declare_parameter('output_topic', '/actor_pose_bridge')
        self.declare_parameter('actor_names', ['walker_center', 'walker_east'])
        self.declare_parameter('frame_id', 'odom')

        self.pose_topic = self.get_parameter('pose_topic').value
        self.output_topic = self.get_parameter('output_topic').value
        self.frame_id = self.get_parameter('frame_id').value
        actor_names = self.get_parameter('actor_names').value
        self.actor_names = set(actor_names)

        self.publisher = self.create_publisher(TFMessage, self.output_topic, 10)

        self._lock = threading.Lock()
        self._pending_transforms: Dict[str, TransformStamped] = {}

        self.timer = self.create_timer(0.05, self._publish_pending)

        self.gz_node = GzNode()
        if not self.gz_node.subscribe(self.pose_topic, Pose_V, self._handle_pose):
            self.get_logger().error(f'Failed to subscribe to {self.pose_topic}')
        else:
            self.get_logger().info(
                f'Subscribed to Gazebo topic: {self.pose_topic} (tracking: {sorted(self.actor_names)})'
            )
            self.get_logger().info(
                f'Publishing TFMessage on: {self.output_topic} (frame_id={self.frame_id})'
            )

    def _handle_pose(self, msg: Pose_V):
        """Gazebo callback executed in a separate thread."""
        selected: List[TransformStamped] = []
        stamp = self.get_clock().now().to_msg()

        for pose in msg.pose:
            name = pose.name
            if not name or name not in self.actor_names:
                continue

            transform = TransformStamped()
            transform.header.stamp = stamp
            transform.header.frame_id = self.frame_id
            transform.child_frame_id = name
            transform.transform.translation.x = pose.position.x
            transform.transform.translation.y = pose.position.y
            transform.transform.translation.z = pose.position.z
            transform.transform.rotation.x = pose.orientation.x
            transform.transform.rotation.y = pose.orientation.y
            transform.transform.rotation.z = pose.orientation.z
            transform.transform.rotation.w = pose.orientation.w
            selected.append(transform)

        if not selected:
            return

        with self._lock:
            for transform in selected:
                self._pending_transforms[transform.child_frame_id] = transform

    def _publish_pending(self):
        with self._lock:
            if not self._pending_transforms:
                return
            transforms = list(self._pending_transforms.values())
            self._pending_transforms.clear()

        msg = TFMessage()
        msg.transforms = transforms
        self.publisher.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = GazeboPoseBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':  # pragma: no cover
    main()
