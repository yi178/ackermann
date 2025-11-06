cmake_minimum_required(VERSION 3.5)
project(saye_rapa_mppi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_mppi_controller REQUIRED)
find_package(nav2_core REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(saye_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(pluginlib REQUIRED)

include_directories(include)

# 库
add_library(${PROJECT_NAME} SHARED
  src/risk_aware_critic.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  nav2_mppi_controller
  nav2_core
  nav2_costmap_2d
  saye_msgs
  geometry_msgs
  nav_msgs
  tf2
  tf2_geometry_msgs
  pluginlib
)

# 安装
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/
)

install(FILES plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

# 导出
pluginlib_export_plugin_description_file(nav2_mppi_controller plugins.xml)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(
  rclcpp
  nav2_mppi_controller
  nav2_core
  saye_msgs
)

ament_package()
