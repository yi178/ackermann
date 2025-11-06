#!/usr/bin/env bash
set -eo pipefail

WS_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
LOG_DIR="${WS_DIR}/ros_logs"
mkdir -p "${LOG_DIR}"
export ROS_LOG_DIR="${LOG_DIR}"

source "${WS_DIR}/install/setup.bash"

ros2 launch saye_bringup_my navigation_bringup.launch.py
