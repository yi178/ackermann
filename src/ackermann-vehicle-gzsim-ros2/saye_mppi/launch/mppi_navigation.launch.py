import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    pkg_nav2_bringup = get_package_share_directory('nav2_bringup')
    pkg_saye_bringup = get_package_share_directory('saye_bringup')
    pkg_saye_mppi = get_package_share_directory('saye_mppi')

    use_sim_time = LaunchConfiguration('use_sim_time', default='True')
    autostart = LaunchConfiguration('autostart', default='True')

    nav2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_nav2_bringup, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
            'autostart': autostart,
            'map': os.path.join(pkg_saye_bringup, 'maps', 'map.yaml'),
            'params_file': os.path.join(pkg_saye_mppi, 'config', 'mppi_nav2_params.yaml'),
            'use_respawn': 'False',
            'use_composition': 'True',
        }.items(),
    )

    return LaunchDescription([
        nav2_launch,
    ])
