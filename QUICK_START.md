# 🚀 快速启动 - 行人预测测试

## 一键启动测试（推荐）

### 1️⃣ 配置检查
```bash
cd ~/ackermann_sim
./quick_test.sh
```

### 2️⃣ 三终端启动

#### 终端1️⃣: Gazebo仿真
```bash
cd ~/ackermann_sim && source install/setup.bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf
```

#### 终端2️⃣: 预测器（等待5秒后启动）
```bash
cd ~/ackermann_sim && source install/setup.bash
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

#### 终端3️⃣: 验证数据
```bash
cd ~/ackermann_sim && source install/setup.bash

# 查看预测
ros2 topic echo /pedestrian_predictions --once

# 检查频率
ros2 topic hz /pedestrian_predictions
```

### 3️⃣ 可视化（可选）
```bash
ros2 run rviz2 rviz2
# Fixed Frame: odom
# Add → MarkerArray → /prediction_markers
```

---

## 预期观察

### Gazebo窗口
- ⏱️ **0-5秒**: walker_center从右→左穿越
- ⏱️ **7-12秒**: walker_east从左→右穿越
- 🔄 循环重复

### RViz窗口
- 🔴 红色球体：当前位置
- 🔵 蓝色线条：预测轨迹（2.8秒）
- 🟢 绿色椭圆：不确定性（1.4秒处）
- 🟡 黄色箭头：速度方向

### 终端输出
```
[INFO] [ground_truth_predictor]: Tracking 2 actors
[INFO] [ground_truth_predictor]: Publishing at 10.0 Hz
```

---

## 成功标准 ✅

- [ ] Gazebo加载，2个行人交替穿越
- [ ] 预测器无错误，10Hz发布
- [ ] 预测包含2个行人，每个56步
- [ ] RViz显示所有marker类型
- [ ] 速度在1.0-1.5 m/s

---

## 快速命令参考

```bash
# 话题列表
ros2 topic list | grep -E "(walker|prediction)"

# 查看预测数据
ros2 topic echo /pedestrian_predictions --once

# 检查频率
ros2 topic hz /pedestrian_predictions

# 节点信息
ros2 node info /ground_truth_predictor

# 测试Gazebo话题
gz topic -e -t /model/walker_center/pose -n 1

# 测试ROS话题
ros2 topic echo /model/walker_center/pose --once
```

---

## 故障排除

| 问题 | 解决方案 |
|------|---------|
| Gazebo话题TIMEOUT | 重启仿真，等待10秒 |
| 预测器"No data" | 等待5-10秒让actors移动 |
| RViz无marker | Fixed Frame改为`odom` |
| 编译错误 | `colcon build && source install/setup.bash` |

---

## 📖 详细文档

- 🚀 **本文档** - 快速启动
- 📋 **READY_TO_TEST.md** - 完整启动清单
- 🧪 **TEST_SCENARIOS.md** - 4个测试场景
- 🗺️ **SCENARIO_DIAGRAM.md** - 可视化图解
- 🏗️ **SYSTEM_ARCHITECTURE.md** - 系统架构
- ⚡ **TEST_NOW.md** - 快速测试指南

---

**测试时间**: 5-10分钟
**难度**: 简单
**状态**: ✅ 就绪
