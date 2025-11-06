import os
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction, SetEnvironmentVariable
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, TextSubstitution
from launch_ros.actions import Node


def generate_launch_description():
    pkg_bringup = get_package_share_directory('saye_bringup_my')
    pkg_description = get_package_share_directory('saye_description')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    robot_sdf = os.path.join(pkg_description, 'models', 'saye', 'model_2d_lidar.sdf')
    if not os.path.exists(robot_sdf):
        raise FileNotFoundError(f"SDF file not found: {robot_sdf}")
    with open(robot_sdf, 'r') as infp:
        robot_desc = infp.read()
    if not robot_desc or len(robot_desc) < 100:
        raise ValueError('Vehicle SDF appears to be empty or invalid.')

    world_arg = DeclareLaunchArgument(
        'world',
        default_value='saye_dynamic_corridor_test.sdf',
        description='测试世界（位于 saye_description/worlds），可切换到自定义 SDF'
    )
    rviz_arg = DeclareLaunchArgument(
        'rviz',
        default_value='true',
        description='是否启动 RViz'
    )
    bridge_core_arg = DeclareLaunchArgument(
        'bridge_core',
        default_value='true',
        description='是否使用配置文件桥接车辆传感器/控制话题'
    )
    bridge_actor_arg = DeclareLaunchArgument(
        'bridge_actor',
        default_value='true',
        description='是否桥接 /actor_pose/* 话题'
    )

    world = LaunchConfiguration('world')

    # 设置 GZ_SIM_RESOURCE_PATH 包含 ~/.gz/models
    home_gz_models = str(Path.home() / '.gz' / 'models')
    current_path = os.environ.get('GZ_SIM_RESOURCE_PATH', '')
    if home_gz_models not in current_path:
        new_path = f"{current_path}:{home_gz_models}" if current_path else home_gz_models
        set_gz_resource_path = SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', new_path)
    else:
        set_gz_resource_path = SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', current_path)

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

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc},
            {'publish_frequency': 30.0},
        ],
        arguments=['--ros-args', '--log-level', 'info']
    )

    bridge_core = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_bringup, 'config', 'ros_gz_bridge.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen',
        condition=IfCondition(LaunchConfiguration('bridge_core'))
    )

    actor_topics = [
        '/actor_pose/test_actor@geometry_msgs/msg/Pose[gz.msgs.Pose@50Hz',
        '/actor_pose/walker_center@geometry_msgs/msg/Pose[gz.msgs.Pose@50Hz',
        '/actor_pose/walker_east@geometry_msgs/msg/Pose[gz.msgs.Pose@50Hz',
    ]
    bridge_actor = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=actor_topics,
        parameters=[{'lazy': False}],  # 禁用Lazy模式，始终桥接数据
        output='screen',
        condition=IfCondition(LaunchConfiguration('bridge_actor'))
    )

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

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', os.path.join(pkg_bringup, 'rviz', 'saye.rviz')],
        condition=IfCondition(LaunchConfiguration('rviz'))
    )
    delayed_rviz = TimerAction(period=5.0, actions=[rviz])

    return LaunchDescription([
        world_arg,
        rviz_arg,
        bridge_core_arg,
        bridge_actor_arg,
        set_gz_resource_path,  # 先设置环境变量
        gz_sim,
        robot_state_publisher,
        bridge_core,
        bridge_actor,
        delayed_spawn,
        delayed_rviz,
    ])
