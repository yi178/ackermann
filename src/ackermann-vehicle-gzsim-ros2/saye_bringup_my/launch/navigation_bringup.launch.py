import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    pkg_nav2_dir = get_package_share_directory('nav2_bringup')
    pkg_saye_bringup = get_package_share_directory('saye_bringup_my')

    use_sim_time = LaunchConfiguration('use_sim_time', default='True')
    autostart = LaunchConfiguration('autostart', default='True')

    default_map = os.path.join(pkg_saye_bringup, 'maps', 'map.yaml')
    default_params = os.path.join(pkg_saye_bringup, 'config', 'nav2_params.yaml')

    nav2_launch_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_nav2_dir, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
            'autostart': autostart,
            'map': default_map,
            'params_file': default_params,
            'use_localization': 'False',
            'package_path': pkg_saye_bringup, 
        }.items()
    )

    rviz_launch_cmd = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        arguments=[
            '-d' + os.path.join(
                pkg_saye_bringup,
                'rviz',
                'navigation.rviz'
            )
        ]
    )
    
    map_server_node = Node(
        package='nav2_map_server',
        executable='map_server',
        name='map_server',
        output='screen',
        parameters=[{
            'yaml_filename': os.path.join(pkg_saye_bringup, 'maps', 'map.yaml'),
            'use_sim_time': True
        }],
    )

    map_lifecycle_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_map',
        output='screen',
        parameters=[{
            'use_sim_time': True,
            'autostart': True,
            'node_names': ['map_server']
        }],
    )

    ground_truth_pose_node = Node(
        package='saye_bringup_my',
        executable='ground_truth_pose',
        name='ground_truth_pose',
        output='screen',
        parameters=[{
            'target_frame': 'saye/base_link',
            'source_frame': 'dynamic_corridor_world',
            'odom_frame': 'odom',
            'map_frame': 'map',
            'tf_topic': '/tf'
        }],
    )



    ld = LaunchDescription()

    ld.add_action(nav2_launch_cmd)
    ld.add_action(rviz_launch_cmd)
    ld.add_action(map_server_node)
    ld.add_action(map_lifecycle_manager)
    ld.add_action(ground_truth_pose_node)

    return ld
