#!/usr/bin/env bash
set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
WORKSPACE=$(cd "${SCRIPT_DIR}/../../.." && pwd)
LOG_DIR="${WORKSPACE}/ros_logs"
mkdir -p "${LOG_DIR}"
export ROS_LOG_DIR="${LOG_DIR}"

source "${WORKSPACE}/install/setup.bash"

ros2 launch saye_bringup_my navigation_bringup.launch.py
