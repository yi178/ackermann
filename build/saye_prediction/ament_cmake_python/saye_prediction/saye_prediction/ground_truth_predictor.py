#!/usr/bin/env python3
"""
Ground Truth Predictor for RAPA-MPPI
基于Gazebo Actor真实位置的GMM预测器

功能：
1. 订阅Gazebo actor的真实位置
2. 估计速度（历史位置差分）
3. 使用恒速模型（CV）生成GMM预测
4. 发布PedestrianPredictions消息

Author: RAPA-MPPI Team
Date: 2025-10
"""

import rclpy
from rclpy.node import Node
from saye_msgs.msg import PedestrianPredictions, PedestrianPrediction, GMMComponent
from geometry_msgs.msg import Quaternion
from tf2_msgs.msg import TFMessage
from collections import deque
import numpy as np
import math


class GroundTruthPredictor(Node):
    """
    基于Gazebo Actor真实位置的预测器

    订阅: /actor_pose_bridge (TFMessage，由GazeboPoseBridge提供)
    发布: /pedestrian_predictions (PedestrianPredictions)
    """

    def __init__(self):
        super().__init__('ground_truth_predictor')

        # ============ 参数配置 ============
        self.declare_parameter('prediction_horizon', 2.8)   # 预测时域（秒）
        self.declare_parameter('prediction_dt', 0.05)       # 时间步长（秒）
        self.declare_parameter('actor_names', [             # 要追踪的actor列表
            'walker_center',
            'walker_east'
        ])
        self.declare_parameter('history_size', 5)           # 历史帧数（用于速度估计）
        self.declare_parameter('sigma_0', 0.1)              # 初始位置不确定性（米）
        self.declare_parameter('sigma_q', 0.5)              # 过程噪声强度（m/s）
        self.declare_parameter('publish_rate', 10.0)        # 发布频率（Hz）
        self.declare_parameter('pose_topic',
                               '/world/moving_obstacles_world/pose/info')  # World pose/info topic
        self.declare_parameter('world_name', 'moving_obstacles_world')  # World name

        # 读取参数
        self.horizon = self.get_parameter('prediction_horizon').value
        self.dt = self.get_parameter('prediction_dt').value
        self.num_steps = int(self.horizon / self.dt)  # 默认56步
        self.sigma_0 = self.get_parameter('sigma_0').value
        self.sigma_q = self.get_parameter('sigma_q').value

        self.get_logger().info(f'Prediction configuration:')
        self.get_logger().info(f'  Horizon: {self.horizon}s')
        self.get_logger().info(f'  Time step: {self.dt}s')
        self.get_logger().info(f'  Number of steps: {self.num_steps}')

        # ============ 追踪状态 ============
        self.actor_states = {}  # {actor_name: {'history': deque, 'last_time': float}}

        actor_names = self.get_parameter('actor_names').value
        history_size = self.get_parameter('history_size').value
        for name in actor_names:
            self.actor_states[name] = {
                'history': deque(maxlen=history_size),
                'last_time': None
            }

        # ============ 订阅Pose_V流 ============
        pose_topic = self.get_parameter('pose_topic').value
        self.pose_sub = self.create_subscription(
            TFMessage,
            pose_topic,
            self.pose_stream_callback,
            50
        )
        self.get_logger().info(f'Subscribed to pose topic: {pose_topic}')

        # ============ 发布预测 ============
        self.prediction_pub = self.create_publisher(
            PedestrianPredictions,
            '/pedestrian_predictions',
            10
        )

        # ============ 定时发布 ============
        publish_rate = self.get_parameter('publish_rate').value
        self.timer = self.create_timer(1.0 / publish_rate, self.publish_predictions)

        # ============ Walker tracking by position ============
        # Since child_frame_id is empty, we track walkers by transform index
        # Walkers have Z ≈ 0.3m, we'll assign names based on Y coordinate
        self.walker_index_map = {}  # {transform_index: actor_name}

        self.get_logger().info(f'Ground Truth Predictor initialized')
        self.get_logger().info(f'Tracking {len(actor_names)} actors: {actor_names}')
        self.get_logger().info(f'Publishing at {publish_rate} Hz')
        self.get_logger().info(f'Will identify walkers by position (Z ≈ 0.3m)')

    def pose_stream_callback(self, msg: TFMessage):
        """
        处理来自 /world/*/pose/info 的 Pose_V 数据

        由于 TFMessage 中 child_frame_id 为空，我们通过以下策略识别 walkers：
        1. Z 坐标约 0.3m 的是 walkers
        2. 通过 transform index 建立持久映射（一旦识别就固定）
        3. 对未识别的 walker，分配给尚未被追踪的 actor
        """
        current_time = self.get_clock().now().nanoseconds / 1e9

        # Find which actors still need to be detected
        detected_actors = set(self.walker_index_map.values())
        needed_actors = [name for name in self.actor_states.keys()
                        if name not in detected_actors]

        for idx, transform in enumerate(msg.transforms):
            translation = transform.transform.translation
            z = translation.z

            # Filter by Z coordinate: walkers are at Z ≈ 0.3m
            if not (0.25 < z < 0.35):
                continue

            # Check if this transform index already has a mapping
            if idx in self.walker_index_map:
                actor_name = self.walker_index_map[idx]
            else:
                # New walker detected, assign to next needed actor
                if not needed_actors:
                    continue  # All walkers already assigned

                actor_name = needed_actors.pop(0)
                self.walker_index_map[idx] = actor_name
                self.get_logger().info(
                    f'Detected {actor_name} at transform index {idx} '
                    f'(position: x={translation.x:.2f}, y={translation.y:.2f}, z={z:.2f})'
                )

            stamp = transform.header.stamp
            if stamp.sec == 0 and stamp.nanosec == 0:
                timestamp = current_time
            else:
                timestamp = stamp.sec + stamp.nanosec / 1e9

            rotation = transform.transform.rotation

            orientation = Quaternion()
            orientation.x = rotation.x
            orientation.y = rotation.y
            orientation.z = rotation.z
            orientation.w = rotation.w

            state = self.actor_states[actor_name]
            state['history'].append({
                'time': timestamp,
                'x': translation.x,
                'y': translation.y,
                'z': translation.z,
                'orientation': orientation
            })
            state['last_time'] = timestamp

    def estimate_velocity(self, actor_name):
        """
        估计当前速度（线性回归）

        使用历史位置队列进行最小二乘拟合

        Args:
            actor_name: actor名称

        Returns:
            tuple: (vx, vy, speed) - x方向速度、y方向速度、速度大小
        """
        state = self.actor_states[actor_name]
        history = list(state['history'])

        if len(history) < 2:
            return 0.0, 0.0, 0.0

        # 提取时间和位置数组
        times = np.array([h['time'] for h in history])
        xs = np.array([h['x'] for h in history])
        ys = np.array([h['y'] for h in history])

        # 计算总时间间隔
        dt_total = times[-1] - times[0]
        if dt_total < 1e-6:
            return 0.0, 0.0, 0.0

        # 简单差分法（也可以用polyfit进行更稳定的拟合）
        vx = (xs[-1] - xs[0]) / dt_total
        vy = (ys[-1] - ys[0]) / dt_total
        speed = np.sqrt(vx**2 + vy**2)

        return vx, vy, speed

    def predict_trajectory_cv(self, x0, y0, vx, vy):
        """
        恒速模型（Constant Velocity）预测

        状态方程:
            x(t) = x(0) + vx * t
            y(t) = y(0) + vy * t

        不确定性传播:
            Σ(t) = Σ(0) + Q * t²

        Args:
            x0: 初始x位置
            y0: 初始y位置
            vx: x方向速度
            vy: y方向速度

        Returns:
            GMMComponent: GMM分量消息
        """
        gmm = GMMComponent()
        gmm.weight = 1.0  # 单模态，权重100%

        # 预测每个时间步
        gmm.mean_x = []
        gmm.mean_y = []
        gmm.cov_xx = []
        gmm.cov_yy = []
        gmm.cov_xy = []

        for step in range(self.num_steps):
            t = step * self.dt

            # 均值预测：μ(t) = μ(0) + v*t
            mean_x = x0 + vx * t
            mean_y = y0 + vy * t

            gmm.mean_x.append(mean_x)
            gmm.mean_y.append(mean_y)

            # 协方差预测：Σ(t) = Σ(0) + Q*t²
            # 这里假设过程噪声随时间平方增长
            var_t = self.sigma_0**2 + (self.sigma_q * t)**2

            gmm.cov_xx.append(var_t)
            gmm.cov_yy.append(var_t)
            gmm.cov_xy.append(0.0)  # 假设x,y独立（可以改进）

        return gmm

    def publish_predictions(self):
        """
        发布所有行人的预测

        遍历所有追踪的actor，生成预测并发布
        """
        msg = PedestrianPredictions()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'odom'  # 预测在odom坐标系下

        current_time = self.get_clock().now().nanoseconds / 1e9

        num_predicted = 0

        for actor_name, state in self.actor_states.items():
            # ========== 数据新鲜度检查 ==========
            if state['last_time'] is None:
                self.get_logger().warn(f'No data received for {actor_name}',
                                      throttle_duration_sec=5.0)
                continue

            # 检查数据是否过期（0.5秒内）
            data_age = current_time - state['last_time']
            if data_age > 0.5:
                self.get_logger().warn(f'Stale data for {actor_name}: {data_age:.2f}s old',
                                      throttle_duration_sec=5.0)
                continue

            # ========== 历史数据充足性检查 ==========
            if len(state['history']) < 2:
                self.get_logger().debug(f'Insufficient history for {actor_name}')
                continue

            # ========== 获取当前状态 ==========
            latest = state['history'][-1]
            x0, y0, z0 = latest['x'], latest['y'], latest['z']

            # ========== 估计速度 ==========
            vx, vy, speed = self.estimate_velocity(actor_name)

            # ========== 创建预测消息 ==========
            pred = PedestrianPrediction()
            pred.id = actor_name

            # 当前位姿
            pred.current_pose.position.x = x0
            pred.current_pose.position.y = y0
            pred.current_pose.position.z = z0
            pred.current_pose.orientation = latest['orientation']

            # 当前速度
            pred.current_speed = speed

            # ========== 生成GMM预测 ==========
            gmm = self.predict_trajectory_cv(x0, y0, vx, vy)
            pred.gmm_modes.append(gmm)

            msg.predictions.append(pred)
            num_predicted += 1

        # ========== 发布 ==========
        if num_predicted > 0:
            self.prediction_pub.publish(msg)
            self.get_logger().debug(f'Published predictions for {num_predicted} pedestrians')
        else:
            self.get_logger().debug('No predictions to publish', throttle_duration_sec=1.0)


def main(args=None):
    rclpy.init(args=args)
    node = GroundTruthPredictor()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
