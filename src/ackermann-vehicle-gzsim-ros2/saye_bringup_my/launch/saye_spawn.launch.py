import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node


def generate_launch_description():
    pkg_project_bringup = get_package_share_directory('saye_bringup_my')
    pkg_project_description = get_package_share_directory('saye_description')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    sdf_file = os.path.join(pkg_project_description, 'models', 'saye', 'model_2d_lidar.sdf')
    if not os.path.exists(sdf_file):
        raise FileNotFoundError(f"SDF file not found: {sdf_file}")

    with open(sdf_file, 'r') as infp:
        robot_desc = infp.read()

    if not robot_desc or len(robot_desc) < 100:
        raise ValueError('SDF file appears to be empty or invalid')

    world_file = LaunchConfiguration('world')

    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={'gz_args': PathJoinSubstitution([
            pkg_project_description,
            'worlds',
            world_file
        ])}.items(),
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

    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_bringup, 'config', 'ros_gz_bridge.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    gz_spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=[
            '-file', os.path.join(pkg_project_description, 'models', 'saye', 'model_2d_lidar.sdf'),
            '-name', 'saye',
            '-allow_renaming', 'true',
            '-z', '0.35'
        ]
    )

    delayed_spawn = TimerAction(
        period=3.0,
        actions=[gz_spawn_entity]
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', os.path.join(pkg_project_bringup, 'rviz', 'saye.rviz')],
        condition=IfCondition(LaunchConfiguration('rviz'))
    )

    delayed_rviz = TimerAction(
        period=5.0,
        actions=[rviz]
    )

    declare_world = DeclareLaunchArgument(
        'world',
        default_value='saye_dynamic_corridor.sdf',
        description='World SDF file inside saye_description/worlds'
    )

    declare_rviz = DeclareLaunchArgument(
        'rviz',
        default_value='true',
        description='Open RViz.'
    )

    return LaunchDescription([
        declare_world,
        declare_rviz,
        gz_sim,
        robot_state_publisher,
        bridge,
        delayed_spawn,
        delayed_rviz,
    ])
