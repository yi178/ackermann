#!/usr/bin/env python3
"""
Prediction Verifier for RAPA-MPPI
预测精度验证工具

功能：
1. 同时订阅预测和真值
2. 计算预测误差指标（ADE, FDE, NLL）
3. 实时打印和记录性能

Author: RAPA-MPPI Team
Date: 2025-10
"""

import rclpy
from rclpy.node import Node
from saye_msgs.msg import PedestrianPredictions
from geometry_msgs.msg import Pose
import numpy as np
import math
from collections import deque, defaultdict


class PredictionVerifier(Node):
    """
    预测验证节点

    对比预测轨迹 vs 真实轨迹，计算误差指标
    """

    def __init__(self):
        super().__init__('prediction_verifier')

        # ============ 参数配置 ============
        self.declare_parameter('prediction_dt', 0.05)        # 预测时间步长
        self.declare_parameter('evaluation_times', [0.5, 1.0, 1.4, 2.0, 2.8])  # 评估的时刻（秒）
        self.declare_parameter('actor_names', ['walker_center', 'walker_east'])
        self.declare_parameter('buffer_size', 100)           # 历史缓冲区大小

        self.dt = self.get_parameter('prediction_dt').value
        self.eval_times = self.get_parameter('evaluation_times').value
        self.actor_names = self.get_parameter('actor_names').value

        # ============ 数据缓冲 ============
        self.predictions_buffer = deque(maxlen=self.get_parameter('buffer_size').value)
        self.ground_truth_buffer = defaultdict(lambda: deque(maxlen=self.get_parameter('buffer_size').value))

        # ============ 误差统计 ============
        self.errors = defaultdict(lambda: defaultdict(list))  # {actor: {time: [errors]}}

        # ============ 订阅 ============
        self.pred_sub = self.create_subscription(
            PedestrianPredictions,
            '/pedestrian_predictions',
            self.prediction_callback,
            10
        )

        # 订阅真值
        self.gt_subs = {}
        for name in self.actor_names:
            topic = f'/model/{name}/pose'
            self.gt_subs[name] = self.create_subscription(
                Pose,
                topic,
                lambda msg, n=name: self.ground_truth_callback(n, msg),
                10
            )

        # ============ 定时评估 ============
        self.timer = self.create_timer(5.0, self.print_statistics)

        self.get_logger().info('Prediction Verifier initialized')
        self.get_logger().info(f'Evaluating at times: {self.eval_times}s')

    def prediction_callback(self, msg: PedestrianPredictions):
        """保存预测"""
        timestamp = msg.header.stamp.sec + msg.header.stamp.nanosec / 1e9

        for pred in msg.predictions:
            if len(pred.gmm_modes) > 0:
                gmm = pred.gmm_modes[0]

                self.predictions_buffer.append({
                    'timestamp': timestamp,
                    'id': pred.id,
                    'mean_x': gmm.mean_x,
                    'mean_y': gmm.mean_y,
                    'cov_xx': gmm.cov_xx,
                    'cov_yy': gmm.cov_yy
                })

    def ground_truth_callback(self, actor_name, msg: Pose):
        """保存真值"""
        timestamp = self.get_clock().now().nanoseconds / 1e9

        self.ground_truth_buffer[actor_name].append({
            'timestamp': timestamp,
            'x': msg.position.x,
            'y': msg.position.y
        })

        # 尝试匹配预测
        self.evaluate_predictions(actor_name)

    def evaluate_predictions(self, actor_name):
        """
        评估预测误差

        对于每个评估时刻t，找到t秒前的预测，对比当前真值
        """
        if len(self.ground_truth_buffer[actor_name]) == 0:
            return

        current_gt = self.ground_truth_buffer[actor_name][-1]
        current_time = current_gt['timestamp']
        current_x = current_gt['x']
        current_y = current_gt['y']

        # 遍历每个评估时刻
        for eval_time in self.eval_times:
            # 找eval_time秒前的预测
            target_pred_time = current_time - eval_time
            pred_step = int(eval_time / self.dt)

            # 在预测缓冲区中查找
            for pred_data in self.predictions_buffer:
                if pred_data['id'] != actor_name:
                    continue

                time_diff = abs(pred_data['timestamp'] - target_pred_time)

                # 时间匹配（允许0.1秒误差）
                if time_diff < 0.1:
                    # 检查预测步数
                    if pred_step < len(pred_data['mean_x']):
                        pred_x = pred_data['mean_x'][pred_step]
                        pred_y = pred_data['mean_y'][pred_step]

                        # 计算欧式距离误差
                        error = math.sqrt((pred_x - current_x)**2 + (pred_y - current_y)**2)

                        # 保存误差
                        self.errors[actor_name][eval_time].append(error)

                        self.get_logger().debug(
                            f'{actor_name} @{eval_time}s: error={error:.3f}m',
                            throttle_duration_sec=1.0
                        )

                        break

    def print_statistics(self):
        """打印统计信息"""
        self.get_logger().info('=' * 60)
        self.get_logger().info('Prediction Error Statistics')
        self.get_logger().info('=' * 60)

        for actor_name in self.actor_names:
            if actor_name not in self.errors:
                continue

            self.get_logger().info(f'\nActor: {actor_name}')

            for eval_time in self.eval_times:
                if eval_time not in self.errors[actor_name]:
                    continue

                errors = self.errors[actor_name][eval_time]

                if len(errors) > 0:
                    mean_error = np.mean(errors)
                    std_error = np.std(errors)
                    max_error = np.max(errors)
                    min_error = np.min(errors)

                    self.get_logger().info(
                        f'  @{eval_time}s ({len(errors)} samples): '
                        f'mean={mean_error:.3f}m, '
                        f'std={std_error:.3f}m, '
                        f'max={max_error:.3f}m, '
                        f'min={min_error:.3f}m'
                    )

                    # 目标检查
                    if eval_time == 1.4:
                        if mean_error < 0.3:
                            self.get_logger().info(f'    ✅ PASS: mean error < 0.3m')
                        else:
                            self.get_logger().warn(f'    ❌ FAIL: mean error >= 0.3m')

        self.get_logger().info('=' * 60)


def main(args=None):
    rclpy.init(args=args)
    node = PredictionVerifier()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
