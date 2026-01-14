#!/bin/bash
# Quick test script for MPPI RiskAwareCritic fix

set -e

echo "=== Testing MPPI RiskAwareCritic Configuration Fix ==="
echo ""

cd /home/user/ackermann

# Step 1: Rebuild
echo "Step 1: Rebuilding saye_mppi package..."
colcon build --packages-select saye_mppi
if [ $? -ne 0 ]; then
    echo "✗ Build failed!"
    exit 1
fi
echo "✓ Build successful"
echo ""

# Step 2: Source workspace
echo "Step 2: Sourcing workspace..."
source install/setup.bash
echo "✓ Workspace sourced"
echo ""

# Step 3: Run diagnostics
echo "Step 3: Running diagnostics..."
./diagnose_mppi.sh
echo ""

# Step 4: Instructions for testing
echo "=== Manual Testing Required ==="
echo ""
echo "The configuration has been fixed. To test the full system:"
echo ""
echo "Terminal 1 - Launch Gazebo simulation:"
echo "  cd ~/ackermann_sim"
echo "  source install/setup.bash"
echo "  ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf"
echo ""
echo "Terminal 2 - Launch MPPI experiment (wait for Gazebo to load):"
echo "  cd ~/ackermann_sim"
echo "  source install/setup.bash"
echo "  ros2 launch saye_mppi mppi_experiment.launch.py"
echo ""
echo "Expected output:"
echo "  ✓ No lifecycle manager errors"
echo "  ✓ Controller server activates successfully"
echo "  ✓ Log message: 'RiskAwareCritic initialized (Phase 1 - Framework Only)'"
echo ""
