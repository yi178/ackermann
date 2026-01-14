#!/bin/bash
# Diagnostic script for MPPI RiskAwareCritic plugin

echo "=== MPPI Plugin Diagnostics ==="
echo ""

cd /home/user/ackermann

# Source ROS 2
if [ -f "/opt/ros/jazzy/setup.bash" ]; then
    source /opt/ros/jazzy/setup.bash
    echo "✓ ROS 2 Jazzy sourced"
else
    echo "✗ ROS 2 Jazzy not found"
    exit 1
fi

# Source workspace
if [ -f "install/setup.bash" ]; then
    source install/setup.bash
    echo "✓ Workspace sourced"
else
    echo "✗ Workspace install/setup.bash not found"
    exit 1
fi

echo ""
echo "=== Checking Plugin Library ==="
if [ -f "install/saye_rapa_mppi/lib/libsaye_rapa_mppi.so" ]; then
    echo "✓ libsaye_rapa_mppi.so exists"
    ldd install/saye_rapa_mppi/lib/libsaye_rapa_mppi.so | grep -E "(not found|saye_msgs)" || echo "  All dependencies resolved"
else
    echo "✗ libsaye_rapa_mppi.so not found"
fi

echo ""
echo "=== Checking Plugin Registration ==="
if [ -f "install/saye_rapa_mppi/share/saye_rapa_mppi/plugins.xml" ]; then
    echo "✓ plugins.xml exists"
    echo "  Content:"
    cat install/saye_rapa_mppi/share/saye_rapa_mppi/plugins.xml | head -5
else
    echo "✗ plugins.xml not found"
fi

echo ""
echo "=== Checking Config File ==="
if [ -f "install/saye_mppi/share/saye_mppi/config/mppi_nav2_params.yaml" ]; then
    echo "✓ mppi_nav2_params.yaml exists"
    echo "  Checking RiskAwareCritic configuration:"
    grep -A 5 "RiskAwareCritic" install/saye_mppi/share/saye_mppi/config/mppi_nav2_params.yaml | head -10
else
    echo "✗ mppi_nav2_params.yaml not found"
fi

echo ""
echo "=== Checking Message Definitions ==="
ros2 interface show saye_msgs/msg/PedestrianPredictions > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ saye_msgs/msg/PedestrianPredictions is available"
else
    echo "✗ saye_msgs/msg/PedestrianPredictions not found"
fi

echo ""
echo "=== Checking Plugin Discoverability ==="
ros2 plugin list nav2_mppi_controller 2>/dev/null | grep -i "RiskAwareCritic"
if [ $? -eq 0 ]; then
    echo "✓ RiskAwareCritic plugin is discoverable"
else
    echo "✗ RiskAwareCritic plugin NOT discoverable"
    echo "  Attempting to see what plugins are available:"
    ros2 plugin list nav2_mppi_controller 2>/dev/null | grep -i "critic" | head -5
fi

echo ""
echo "=== Diagnostics Complete ==="
