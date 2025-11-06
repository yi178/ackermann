import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, TimerAction
from launch.actions import IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessStart
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node


def generate_launch_description():
    # Setup project paths
    pkg_project_bringup = get_package_share_directory('saye_bringup')
    pkg_project_localization = get_package_share_directory('saye_localization')
    pkg_project_description = get_package_share_directory('saye_description')
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')

    # Load the SDF file from "description" package
    sdf_file = os.path.join(pkg_project_description, 'models', 'saye', 'model.sdf')
    
    # ȷ���ļ�����
    if not os.path.exists(sdf_file):
        raise FileNotFoundError(f"SDF file not found: {sdf_file}")
    
    with open(sdf_file, 'r') as infp:
        robot_desc = infp.read()
    
    # ��֤����
    if not robot_desc or len(robot_desc) < 100:
        raise ValueError("SDF file appears to be empty or invalid")

    # Setup to launch the simulator and Gazebo world
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={'gz_args': PathJoinSubstitution([
            pkg_project_description,
            'worlds',
            LaunchConfiguration('world')
        ])}.items(),
    )

    # Takes the description and joint angles as inputs and publishes the 3D poses of the robot links
    # �ؼ��޸ģ����Ӹ�������Ͳ���
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc},
            {'publish_frequency': 30.0},  # ���ӷ���Ƶ��
        ],
        arguments=['--ros-args', '--log-level', 'info']  # ������־����
    )

    # Bridge ROS topics and Gazebo messages for establishing communication
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_project_bringup, 'config', 'ros_gz_bridge.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )

    # �ؼ��޸ģ��ӳ����� gz_spawn_entity��ȷ�� robot_state_publisher ������
    gz_spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=[
            '-topic', '/robot_description',
            '-name', 'saye',
            '-allow_renaming', 'true',
            '-z', '0.35'
        ]
    )

    # ʹ�� TimerAction �ӳ����� spawn_entity
    delayed_spawn = TimerAction(
        period=3.0,  # �ӳ� 3 �룬�ȴ� robot_state_publisher ��ȫ����
        actions=[gz_spawn_entity]
    )

    # Visualize in RViz
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', os.path.join(pkg_project_bringup, 'rviz', 'saye.rviz')],
        condition=IfCondition(LaunchConfiguration('rviz'))
    )

    # �����ӳ����� RViz��ȷ��ģ���Ѽ���
    delayed_rviz = TimerAction(
        period=5.0,
        actions=[rviz]
    )

    return LaunchDescription([
        DeclareLaunchArgument('world', default_value='saye_world.sdf',
                              description='Gazebo world file name (without path)'),
        DeclareLaunchArgument('rviz', default_value='true',
                              description='Open RViz.'),
        gz_sim,
        robot_state_publisher,  # ������ robot_state_publisher
        bridge,                 # ������ bridge
        delayed_spawn,          # �ӳ����� spawn
        delayed_rviz,           # �ӳ����� rviz
    ])