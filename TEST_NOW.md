# 立即测试 GMM预测器 - 行人交替穿越场景

## 最新更新 (2025-10-30)
✅ 修复了`subscriptions`变量名冲突
✅ 添加了PosePublisher插件到世界文件
✅ 添加了actor pose话题的ros_gz_bridge配置
✅ **修改了walker位置为交替穿越模式**:
   - walker_center: (8, -3) → 右侧起点，向左穿越
   - walker_east: (8, 3) → 左侧起点，延迟7秒后向右穿越
   - 两者在车辆前方8米处交替穿越车辆路径
✅ 创建了完整测试文档（TEST_SCENARIOS.md, SCENARIO_DIAGRAM.md）
✅ 重新编译了所有包

---

## 测试步骤（3个终端）

### 准备工作
```bash
cd ~/ackermann_sim
source install/setup.bash
```

---

### Terminal 1: 启动Gazebo仿真（默认场景）

```bash
# 清屏
clear

# 启动仿真（包含walker_center和walker_east两个行人）
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf
```

**预期结果**：
- Gazebo窗口打开
- 看到车辆和2个行人
- 行人开始移动

---

### Terminal 2: 启动预测器

**等待Gazebo完全启动后**（约5-10秒），在新终端执行：

```bash
cd ~/ackermann_sim
source install/setup.bash
clear

# 启动预测器（包含可视化）
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

**预期输出**：
```
[INFO] [ground_truth_predictor]: Prediction configuration:
[INFO] [ground_truth_predictor]:   Horizon: 2.8s
[INFO] [ground_truth_predictor]:   Time step: 0.05s
[INFO] [ground_truth_predictor]:   Number of steps: 56
[INFO] [ground_truth_predictor]: Subscribed to: /model/walker_center/pose
[INFO] [ground_truth_predictor]: Subscribed to: /model/walker_east/pose
[INFO] [ground_truth_predictor]: Ground Truth Predictor initialized
[INFO] [ground_truth_predictor]: Tracking 2 actors: ['walker_center', 'walker_east']
[INFO] [ground_truth_predictor]: Publishing at 10.0 Hz
[INFO] [prediction_visualizer]: Prediction Visualizer initialized
```

❌ **如果看到错误**：
- `No data received for walker_*` → Gazebo还没启动完成，等几秒
- `Stale data` → actor可能还没开始移动

---

### Terminal 3: 验证输出

```bash
cd ~/ackermann_sim
source install/setup.bash

# 检查话题列表
ros2 topic list | grep -E "(prediction|walker)"

# 应该看到：
# /model/walker_center/pose
# /model/walker_east/pose
# /pedestrian_predictions
# /prediction_markers

# 查看预测数据（Ctrl+C停止）
ros2 topic echo /pedestrian_predictions --once

# 检查发布频率
ros2 topic hz /pedestrian_predictions

# 应该显示：average rate: 10.xxx
```

---

### Terminal 4（可选）: RViz可视化

```bash
ros2 run rviz2 rviz2
```

**RViz配置步骤**：
1. **Fixed Frame**: 改为 `odom`
2. **Add** → **MarkerArray**:
   - Topic: `/prediction_markers`
   - 应该看到：
     - 🔴 红色球体 = 当前位置
     - 🔵 蓝色线条 = 预测轨迹
     - 🟢 绿色椭圆 = 不确定性
     - 🟡 黄色箭头 = 速度方向
3. **Add** → **TF**: 查看坐标系

---

## 快速验证检查表

```bash
# 1. 消息是否发布？
ros2 topic hz /pedestrian_predictions
# ✅ 目标: ~10 Hz

# 2. 数据是否正确？
ros2 topic echo /pedestrian_predictions --once
# ✅ 检查:
#    - predictions列表有2个元素
#    - 每个prediction有56步预测
#    - current_speed > 0

# 3. 可视化是否显示？
ros2 topic hz /prediction_markers
# ✅ 目标: ~10 Hz
```

---

## 故障排除

### 问题1: `No data received for walker_*`
**原因**: Gazebo还没完全启动
**解决**: 等待10-15秒让Gazebo完全加载

### 问题2: `/model/walker_*/pose` 话题不存在
```bash
# 检查Gazebo话题
gz topic -l | grep walker

# 检查ROS话题
ros2 topic list | grep walker
```

**如果Gazebo有话题但ROS没有**:
→ ros_gz_bridge没启动或配置错误
→ 检查launch文件是否包含bridge

### 问题3: 预测数据全是0
**原因**: 历史数据不足
**解决**: 等待5秒让系统收集历史数据

### 问题4: RViz不显示marker
1. 确认Fixed Frame = `odom`
2. 确认MarkerArray的Namespaces全部勾选
3. 检查话题：`ros2 topic echo /prediction_markers --once`

---

## 成功标准

✅ **基础功能**:
- [ ] 预测器启动无错误
- [ ] `/pedestrian_predictions`话题以10Hz发布
- [ ] 预测数据包含2个行人
- [ ] 每个行人有56步预测

✅ **可视化**:
- [ ] RViz显示红色球体（当前位置）
- [ ] RViz显示蓝色轨迹线（预测）
- [ ] RViz显示绿色椭圆（不确定性）

✅ **数据质量**:
- [ ] current_speed合理（0.5-2.0 m/s）
- [ ] 预测轨迹平滑连续
- [ ] 不确定性随时间增长

---

## 下一步测试

成功后，测试更多场景：

```bash
# 场景1: 4个行人
ros2 launch saye_bringup_my saye_spawn.launch.py \
  world:=scenario1_4ped_crossing.sdf

ros2 launch saye_prediction ground_truth_prediction.launch.py \
  config:=scenario1_actors.yaml \
  enable_verifier:=true
```

---

**问题反馈**: 如果遇到问题，运行：
```bash
ros2 topic list
ros2 topic echo /pedestrian_predictions --once
ros2 node info /ground_truth_predictor
```

将输出发给我分析。
