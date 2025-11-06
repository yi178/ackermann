# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an **Ackermann steering vehicle simulation** project based on https://github.com/alitekes1/ackermann-vehicle-gzsim-ros2. The primary goal is to **test and optimize the MPPI (Model Predictive Path Integral) local planner** in dynamic scenarios with pedestrians and moving obstacles.

**Environment:** Ubuntu 24.04, ROS 2 Jazzy, Gazebo Harmonic (running in WSL2 on Windows)

## Build System

```bash
# Build entire workspace
colcon build && source install/setup.bash

# Build specific package
colcon build --packages-select <package_name> && source install/setup.bash

# Build with symlink-install for faster Python iteration
colcon build --symlink-install && source install/setup.bash
```

**Required environment variables** (should be in ~/.bashrc):
```bash
export GZ_SIM_RESOURCE_PATH=$GZ_SIM_RESOURCE_PATH:/home/yiyi/ackermann_sim/src/ackermann-vehicle-gzsim-ros2/
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:/home/yiyi/ackermann_sim/src/ackermann-vehicle-gzsim-ros2/
```

## Package Architecture

The workspace contains these key ROS 2 packages:

- **saye_description**: Vehicle models (SDF), world files, and Gazebo plugins
  - `models/saye/model.sdf` - Standard vehicle with 3D LiDAR
  - `models/saye/model_2d_lidar.sdf` - Vehicle with single-layer 2D LiDAR
  - `worlds/` - Multiple world files including dynamic corridor environments
  - `plugins/pedestrian_manager/` - Gazebo plugin for dynamic pedestrian spawning

- **saye_bringup**: Original launch files and configurations
  - `saye_spawn.launch.py` - Spawns vehicle in Gazebo
  - `navigation_bringup.launch.py` - Full Nav2 stack with global + local planning
  - `slam.launch.py` - SLAM Toolbox for mapping

- **saye_bringup_my**: Modified bringup package that uses 2D LiDAR by default
  - Same structure as saye_bringup but loads `model_2d_lidar.sdf`
  - Supports world file selection via launch argument

- **saye_mppi**: Custom package for MPPI experimentation
  - `path_commander.py` - FollowPath action client that sends straight paths (bypasses global planner)
  - `moving_obstacles.py` - Spawns dynamic obstacles in Gazebo
  - `mppi_experiment.launch.py` - Minimal Nav2 setup (MPPI controller only, no AMCL/global planner)
  - `config/mppi_nav2_params.yaml` - Tuned MPPI parameters for Ackermann steering

- **saye_control**: Ackermann steering control plugins
- **saye_localization**: Localization configuration
- **saye_msgs**: Custom message definitions

## Common Commands

### Basic Simulation
```bash
# Launch vehicle in default world
ros2 launch saye_bringup saye_spawn.launch.py

# Launch with 2D LiDAR variant
ros2 launch saye_bringup_my saye_spawn.launch.py

# Launch with specific world file
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf

# Manual control via keyboard
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

### MPPI Experiments
```bash
# Launch MPPI-only navigation (no global planner, uses FollowPath action)
ros2 launch saye_mppi mppi_experiment.launch.py

# This launches:
# - Gazebo simulation with vehicle
# - Static map->odom transform (bypasses AMCL)
# - Nav2 controller_server (MPPI only)
# - straight_path_commander node (sends straight path goals)
# - moving_obstacles node (optional dynamic obstacles)
```

### Mapping
```bash
# 1. Launch simulation
ros2 launch saye_bringup_my saye_spawn.launch.py

# 2. Launch SLAM
ros2 launch saye_bringup_my slam.launch.py

# 3. Drive around manually to build map
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# 4. Save map
ros2 run nav2_map_server map_saver_cli -f /path/to/save/map
```

### Full Navigation (with AMCL and global planning)
```bash
ros2 launch saye_bringup navigation_bringup.launch.py
```

## Dynamic Pedestrian System

The project has **two approaches** for adding pedestrians:

### 1. Inline SDF Actors (Working)
- File: `saye_description/worlds/saye_dynamic_corridor.sdf`
- Pedestrians are defined directly in the world file as `<actor>` tags
- Two pedestrians: `walker_center` and `walker_east` with looping trajectories
- **This is the default approach** and works reliably

### 2. Plugin-Based System (Not Loading)
- File: `saye_description/worlds/saye_dynamic_corridor_plugin.sdf`
- Uses Gazebo plugin `libpedestrian_manager_system.so`
- Reads pedestrian config from `plugins/pedestrian_manager/config/pedestrian_actors_plugin.yaml`
- **Current issue**: Plugin approach does not spawn actors, only inline SDF works
- To use plugin world: `ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor_plugin.sdf`

**Modifying pedestrians**:
- For inline approach: Edit waypoints directly in `saye_dynamic_corridor.sdf` in the `<trajectory>` sections
- For plugin approach: Edit `pedestrian_actors_plugin.yaml` to change count, spawn positions, or trajectories
- The YAML format includes: `name`, `spawn_pose [x,y,z,roll,pitch,yaw]`, and `trajectory` with time/pose waypoints

## Key Architecture Decisions

1. **MPPI Testing Strategy**: Uses `straight_path_commander` to send FollowPath goals instead of full Nav2 navigation. This isolates MPPI controller performance from global planner and localization effects.

2. **Odometry-Based Localization**: `path_commander.py` subscribes to `/odom` instead of using TF lookups, allowing operation without AMCL.

3. **World Variants**:
   - `saye_world.sdf` - Original warehouse environment
   - `saye_long_corridor.sdf` - Long straight corridor for testing forward motion
   - `saye_dynamic_corridor.sdf` - Corridor with inline pedestrian actors
   - `saye_dynamic_corridor_plugin.sdf` - Corridor configured for plugin-based pedestrians

4. **Frame Convention**: Corridor worlds are oriented so robot's forward axis aligns with corridor length (goal yaw = -1.5708 radians).

## Configuration Files

- `saye_bringup/config/nav2_params.yaml` - Full Nav2 parameters
- `saye_mppi/config/mppi_nav2_params.yaml` - MPPI-tuned parameters for experiments
- `saye_mppi/config/path_commander.yaml` - Goal position and path parameters
- `saye_bringup/config/ros_gz_bridge.yaml` - Sensor topic bridging (cameras commented out by default except front)

## Known Issues & Workarounds

1. **Pedestrian Plugin Not Loading**: The `libpedestrian_manager_system.so` plugin compiles but actors don't spawn. Use inline SDF actors in `saye_dynamic_corridor.sdf` as the working solution.

2. **Map->Odom Transform**: For MPPI experiments without AMCL, a static transform is published. Don't use this with full navigation stack.

3. **Camera Topics**: By default only front camera is bridged to ROS 2. Uncomment lines in `ros_gz_bridge.yaml` to enable left/right/rear cameras.

## Project Files Reference

- `memory.md` - Detailed project progress and implementation notes (Chinese)
- `task.md` - Customization summary and outstanding follow-ups
- `start_navigation.sh` - Helper script for launching navigation
