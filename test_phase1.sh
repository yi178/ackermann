#!/bin/bash

echo "=========================================="
echo "  Phase 1 验证测试"
echo "=========================================="
echo ""

cd ~/ackermann_sim
source install/setup.bash

echo "1. 检查包安装..."
if ros2 pkg list | grep -q saye_rapa_mppi; then
    echo "   ✓ saye_rapa_mppi 包已安装"
else
    echo "   ✗ 包未找到"
    exit 1
fi

echo ""
echo "2. 检查库文件..."
if [ -f "install/saye_rapa_mppi/lib/libsaye_rapa_mppi.so" ]; then
    echo "   ✓ 共享库存在"
else
    echo "   ✗ 共享库未找到"
    exit 1
fi

echo ""
echo "3. 检查配置文件..."
if [ -f "install/saye_rapa_mppi/share/saye_rapa_mppi/config/rapa_mppi_params.yaml" ]; then
    echo "   ✓ 参数配置文件存在"
else
    echo "   ✗ 配置文件未找到"
    exit 1
fi

echo ""
echo "4. 检查插件文件..."
if [ -f "install/saye_rapa_mppi/share/saye_rapa_mppi/plugins.xml" ]; then
    echo "   ✓ plugins.xml 存在"
    cat install/saye_rapa_mppi/share/saye_rapa_mppi/plugins.xml | grep -q "mppi::critics::CriticFunction"
    if [ $? -eq 0 ]; then
        echo "   ✓ 插件base_class_type正确"
    else
        echo "   ✗ 插件base_class_type错误"
        exit 1
    fi
else
    echo "   ✗ plugins.xml未找到"
    exit 1
fi

echo ""
echo "=========================================="
echo "  ✓ Phase 1 验证通过！"
echo "=========================================="
echo ""
echo "现在可以运行完整测试："
echo ""
echo "终端1 - 启动仿真:"
echo "  ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_moving_obstacles.sdf"
echo ""
echo "终端2 - 启动预测器:"
echo "  ros2 launch saye_prediction ground_truth_prediction.launch.py"
echo ""
echo "终端3 - 启动MPPI (带RiskAwareCritic):"
echo "  ros2 launch saye_mppi mppi_experiment.launch.py"
echo ""
echo "终端4 - 监控日志:"
echo "  ros2 topic echo /rosout | grep RiskAwareCritic"
echo ""
