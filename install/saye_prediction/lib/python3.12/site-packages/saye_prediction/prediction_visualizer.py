#!/usr/bin/env python3
"""
Prediction Visualizer for RAPA-MPPI
将PedestrianPredictions可视化为RViz Marker

功能：
1. 订阅 /pedestrian_predictions
2. 为每个行人生成可视化：
   - 当前位置（球体）
   - 预测轨迹（线条）
   - 不确定性椭圆（每5步显示一个）
3. 发布到 /prediction_markers

Author: RAPA-MPPI Team
Date: 2025-10
"""

import rclpy
from rclpy.node import Node
from saye_msgs.msg import PedestrianPredictions
from visualization_msgs.msg import Marker, MarkerArray
from geometry_msgs.msg import Point
from std_msgs.msg import ColorRGBA
import numpy as np
import math


class PredictionVisualizer(Node):
    """预测可视化节点"""

    def __init__(self):
        super().__init__('prediction_visualizer')

        # ============ 参数配置 ============
        self.declare_parameter('marker_lifetime', 0.2)  # Marker生命周期（秒）
        self.declare_parameter('show_ellipse_interval', 5)  # 每N步显示一个椭圆
        self.declare_parameter('trajectory_line_width', 0.02)  # 轨迹线宽度
        self.declare_parameter('ellipse_resolution', 20)  # 椭圆分辨率

        self.marker_lifetime = self.get_parameter('marker_lifetime').value
        self.ellipse_interval = self.get_parameter('show_ellipse_interval').value
        self.line_width = self.get_parameter('trajectory_line_width').value
        self.ellipse_res = self.get_parameter('ellipse_resolution').value

        # ============ 订阅预测 ============
        self.prediction_sub = self.create_subscription(
            PedestrianPredictions,
            '/pedestrian_predictions',
            self.prediction_callback,
            10
        )

        # ============ 发布Marker ============
        self.marker_pub = self.create_publisher(
            MarkerArray,
            '/prediction_markers',
            10
        )

        # 颜色方案（为不同行人分配不同颜色）
        self.color_map = {}
        self.base_colors = [
            ColorRGBA(r=1.0, g=0.0, b=0.0, a=0.8),  # 红色
            ColorRGBA(r=0.0, g=1.0, b=0.0, a=0.8),  # 绿色
            ColorRGBA(r=0.0, g=0.0, b=1.0, a=0.8),  # 蓝色
            ColorRGBA(r=1.0, g=1.0, b=0.0, a=0.8),  # 黄色
            ColorRGBA(r=1.0, g=0.0, b=1.0, a=0.8),  # 品红
            ColorRGBA(r=0.0, g=1.0, b=1.0, a=0.8),  # 青色
        ]

        self.get_logger().info('Prediction Visualizer initialized')
        self.get_logger().info(f'  Marker lifetime: {self.marker_lifetime}s')
        self.get_logger().info(f'  Ellipse interval: every {self.ellipse_interval} steps')

    def get_color_for_pedestrian(self, ped_id):
        """为行人分配颜色"""
        if ped_id not in self.color_map:
            idx = len(self.color_map) % len(self.base_colors)
            self.color_map[ped_id] = self.base_colors[idx]
        return self.color_map[ped_id]

    def create_ellipse_points(self, center_x, center_y, cov_xx, cov_yy, cov_xy, sigma_scale=2.0):
        """
        创建协方差椭圆的点

        Args:
            center_x, center_y: 中心位置
            cov_xx, cov_yy, cov_xy: 协方差矩阵元素
            sigma_scale: 标准差倍数（2.0表示95%置信区间）

        Returns:
            list of Point: 椭圆上的点
        """
        # 构建协方差矩阵
        cov = np.array([[cov_xx, cov_xy],
                       [cov_xy, cov_yy]])

        # 特征值分解
        eigenvalues, eigenvectors = np.linalg.eig(cov)

        # 椭圆的长短轴
        major_axis = sigma_scale * np.sqrt(eigenvalues[0])
        minor_axis = sigma_scale * np.sqrt(eigenvalues[1])

        # 旋转角度
        angle = np.arctan2(eigenvectors[1, 0], eigenvectors[0, 0])

        # 生成椭圆点
        points = []
        for i in range(self.ellipse_res + 1):
            theta = 2.0 * np.pi * i / self.ellipse_res
            x = major_axis * np.cos(theta)
            y = minor_axis * np.sin(theta)

            # 旋转
            x_rot = x * np.cos(angle) - y * np.sin(angle)
            y_rot = x * np.sin(angle) + y * np.cos(angle)

            # 平移
            point = Point()
            point.x = center_x + x_rot
            point.y = center_y + y_rot
            point.z = 0.1  # 稍微抬高避免与地面重叠

            points.append(point)

        return points

    def prediction_callback(self, msg: PedestrianPredictions):
        """处理预测消息并生成Marker"""
        marker_array = MarkerArray()
        marker_id = 0

        for pred in msg.predictions:
            ped_id = pred.id
            color = self.get_color_for_pedestrian(ped_id)

            # ========== Marker 1: 当前位置（球体） ==========
            current_marker = Marker()
            current_marker.header = msg.header
            current_marker.ns = f"current_{ped_id}"
            current_marker.id = marker_id
            marker_id += 1
            current_marker.type = Marker.SPHERE
            current_marker.action = Marker.ADD
            current_marker.pose = pred.current_pose
            current_marker.scale.x = 0.4
            current_marker.scale.y = 0.4
            current_marker.scale.z = 0.4
            current_marker.color = color
            current_marker.lifetime = rclpy.duration.Duration(seconds=self.marker_lifetime).to_msg()
            marker_array.markers.append(current_marker)

            # ========== Marker 2: 速度文本 ==========
            text_marker = Marker()
            text_marker.header = msg.header
            text_marker.ns = f"speed_{ped_id}"
            text_marker.id = marker_id
            marker_id += 1
            text_marker.type = Marker.TEXT_VIEW_FACING
            text_marker.action = Marker.ADD
            text_marker.pose = pred.current_pose
            text_marker.pose.position.z += 0.5  # 抬高文本
            text_marker.scale.z = 0.3  # 文本大小
            text_marker.color = ColorRGBA(r=1.0, g=1.0, b=1.0, a=1.0)
            text_marker.text = f"{ped_id}\nv={pred.current_speed:.2f}m/s"
            text_marker.lifetime = rclpy.duration.Duration(seconds=self.marker_lifetime).to_msg()
            marker_array.markers.append(text_marker)

            # ========== Marker 3: 预测轨迹（对每个GMM模式） ==========
            for mode_idx, gmm in enumerate(pred.gmm_modes):
                # 轨迹线
                traj_marker = Marker()
                traj_marker.header = msg.header
                traj_marker.ns = f"trajectory_{ped_id}_mode{mode_idx}"
                traj_marker.id = marker_id
                marker_id += 1
                traj_marker.type = Marker.LINE_STRIP
                traj_marker.action = Marker.ADD
                traj_marker.scale.x = self.line_width
                traj_marker.color = color
                traj_marker.color.a = 0.6  # 半透明
                traj_marker.lifetime = rclpy.duration.Duration(seconds=self.marker_lifetime).to_msg()

                # 添加所有预测点
                for i in range(len(gmm.mean_x)):
                    point = Point()
                    point.x = gmm.mean_x[i]
                    point.y = gmm.mean_y[i]
                    point.z = 0.05
                    traj_marker.points.append(point)

                marker_array.markers.append(traj_marker)

                # ========== Marker 4: 不确定性椭圆（每N步显示一个） ==========
                for i in range(0, len(gmm.mean_x), self.ellipse_interval):
                    ellipse_marker = Marker()
                    ellipse_marker.header = msg.header
                    ellipse_marker.ns = f"ellipse_{ped_id}_mode{mode_idx}_step{i}"
                    ellipse_marker.id = marker_id
                    marker_id += 1
                    ellipse_marker.type = Marker.LINE_STRIP
                    ellipse_marker.action = Marker.ADD
                    ellipse_marker.scale.x = self.line_width * 0.5  # 椭圆线细一些
                    ellipse_marker.color = color
                    ellipse_marker.color.a = 0.3  # 更透明
                    ellipse_marker.lifetime = rclpy.duration.Duration(seconds=self.marker_lifetime).to_msg()

                    # 生成椭圆点
                    ellipse_points = self.create_ellipse_points(
                        gmm.mean_x[i],
                        gmm.mean_y[i],
                        gmm.cov_xx[i],
                        gmm.cov_yy[i],
                        gmm.cov_xy[i]
                    )
                    ellipse_marker.points = ellipse_points

                    marker_array.markers.append(ellipse_marker)

        # 发布所有Marker
        self.marker_pub.publish(marker_array)
        self.get_logger().debug(f'Published {len(marker_array.markers)} markers for {len(msg.predictions)} pedestrians')


def main(args=None):
    rclpy.init(args=args)
    node = PredictionVisualizer()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
