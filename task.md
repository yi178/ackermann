# Ackermann Sim Customization Summary

This document captures the main changes introduced during the recent tuning work so that future iterations (e.g., MPPI improvements, dynamic-scene testing) have a single reference point.

## New Packages & Utilities

- **`saye_mppi/`** (CMake + setuptools):
  - Python scripts `straight_path_commander` (FollowPath action client sending straight `nav_msgs/Path`) and `moving_obstacles` (spawns and animates Gazebo entities).
  - Configuration under `config/` for MPPI-only Nav2 parameters, 2D path goal, and obstacle motion defaults.
  - Launch file `mppi_experiment.launch.py` wiring Gazebo + reduced Nav2 stack + experimental nodes.
  - Scripts installed to `lib/saye_mppi/` via `setup.py`; package depends on `ros_gz_interfaces`, `nav2_msgs`, `tf2_ros`, etc.

## Launch / Parameter Tweaks

- `saye_bringup/launch/saye_spawn.launch.py`: now loads robot via `model.sdf` or the 2D variant and allows `-Y` rotation tweaks.
- `saye_bringup/config/nav2_params.yaml` and `saye_mppi/config/path_commander.yaml`: yaw/goal aligned with corridor axis (`-1.5708`), MPPI controller tuned for Ackermann, and follow-path goal exposed for quick edits.
- `saye_mppi/saye_mppi/path_commander.py`: switched from TF lookup to `/odom` subscription so it can operate without AMCL; added frame mismatch warning suppression.

## 2D Lidar Variant & Alternate Bringup

- Added `saye_description/models/saye/model_2d_lidar.sdf` (single-layer GPU lidar) and made `saye_bringup_my` (copy of bringup) load it by default.
- `saye_bringup_my` has its own install tree, re-pointed `get_package_share_directory(...)` calls, and compiles independently.

## Long Corridor World

- New SDF `saye_description/worlds/saye_long_corridor.sdf`:
  - Corridor rotated so the long axis is the robot’s forward direction, with doorways at both ends.
  - Added lightweight features (e.g., pillars/boxes) to provide unique mapping cues without blocking the path.
- `saye_bringup_my` spawn launch declares a `world` launch arg (defaulting to `saye_long_corridor.sdf`) so world files are swappable.

## Dynamic Scene Support

- `moving_obstacles` now retries `/world/<name>/spawn_entity` until Gazebo responds and expects `my_world` by default.
- Scripts demonstrate spawning additional diff-drive robots via `ros_gz_sim create` and commanding them through `/model/<name>/cmd_vel`.

## Outstanding Follow-ups

- Long corridor world still needs minor clean-ups (initial XML mismatch fixed, but further aesthetic gates/geometry may still cause parsing issues if modified manually).
- Map generation workflow: use `ros2 launch saye_bringup_my slam.launch.py`, drive the corridor once, then `ros2 run nav2_map_server map_saver_cli -f <path>`; remember to update Nav2 map references before navigation.

With these adjustments you can focus on MPPI behaviour in constrained, dynamic scenarios while keeping the original project intact for comparison.
