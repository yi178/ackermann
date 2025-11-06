"""
Ground Truth Prediction Launch File
真值预测启动文件

启动:
  1. ground_truth_predictor - 预测节点
  2. prediction_visualizer - 可视化节点
  3. (可选) prediction_verifier - 验证节点

Usage:
    ros2 launch saye_prediction ground_truth_prediction.launch.py
    ros2 launch saye_prediction ground_truth_prediction.launch.py config:=scenario1_actors.yaml
    ros2 launch saye_prediction ground_truth_prediction.launch.py enable_verifier:=true
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch.conditions import IfCondition
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # ========== 参数 ==========
    config_arg = DeclareLaunchArgument(
        'config',
        default_value='ground_truth_predictor.yaml',
        description='Configuration file name'
    )

    enable_visualizer_arg = DeclareLaunchArgument(
        'enable_visualizer',
        default_value='true',
        description='Enable prediction visualizer'
    )

    enable_verifier_arg = DeclareLaunchArgument(
        'enable_verifier',
        default_value='false',
        description='Enable prediction verifier'
    )

    # ========== 配置文件路径 ==========
    config_file = PathJoinSubstitution([
        FindPackageShare('saye_prediction'),
        'config',
        LaunchConfiguration('config')
    ])

    # ========== 节点 ==========

    # 1. 预测器
    predictor_node = Node(
        package='saye_prediction',
        executable='ground_truth_predictor',
        name='ground_truth_predictor',
        output='screen',
        parameters=[config_file],
        remappings=[
            # 可以在这里重映射话题
        ]
    )

    # 2. 可视化器
    visualizer_node = Node(
        package='saye_prediction',
        executable='prediction_visualizer',
        name='prediction_visualizer',
        output='screen',
        condition=IfCondition(LaunchConfiguration('enable_visualizer')),
        parameters=[{
            'visualization_step': 5,
            'uncertainty_time': 1.4,
            'uncertainty_sigma': 3.0
        }]
    )

    # 3. 验证器
    verifier_node = Node(
        package='saye_prediction',
        executable='prediction_verifier',
        name='prediction_verifier',
        output='screen',
        condition=IfCondition(LaunchConfiguration('enable_verifier')),
        parameters=[config_file]
    )

    return LaunchDescription([
        config_arg,
        enable_visualizer_arg,
        enable_verifier_arg,
        predictor_node,
        visualizer_node,
        verifier_node
    ])
