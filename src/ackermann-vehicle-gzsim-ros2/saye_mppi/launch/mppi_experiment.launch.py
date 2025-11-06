import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node


def generate_launch_description():
    pkg_saye_bringup = get_package_share_directory('saye_bringup_my')
    pkg_saye_mppi = get_package_share_directory('saye_mppi')

    nav2_params = os.path.join(pkg_saye_mppi, 'config', 'mppi_nav2_params.yaml')
    path_params = os.path.join(pkg_saye_mppi, 'config', 'path_commander.yaml')
    obstacle_params = os.path.join(pkg_saye_mppi, 'config', 'moving_obstacles.yaml')

    # saye_sim = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         os.path.join(pkg_saye_bringup, 'launch', 'saye_spawn.launch.py'))
    # )
    # 注释原因: 手动启动仿真以便指定世界文件

    static_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='map_to_odom',
        arguments=['0', '0', '0', '0', '0', '0', 'map', 'odom'],
        output='screen',
    )

    controller_server = Node(
        package='nav2_controller',
        executable='controller_server',
        name='controller_server',
        output='screen',
        parameters=[nav2_params],
    )

    behavior_server = Node(
        package='nav2_behaviors',
        executable='behavior_server',
        name='behavior_server',
        output='screen',
        parameters=[nav2_params],
    )

    velocity_smoother = Node(
        package='nav2_velocity_smoother',
        executable='velocity_smoother',
        name='velocity_smoother',
        output='screen',
        parameters=[nav2_params],
    )

    collision_monitor = Node(
        package='nav2_collision_monitor',
        executable='collision_monitor',
        name='collision_monitor',
        output='screen',
        parameters=[nav2_params],
    )

    lifecycle_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_navigation',
        output='screen',
        parameters=[nav2_params],
    )

    # moving_obstacles = Node(
    #     package='saye_mppi',
    #     executable='moving_obstacles',
    #     name='moving_obstacles',
    #     output='screen',
    #     parameters=[obstacle_params],
    # )
    # 注释原因: saye_moving_obstacles.sdf 世界文件已包含行人，不需要动态生成

    straight_path_commander = Node(
        package='saye_mppi',
        executable='straight_path_commander',
        name='straight_path_commander',
        output='screen',
        parameters=[path_params],
    )

    ld = LaunchDescription()
    # ld.add_action(saye_sim)  # 已注释，手动启动仿真
    ld.add_action(static_tf)
    ld.add_action(controller_server)
    ld.add_action(behavior_server)
    ld.add_action(velocity_smoother)
    ld.add_action(collision_monitor)
    ld.add_action(lifecycle_manager)

    # Allow Gazebo and Nav2 to finish bringing up before adding experiment nodes.
    # ld.add_action(TimerAction(period=5.0, actions=[moving_obstacles]))  # 已注释
    ld.add_action(TimerAction(period=5.0, actions=[straight_path_commander]))

    return ld
