# 行人交替穿越场景测试文档

## 场景描述

在 `saye_dynamic_corridor.sdf` 世界中创建了**2个行人交替穿越**智能车前方的测试场景。

### 场景配置

**智能车**：
- 起始位置：(0, 0, 0.35)
- 朝向：+X 方向（yaw=0）
- 预期路径：直线沿+X方向行驶（使用followpath发送直线全局路径）

**行人设置**：

| 名称 | 起始位置 | 轨迹 | 速度 | 周期 |
|------|---------|------|------|------|
| walker_center | (8, -3, 1) 右侧 | 右→左→右循环 | ~1.2 m/s | 14秒 |
| walker_east | (8, 3, 1) 左侧 | 左→右→左循环（延迟7秒） | ~1.2 m/s | 21秒 |

### 交替穿越时序

```
时间轴（秒）:  0    5    7    12   14   15   20   21
walker_center: →→→→ [等] ←←←←← [转]
walker_east:   [等等等] →→→→ [等] ←←←←← [转]

说明：
→ = 向左/右穿越
[等] = 等待在边缘
[转] = 转身准备下一次穿越
```

**关键特征**：
1. 两个行人在**同一X位置（8米）**交替穿越
2. walker_center先穿越（t=0-5s）
3. walker_east延迟穿越（t=7-12s）
4. 穿越宽度：6米（Y: -3 到 +3）
5. 车辆前进路径（Y≈0）被两次穿越

---

## 测试准备

### 1. 编译检查

```bash
cd ~/ackermann_sim
colcon build --packages-select saye_description saye_prediction
source install/setup.bash
```

### 2. 验证PosePublisher插件

检查世界文件是否包含pose发布插件：

```bash
grep -A 5 "PosePublisher" \
  src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf
```

**预期输出**：
```xml
<plugin filename="gz-sim-pose-publisher-system" name="gz::sim::systems::PosePublisher">
  <publish_model_pose>true</publish_model_pose>
  <update_frequency>10</update_frequency>
</plugin>
```

---

## 测试场景1：观察行人运动

**目标**：验证行人按预期交替穿越，验证Gazebo发布actor位姿。

### Terminal 1: 启动仿真

```bash
cd ~/ackermann_sim
source install/setup.bash
clear

ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf
```

**观察要点**：
- ✅ Gazebo窗口打开，加载世界
- ✅ 看到车辆在原点
- ✅ 看到2个行人，walker_center在右侧(Y=-3)，walker_east在左侧(Y=+3)
- ✅ **0-5秒**：walker_center从右向左穿越（跨过Y=0）
- ✅ **7-12秒**：walker_east从左向右穿越（跨过Y=0）
- ✅ 两个行人都在X=8米处，形成明显的交替穿越模式

### Terminal 2: 验证Gazebo话题

等待Gazebo完全启动（约5秒），然后检查：

```bash
cd ~/ackermann_sim
source install/setup.bash

# 1. 检查Gazebo是否发布actor位姿
echo "=== Testing walker_center pose in Gazebo ==="
timeout 3 gz topic -e -t /model/walker_center/pose -n 1

echo ""
echo "=== Testing walker_east pose in Gazebo ==="
timeout 3 gz topic -e -t /model/walker_east/pose -n 1
```

**成功标准**：
- ✅ 两个命令都能打印出pose数据（包含position和orientation）
- ✅ X值在8左右，Y值在-3到+3之间变化
- ❌ 如果TIMEOUT：PosePublisher插件未加载或actors未生成

### Terminal 3: 验证ROS话题桥接

```bash
cd ~/ackermann_sim
source install/setup.bash

# 1. 检查话题是否存在
echo "=== Checking ROS topics ==="
ros2 topic list | grep -E "(walker|prediction)"

# 应该看到：
# /model/walker_center/pose
# /model/walker_east/pose

# 2. 检查ROS是否接收数据
echo ""
echo "=== Testing ROS bridge for walker_center ==="
timeout 3 ros2 topic echo /model/walker_center/pose --once

echo ""
echo "=== Testing ROS bridge for walker_east ==="
timeout 3 ros2 topic echo /model/walker_east/pose --once

# 3. 检查发布频率
echo ""
echo "=== Checking publish rate ==="
timeout 10 ros2 topic hz /model/walker_center/pose
```

**成功标准**：
- ✅ 话题列表包含 `/model/walker_center/pose` 和 `/model/walker_east/pose`
- ✅ `ros2 topic echo` 能显示pose数据
- ✅ 发布频率约10Hz

---

## 测试场景2：GMM预测器验证

**目标**：验证ground_truth_predictor能接收actor数据并生成预测。

### Terminal 1: 启动仿真（同上）

```bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf
```

### Terminal 2: 启动预测器（等待Gazebo完全启动后）

```bash
cd ~/ackermann_sim
source install/setup.bash
clear

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

**故障排除**：
- ❌ `No data received for walker_*` → 等待5-10秒让数据流动
- ❌ `Stale data` → actors可能还没开始移动，等待几秒

### Terminal 3: 验证预测输出

```bash
cd ~/ackermann_sim
source install/setup.bash

# 1. 检查预测话题
echo "=== Checking prediction topics ==="
ros2 topic list | grep prediction

# 应该看到：
# /pedestrian_predictions
# /prediction_markers

# 2. 查看预测数据
echo ""
echo "=== Viewing prediction data ==="
ros2 topic echo /pedestrian_predictions --once

# 3. 检查发布频率
echo ""
echo "=== Checking prediction publish rate ==="
timeout 10 ros2 topic hz /pedestrian_predictions

# 4. 检查可视化markers
echo ""
echo "=== Checking visualization markers ==="
timeout 10 ros2 topic hz /prediction_markers
```

**成功标准**：
- ✅ `/pedestrian_predictions` 话题存在且以10Hz发布
- ✅ 数据包含2个pedestrian predictions
- ✅ 每个prediction有56步（0.05s × 56 = 2.8s）
- ✅ `current_speed` > 0 （行人正在移动时）
- ✅ `gmm_modes` 有1个component，weight=1.0
- ✅ `/prediction_markers` 也在发布（用于RViz可视化）

### 预测数据示例

```yaml
predictions:
- id: "walker_center"
  current_pose:
    position:
      x: 8.0
      y: -1.2  # 正在从-3向+3移动
      z: 1.0
  current_speed: 1.2  # m/s
  gmm_modes:
  - weight: 1.0
    mean_x: [8.0, 8.0, 8.0, ...]  # 56个值，恒定X
    mean_y: [-1.2, -1.14, -1.08, ...]  # 56个值，向+Y增长
    cov_xx: [0.01, 0.0103, 0.0112, ...]  # 不确定性增长
    cov_yy: [0.01, 0.0103, 0.0112, ...]
    cov_xy: [0.0, 0.0, 0.0, ...]
```

---

## 测试场景3：RViz可视化

**目标**：在RViz中观察预测轨迹，验证GMM可视化。

### Terminal 4: 启动RViz

```bash
cd ~/ackermann_sim
source install/setup.bash

ros2 run rviz2 rviz2
```

### RViz配置步骤

1. **设置Fixed Frame**：
   - 左侧 **Global Options** → **Fixed Frame** → 改为 `odom`

2. **添加MarkerArray**：
   - 点击 **Add** 按钮（左下角）
   - 选择 **By topic** → `/prediction_markers` → **MarkerArray**
   - 确保所有Namespaces都勾选

3. **添加TF（可选）**：
   - **Add** → **TF** → 查看坐标系

### 可视化内容

**应该看到**：

| 颜色 | 形状 | 含义 | 位置 |
|------|------|------|------|
| 🔴 红色 | 球体 | 行人当前位置 | X=8, Y在-3到+3之间 |
| 🔵 蓝色 | 线条 | 预测轨迹（2.8秒） | 从当前位置延伸 |
| 🟢 绿色 | 椭圆 | 1.4秒处的不确定性 | 轨迹中点 |
| 🟡 黄色 | 箭头 | 速度方向 | 指向运动方向 |

**验证要点**：
- ✅ 看到2个红色球体（2个行人）
- ✅ walker_center在右侧，蓝色轨迹指向左侧（+Y方向）
- ✅ walker_east在左侧，蓝色轨迹指向右侧（-Y方向）
- ✅ 绿色椭圆随时间增大（不确定性传播）
- ✅ 轨迹平滑连续，无跳变
- ✅ 标记实时更新（10Hz）

---

## 测试场景4：followpath直线行驶测试

**目标**：车辆沿直线前进，观察行人交替穿越，为后续MPPI测试做准备。

### 准备工作

需要启动以下节点：
1. Gazebo仿真（Terminal 1）
2. 预测器（Terminal 2）
3. Nav2控制器或简化的followpath节点（Terminal 3）

### Terminal 1: 启动仿真

```bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf
```

### Terminal 2: 启动预测器

```bash
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

### Terminal 3: 发送直线路径

**方法A：使用现有path_commander**（如果已配置）

```bash
ros2 launch saye_mppi mppi_experiment.launch.py
```

**方法B：手动发送路径目标**（简化测试）

```bash
# 发送一个直线导航目标到X=20m
ros2 topic pub --once /goal_pose geometry_msgs/msg/PoseStamped "{
  header: {frame_id: 'odom'},
  pose: {
    position: {x: 20.0, y: 0.0, z: 0.0},
    orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}
  }
}"
```

### 观察要点

**在Gazebo中观察**：
1. ⏱️ **t=0-5s**：walker_center从车辆前方右侧穿越到左侧
   - 车辆应该减速/停止/绕行
2. ⏱️ **t=7-12s**：walker_east从车辆前方左侧穿越到右侧
   - 车辆再次遇到障碍
3. 📊 车辆行为：
   - 理想情况：预测到穿越，提前减速/规划绕行
   - 基础情况：检测到障碍后停止/绕行

**在RViz中观察**：
- 预测轨迹是否准确反映行人运动
- 不确定性是否合理增长
- 车辆规划路径是否考虑预测

---

## 验证检查表

### ✅ 基础功能（场景1-2）

- [ ] Gazebo正常加载，看到2个行人交替穿越
- [ ] `gz topic -e` 能接收 `/model/walker_*/pose` 数据
- [ ] ROS话题 `/model/walker_*/pose` 桥接成功
- [ ] 预测器启动无错误
- [ ] `/pedestrian_predictions` 以10Hz发布
- [ ] 预测包含2个行人，每个56步

### ✅ 可视化（场景3）

- [ ] RViz显示红色当前位置球体
- [ ] RViz显示蓝色预测轨迹
- [ ] RViz显示绿色不确定性椭圆
- [ ] RViz显示黄色速度箭头
- [ ] 可视化实时更新

### ✅ 数据质量

- [ ] `current_speed` 在1.0-1.5 m/s范围（行人移动时）
- [ ] 预测轨迹沿Y方向延伸（X≈8保持不变）
- [ ] 不确定性随时间增长（cov_xx和cov_yy递增）
- [ ] 无警告/错误日志

### ✅ 场景配置

- [ ] walker_center初始位置：(8, -3, 1)
- [ ] walker_east初始位置：(8, 3, 1)
- [ ] 两个行人在X=8处交替穿越
- [ ] 穿越时序清晰可见（先右→左，后左→右）

---

## 故障排除

### 问题1: Gazebo话题无数据（`gz topic -e` TIMEOUT）

**可能原因**：
- PosePublisher插件未加载
- actors未正确生成

**解决方法**：
```bash
# 1. 检查插件是否在世界文件中
grep "PosePublisher" src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf

# 2. 检查actors是否生成
gz model -l | grep walker

# 3. 重新编译并重启仿真
colcon build --packages-select saye_description && source install/setup.bash
```

### 问题2: ROS话题无数据（`ros2 topic echo` TIMEOUT）

**可能原因**：
- ros_gz_bridge未启动
- bridge配置缺失

**解决方法**：
```bash
# 1. 检查bridge节点
ros2 node list | grep bridge

# 2. 检查bridge配置
ros2 node info /ros_gz_bridge | grep walker

# 3. 手动检查配置文件
cat src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/config/ros_gz_bridge.yaml | grep walker
```

### 问题3: 预测器报"No data received"

**可能原因**：
- Gazebo还没完全启动
- actors还没开始移动
- 数据流问题

**解决方法**：
1. **等待10-15秒**让Gazebo完全初始化
2. 检查数据流：
   ```bash
   ros2 topic hz /model/walker_center/pose
   ```
3. 重启预测器节点

### 问题4: RViz不显示markers

**可能原因**：
- Fixed Frame错误
- MarkerArray namespace未勾选
- markers未发布

**解决方法**：
1. 确认Fixed Frame = `odom`（不是`map`或`base_link`）
2. 在MarkerArray显示中，展开Namespaces，全部勾选
3. 检查话题：
   ```bash
   ros2 topic echo /prediction_markers --once
   ```

### 问题5: 预测轨迹不准确

**可能原因**：
- 历史数据不足（< 5帧）
- 速度估计不稳定
- CV模型对非直线运动不适用

**诊断方法**：
```bash
# 查看预测数据
ros2 topic echo /pedestrian_predictions --once

# 检查：
# - current_speed是否合理（1.0-1.5 m/s）
# - mean_x/mean_y是否连续
# - cov值是否合理增长
```

---

## 性能指标

### 预测精度（理论值）

| 时刻 | CV模型误差（ADE） | 备注 |
|------|------------------|------|
| 0.5s | < 0.05m | 短期预测非常准 |
| 1.0s | < 0.1m | 中期预测较准 |
| 1.4s | < 0.2m | 论文评估点 |
| 2.0s | < 0.4m | 长期预测误差增大 |
| 2.8s | < 0.8m | 最远预测点 |

**注意**：
- 上述指标适用于**直线匀速运动**（行人穿越期间）
- 转向期间（waypoint转折点）误差会显著增大
- 可以运行 `prediction_verifier` 节点实时计算ADE/FDE

### 计算性能

- **预测频率**：10 Hz
- **每帧计算**：2个行人 × 56步 = 112个预测点
- **延迟**：< 5ms（Python实现）

---

## 下一步开发

### Phase 1: 多场景测试

创建更多测试场景：

```bash
# 场景1: 4个行人（已有配置）
ros2 launch saye_bringup_my saye_spawn.launch.py \
  world:=scenario1_4ped_crossing.sdf

ros2 launch saye_prediction ground_truth_prediction.launch.py \
  config:=scenario1_actors.yaml
```

### Phase 2: IMM预测器

- 实现3模态GMM（CV + CA + Stop）
- 对比单模态vs多模态性能
- 测试转向/停止场景

### Phase 3: MPPI集成

- 实现RiskAwareCritic（订阅`/pedestrian_predictions`）
- 计算CP（碰撞概率）和CVaR（风险评估）
- 集成到Nav2 MPPI控制器

### Phase 4: LiDAR预测

- 实现基于Costmap的预测器
- 对比ground truth vs LiDAR性能
- 测试遮挡/噪声鲁棒性

---

## 参考文档

- **项目主文档**: `CLAUDE.md`
- **进度记录**: `memory.md`
- **快速测试**: `TEST_NOW.md`
- **消息定义**:
  - `saye_msgs/msg/GMMComponent.msg`
  - `saye_msgs/msg/PedestrianPrediction.msg`
  - `saye_msgs/msg/PedestrianPredictions.msg`

---

**创建日期**: 2025-10-30
**最后更新**: 2025-10-30
**版本**: 1.0
