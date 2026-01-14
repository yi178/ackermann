#!/bin/bash
# Test script to launch MPPI experiment with verbose output

cd /home/user/ackermann
source /opt/ros/jazzy/setup.bash 2>/dev/null || echo "ROS 2 Jazzy not found at default location"
source install/setup.bash

# Set verbose logging
export RCUTILS_CONSOLE_OUTPUT_FORMAT="[{severity}] [{name}]: {message}"
export RCUTILS_LOGGING_BUFFERED_STREAM=1

echo "=== Checking plugin registration ==="
ros2 plugin list | grep -i critic || echo "Could not list plugins"

echo ""
echo "=== Launching MPPI Experiment ==="
ros2 launch saye_mppi mppi_experiment.launch.py
