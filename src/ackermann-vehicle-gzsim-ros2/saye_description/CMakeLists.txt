cmake_minimum_required(VERSION 3.8)
project(saye_description)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(gz-plugin2 REQUIRED)
find_package(gz-sim8 REQUIRED)
find_package(gz-transport13 REQUIRED)
find_package(robot_state_publisher REQUIRED)
find_package(xacro REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(sdformat14 REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY
  models
  worlds
  hooks
  DESTINATION share/${PROJECT_NAME}/
)

# World generation from templates
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Custom command to generate dynamic corridor world with pedestrians
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/saye_dynamic_corridor_generated.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/saye_dynamic_corridor_generated.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians.yaml
  COMMENT "Generating dynamic corridor world with pedestrians from template"
  VERBATIM
)

# Scenario 0: Empty world (no pedestrians) - For SLAM mapping
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/corridor_empty.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_empty.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/corridor_empty.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_empty.yaml
  COMMENT "Generating empty corridor world (no pedestrians) for SLAM mapping"
  VERBATIM
)

# Scenario 1: 4 pedestrians crossing pattern
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario1_4ped_crossing.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_4ped_crossing.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario1_4ped_crossing.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_4ped_crossing.yaml
  COMMENT "Generating Scenario 1: 4 pedestrians crossing"
  VERBATIM
)

# Scenario 2: 8 pedestrians mixed patterns
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario2_8ped_mixed.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_8ped_mixed.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario2_8ped_mixed.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_8ped_mixed.yaml
  COMMENT "Generating Scenario 2: 8 pedestrians mixed"
  VERBATIM
)

# Scenario 3: 12 pedestrians crowded environment
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario3_12ped_crowded.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_12ped_crowded.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario3_12ped_crowded.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_12ped_crowded.yaml
  COMMENT "Generating Scenario 3: 12 pedestrians crowded"
  VERBATIM
)

# Scenario 4: 6 pedestrians static obstacles
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario4_6ped_static.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_6ped_static.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario4_6ped_static.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_6ped_static.yaml
  COMMENT "Generating Scenario 4: 6 pedestrians static"
  VERBATIM
)

# Scenario 5: 6 pedestrians random movement
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario5_6ped_random.sdf
  COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    --template ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    --config ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_6ped_random.yaml
    --output ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario5_6ped_random.sdf
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_world.py
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/templates/saye_dynamic_corridor.sdf.jinja2
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pedestrians_6ped_random.yaml
  COMMENT "Generating Scenario 5: 6 pedestrians random"
  VERBATIM
)

# Target to ensure all world generation happens during build
add_custom_target(generate_worlds ALL
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/corridor_empty.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/saye_dynamic_corridor_generated.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario1_4ped_crossing.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario2_8ped_mixed.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario3_12ped_crowded.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario4_6ped_static.sdf
    ${CMAKE_CURRENT_SOURCE_DIR}/worlds/scenario5_6ped_random.sdf
)

add_library(pedestrian_manager_system SHARED
  plugins/pedestrian_manager/src/pedestrian_manager_system.cpp
)
target_include_directories(pedestrian_manager_system
  PUBLIC
    plugins/pedestrian_manager/include
)
target_link_libraries(pedestrian_manager_system
  PUBLIC
    gz-sim8::gz-sim8
    gz-plugin2::gz-plugin2
    gz-transport13::gz-transport13
    ament_index_cpp::ament_index_cpp
    sdformat14::sdformat14
    yaml-cpp
)

install(TARGETS pedestrian_manager_system
  DESTINATION lib
)

install(DIRECTORY plugins/pedestrian_manager/config/
  DESTINATION share/${PROJECT_NAME}/config
)

add_library(actor_pose_publisher_system SHARED
  plugins/actor_pose_publisher/src/actor_pose_publisher_system.cpp
)
target_include_directories(actor_pose_publisher_system
  PUBLIC
    plugins/actor_pose_publisher/include
)
target_link_libraries(actor_pose_publisher_system
  PUBLIC
    gz-sim8::gz-sim8
    gz-plugin2::gz-plugin2
    gz-transport13::gz-transport13
)

install(TARGETS actor_pose_publisher_system
  DESTINATION lib
)

add_library(test_actor_driver_system SHARED
  plugins/actor_pose_publisher/src/test_actor_driver_system.cpp
)
target_include_directories(test_actor_driver_system
  PUBLIC
    plugins/actor_pose_publisher/include
)
target_link_libraries(test_actor_driver_system
  PUBLIC
    gz-sim8::gz-sim8
    gz-plugin2::gz-plugin2
)

install(TARGETS test_actor_driver_system
  DESTINATION lib
)

ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/ros_gz_example_description.sh.in")
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/ros_gz_example_description.dsv.in")

ament_package()
