"""
Actor Prediction Demo Launch File
一键启动Actor仿真 + 预测 + RViz可视化

功能：
1. 启动Gazebo仿真(Actor world)
2. 桥接Actor pose topics (并自动激活Lazy模式)
3. 启动Actor预测器
4. 启动预测可视化器
5. 启动RViz

使用方法：
    ros2 launch saye_prediction actor_prediction_demo.launch.py
"""

import os
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, IncludeLaunchDescription, 
                           TimerAction, ExecuteProcess, SetEnvironmentVariable)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, TextSubstitution
from launch_ros.actions import Node


def generate_launch_description():
    pkg_bringup = get_package_share_directory('saye_bringup_my')
    pkg_description = get_package_share_directory('saye_description')
    pkg_prediction = get_package_share_directory('saye_prediction')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    # ========== 参数定义 ==========
    world_arg = DeclareLaunchArgument(
        'world',
        default_value='saye_dynamic_corridor_test.sdf',
        description='测试世界文件'
    )
    rviz_arg = DeclareLaunchArgument(
        'rviz',
        default_value='true',
        description='是否启动RViz'
    )

    world = LaunchConfiguration('world')

    # ========== 设置Gazebo资源路径 ==========
    home_gz_models = str(Path.home() / '.gz' / 'models')
    current_path = os.environ.get('GZ_SIM_RESOURCE_PATH', '')
    if home_gz_models not in current_path:
        new_path = f"{current_path}:{home_gz_models}" if current_path else home_gz_models
        set_gz_resource_path = SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', new_path)
    else:
        set_gz_resource_path = SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', current_path)

    # ========== 读取车辆模型 ==========
    robot_sdf = os.path.join(pkg_description, 'models', 'saye', 'model_2d_lidar.sdf')
    with open(robot_sdf, 'r') as infp:
        robot_desc = infp.read()

    # ========== 1. 启动Gazebo仿真 ==========
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={
            'gz_args': [
                TextSubstitution(text='-r '),
                PathJoinSubstitution([pkg_description, 'worlds', world])
            ]
        }.items()
    )

    # ========== 2. Robot State Publisher ==========
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc},
        ]
    )

    # ========== 3. 核心传感器桥接（使用Actor专用配置，不包含world pose info） ==========
    bridge_core = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_bringup, 'config', 'ros_gz_bridge_actor.yaml'),
        }],
        output='screen'
    )

    # ========== 4. Actor pose 桥接 ==========
    actor_topics = [
        '/actor_pose/walker_center@geometry_msgs/msg/Pose[gz.msgs.Pose',
        '/actor_pose/walker_east@geometry_msgs/msg/Pose[gz.msgs.Pose',
    ]
    bridge_actor = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=actor_topics,
        output='screen'
    )

    # ========== 5. 激活Actor桥接（通过订阅） ==========
    # 这个节点会订阅actor topics来激活ros_gz_bridge的Lazy模式
    bridge_activator = ExecuteProcess(
        cmd=[
            'bash', '-c',
            'source $AMENT_PREFIX_PATH/setup.bash && ' +
            'ros2 topic echo /actor_pose/walker_center > /dev/null & ' +
            'ros2 topic echo /actor_pose/walker_east > /dev/null &'
        ],
        shell=True,
        output='screen'
    )

    # ========== 6. 生成车辆 ==========
    spawn_vehicle = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=[
            '-file', robot_sdf,
            '-name', 'saye',
            '-allow_renaming', 'true',
            '-z', '0.35'
        ]
    )
    delayed_spawn = TimerAction(period=3.0, actions=[spawn_vehicle])

    # ========== 7. Actor预测器 ==========
    actor_predictor = Node(
        package='saye_prediction',
        executable='actor_ground_truth_predictor',
        name='actor_ground_truth_predictor',
        output='screen',
        parameters=[{
            'prediction_horizon': 2.8,
            'prediction_dt': 0.05,
            'actor_names': ['walker_center', 'walker_east'],
            'publish_rate': 10.0,
        }]
    )
    delayed_predictor = TimerAction(period=5.0, actions=[actor_predictor])

    # ========== 8. 预测可视化器 ==========
    prediction_viz = Node(
        package='saye_prediction',
        executable='prediction_visualizer',
        name='prediction_visualizer',
        output='screen',
        parameters=[{
            'marker_lifetime': 0.2,
            'show_ellipse_interval': 5,
        }]
    )
    delayed_viz = TimerAction(period=5.5, actions=[prediction_viz])

    # ========== 9. RViz ==========
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', os.path.join(pkg_bringup, 'rviz', 'saye.rviz')],
        condition=IfCondition(LaunchConfiguration('rviz')),
        output='screen'
    )
    delayed_rviz = TimerAction(period=6.0, actions=[rviz])

    # ========== 组装Launch Description ==========
    return LaunchDescription([
        world_arg,
        rviz_arg,
        set_gz_resource_path,
        gz_sim,
        robot_state_publisher,
        bridge_core,
        bridge_actor,
        delayed_spawn,
        TimerAction(period=4.0, actions=[bridge_activator]),  # 在车辆生成后激活桥接
        delayed_predictor,
        delayed_viz,
        delayed_rviz,
    ])
