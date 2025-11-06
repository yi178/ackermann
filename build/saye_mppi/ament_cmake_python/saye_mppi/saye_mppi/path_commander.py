import math
from typing import List

import rclpy
from rclpy.action import ActionClient
from rclpy.duration import Duration
from rclpy.node import Node
from nav2_msgs.action import FollowPath
from nav_msgs.msg import Path, Odometry
from geometry_msgs.msg import PoseStamped


def yaw_from_quaternion(x: float, y: float, z: float, w: float) -> float:
    """Compute yaw from quaternion components."""
    siny_cosp = 2.0 * (w * z + x * y)
    cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
    return math.atan2(siny_cosp, cosy_cosp)


def quaternion_from_yaw(yaw: float) -> List[float]:
    """Return quaternion [x, y, z, w] representing yaw rotation."""
    half = yaw * 0.5
    return [0.0, 0.0, math.sin(half), math.cos(half)]


class StraightPathCommander(Node):
    """Send a straight nav_msgs/Path goal to the MPPI controller."""

    def __init__(self) -> None:
        super().__init__('straight_path_commander')

        self.declare_parameter('base_frame', 'saye/base_link')
        self.declare_parameter('path_frame', 'odom')
        self.declare_parameter('goal', [4.0, 0.0, 0.0])  # x, y, yaw
        self.declare_parameter('path_resolution', 0.25)
        self.declare_parameter('goal_tolerance', 0.05)

        self._base_frame = self.get_parameter('base_frame').get_parameter_value().string_value
        self._path_frame = self.get_parameter('path_frame').get_parameter_value().string_value
        goal_param = self.get_parameter('goal').get_parameter_value().double_array_value
        if len(goal_param) < 3:
            raise ValueError('Goal parameter must contain [x, y, yaw]')
        self._goal = goal_param[:3]

        self._resolution = float(
            self.get_parameter('path_resolution').get_parameter_value().double_value)
        self._goal_tolerance = float(
            self.get_parameter('goal_tolerance').get_parameter_value().double_value)
        self._latest_pose: PoseStamped | None = None
        self._frame_warning_logged = False
        self._odom_sub = self.create_subscription(
            Odometry,
            'odom',
            self._odom_callback,
            10,
        )

        self._action_client = ActionClient(self, FollowPath, 'follow_path')
        self._goal_sent = False

        self._send_timer = self.create_timer(1.0, self._attempt_send_goal)
        self.get_logger().info(
            'StraightPathCommander ready: waiting for controller_server follow_path action')

    def _attempt_send_goal(self) -> None:
        if self._goal_sent:
            return

        if not self._action_client.wait_for_server(timeout_sec=0.1):
            self.get_logger().debug('Waiting for follow_path action server...')
            return

        start_pose = self._lookup_start_pose()
        if start_pose is None:
            self.get_logger().warn('Unable to resolve robot pose; will retry.')
            return

        distance = math.hypot(
            self._goal[0] - start_pose.pose.position.x,
            self._goal[1] - start_pose.pose.position.y)
        if distance < self._goal_tolerance:
            self.get_logger().info('Robot already within goal tolerance; skipping path command.')
            self._goal_sent = True
            return

        path = self._build_straight_path(start_pose, distance)
        goal_msg = FollowPath.Goal()
        goal_msg.path = path
        goal_msg.controller_id = 'FollowPath'

        self.get_logger().info(
            f'Sending straight path with {len(path.poses)} poses to controller_server.')
        send_future = self._action_client.send_goal_async(goal_msg)
        send_future.add_done_callback(self._goal_response_callback)
        self._goal_sent = True

    def _lookup_start_pose(self) -> PoseStamped | None:
        latest = self._latest_pose
        if latest is None:
            return None

        if latest.header.frame_id != self._path_frame:
            if not self._frame_warning_logged:
                self.get_logger().warn(
                    f'Odom frame ({latest.header.frame_id}) does not match path_frame '
                    f'({self._path_frame}); using pose as-is.'
                )
                self._frame_warning_logged = True

        start = PoseStamped()
        start.header.frame_id = self._path_frame
        start.header.stamp = self.get_clock().now().to_msg()
        start.pose = latest.pose
        return start

    def _odom_callback(self, msg: Odometry) -> None:
        pose = PoseStamped()
        pose.header = msg.header
        pose.pose = msg.pose.pose
        self._latest_pose = pose

    def _build_straight_path(self, start_pose: PoseStamped, distance: float) -> Path:
        path = Path()
        path.header.frame_id = self._path_frame
        path.header.stamp = self.get_clock().now().to_msg()

        direction_yaw = math.atan2(
            self._goal[1] - start_pose.pose.position.y,
            self._goal[0] - start_pose.pose.position.x)
        goal_quat = quaternion_from_yaw(self._goal[2])

        num_segments = max(2, int(math.ceil(distance / max(self._resolution, 0.05))) + 1)
        for i in range(num_segments):
            ratio = float(i) / float(num_segments - 1)
            pose = PoseStamped()
            pose.header = path.header
            pose.pose.position.x = (
                start_pose.pose.position.x +
                ratio * (self._goal[0] - start_pose.pose.position.x))
            pose.pose.position.y = (
                start_pose.pose.position.y +
                ratio * (self._goal[1] - start_pose.pose.position.y))
            pose.pose.position.z = 0.0

            heading = direction_yaw if ratio < 0.999 else self._goal[2]
            qx, qy, qz, qw = quaternion_from_yaw(heading)
            pose.pose.orientation.x = qx
            pose.pose.orientation.y = qy
            pose.pose.orientation.z = qz
            pose.pose.orientation.w = qw
            path.poses.append(pose)

        # Ensure final pose exactly matches requested goal orientation.
        path.poses[-1].pose.orientation.x = goal_quat[0]
        path.poses[-1].pose.orientation.y = goal_quat[1]
        path.poses[-1].pose.orientation.z = goal_quat[2]
        path.poses[-1].pose.orientation.w = goal_quat[3]
        path.poses[-1].pose.position.x = self._goal[0]
        path.poses[-1].pose.position.y = self._goal[1]
        path.poses[-1].pose.position.z = 0.0
        return path

    def _goal_response_callback(self, future) -> None:
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('FollowPath goal rejected by controller_server.')
            return

        self.get_logger().info('FollowPath goal accepted; waiting for result.')
        result_future = goal_handle.get_result_async()
        result_future.add_done_callback(self._result_callback)

    def _result_callback(self, future) -> None:
        result = future.result().result
        if result.controller_id:
            self.get_logger().info(
                f'FollowPath completed by controller {result.controller_id} with code {result.error_code.value}.')
        else:
            self.get_logger().info(
                f'FollowPath completed with code {result.error_code.value}.')


def main(args: list[str] | None = None) -> None:
    rclpy.init(args=args)
    node = StraightPathCommander()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
