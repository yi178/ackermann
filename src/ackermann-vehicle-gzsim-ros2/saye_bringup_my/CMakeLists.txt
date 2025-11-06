cmake_minimum_required(VERSION 3.5)

project(saye_bringup_my)

find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(saye_description REQUIRED)

install(
  DIRECTORY
    launch
    maps
    rviz
    config
  DESTINATION share/${PROJECT_NAME}/
)

install(
  PROGRAMS
    scripts/ground_truth_pose
    scripts/start_navigation.sh
  DESTINATION lib/${PROJECT_NAME}
)
  
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
