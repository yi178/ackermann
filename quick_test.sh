#!/bin/bash
# 快速测试脚本 - 验证行人交替穿越场景

set -e

cd ~/ackermann_sim
source install/setup.bash

echo "========================================"
echo "  行人交替穿越场景 - 快速验证"
echo "========================================"
echo ""

# 测试1: 检查世界文件
echo "✓ 测试1: 检查世界文件配置"
if grep -q "PosePublisher" src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf; then
    echo "  ✅ PosePublisher插件已配置"
else
    echo "  ❌ PosePublisher插件缺失！"
    exit 1
fi

if grep -q "walker_center" src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf; then
    echo "  ✅ walker_center已配置"
else
    echo "  ❌ walker_center缺失！"
    exit 1
fi

if grep -q "walker_east" src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf; then
    echo "  ✅ walker_east已配置"
else
    echo "  ❌ walker_east缺失！"
    exit 1
fi

echo ""

# 测试2: 检查ros_gz_bridge配置
echo "✓ 测试2: 检查ros_gz_bridge配置"
if grep -q "/model/walker_center/pose" src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/config/ros_gz_bridge.yaml; then
    echo "  ✅ walker_center桥接已配置"
else
    echo "  ❌ walker_center桥接缺失！"
    exit 1
fi

if grep -q "/model/walker_east/pose" src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/config/ros_gz_bridge.yaml; then
    echo "  ✅ walker_east桥接已配置"
else
    echo "  ❌ walker_east桥接缺失！"
    exit 1
fi

echo ""

# 测试3: 检查预测器配置
echo "✓ 测试3: 检查预测器配置"
if [ -f "src/ackermann-vehicle-gzsim-ros2/saye_prediction/config/ground_truth_predictor.yaml" ]; then
    echo "  ✅ 预测器配置文件存在"
else
    echo "  ❌ 预测器配置文件缺失！"
    exit 1
fi

if [ -f "src/ackermann-vehicle-gzsim-ros2/saye_prediction/launch/ground_truth_prediction.launch.py" ]; then
    echo "  ✅ 预测器launch文件存在"
else
    echo "  ❌ 预测器launch文件缺失！"
    exit 1
fi

echo ""

# 测试4: 检查消息定义
echo "✓ 测试4: 检查消息定义"
if [ -f "install/saye_msgs/share/saye_msgs/msg/PedestrianPredictions.msg" ]; then
    echo "  ✅ PedestrianPredictions消息已编译"
else
    echo "  ❌ PedestrianPredictions消息未编译！"
    echo "  提示: 运行 'colcon build --packages-select saye_msgs'"
    exit 1
fi

echo ""

# 显示场景配置
echo "========================================"
echo "  场景配置摘要"
echo "========================================"
echo ""
echo "智能车位置: (0, 0, 0.35)"
echo "智能车朝向: +X方向"
echo ""
echo "行人1 (walker_center):"
echo "  位置: (8, -3, 1) 右侧"
echo "  轨迹: 右→左→右 循环"
echo "  时序: t=0-5s 向左, t=8-13s 向右"
echo ""
echo "行人2 (walker_east):"
echo "  位置: (8, 3, 1) 左侧"
echo "  轨迹: 左→右→左 循环"
echo "  时序: t=7-12s 向右, t=15-20s 向左"
echo ""
echo "交替穿越模式:"
echo "  0-5s:  walker_center 从右向左穿越"
echo "  7-12s: walker_east   从左向右穿越"
echo "  ↻ 循环"
echo ""

echo "========================================"
echo "  ✅ 所有配置检查通过！"
echo "========================================"
echo ""
echo "下一步测试:"
echo ""
echo "终端1 - 启动仿真:"
echo "  ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf"
echo ""
echo "终端2 - 启动预测器（等待5秒后）:"
echo "  ros2 launch saye_prediction ground_truth_prediction.launch.py"
echo ""
echo "终端3 - 验证数据流:"
echo "  ros2 topic hz /model/walker_center/pose"
echo "  ros2 topic hz /pedestrian_predictions"
echo "  ros2 topic echo /pedestrian_predictions --once"
echo ""
echo "可选 - RViz可视化:"
echo "  ros2 run rviz2 rviz2"
echo "  (Fixed Frame: odom, Add: /prediction_markers)"
echo ""

echo "📖 完整测试流程请查看: TEST_SCENARIOS.md"
echo ""
