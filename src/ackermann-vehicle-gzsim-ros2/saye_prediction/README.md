# saye_prediction

基于GMM的行人轨迹预测包，用于RAPA-MPPI局部规划器。

## 功能

1. **ground_truth_predictor**: 从Gazebo actor真实位置生成GMM预测
2. **prediction_visualizer**: 在RViz中可视化预测轨迹
3. **prediction_verifier**: 验证预测精度

## 编译

```bash
cd ~/ackermann_sim
colcon build --packages-select saye_msgs saye_prediction
source install/setup.bash
```

## 使用方法

### 方式1：单独启动节点

```bash
# Terminal 1: 启动仿真
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_dynamic_corridor.sdf

# Terminal 2: 启动预测器
ros2 run saye_prediction ground_truth_predictor

# Terminal 3: 启动可视化（可选）
ros2 run saye_prediction prediction_visualizer

# Terminal 4: 启动验证器（可选）
ros2 run saye_prediction prediction_verifier
```

### 方式2：使用Launch文件

```bash
# Terminal 1: 启动仿真
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf

# Terminal 2: 启动预测（包含可视化）
ros2 launch saye_prediction ground_truth_prediction.launch.py \
  config:=scenario1_actors.yaml

# 启用验证器
ros2 launch saye_prediction ground_truth_prediction.launch.py \
  config:=scenario1_actors.yaml \
  enable_verifier:=true
```

## 配置场景

根据不同的场景，修改配置文件中的`actor_names`:

### Scenario 1: 4个行人交叉
```yaml
actor_names: [
  'walker_oncoming_1',
  'walker_oncoming_2',
  'walker_crossing_left',
  'walker_crossing_right'
]
```

### 默认场景: 2个行人
```yaml
actor_names: [
  'walker_center',
  'walker_east'
]
```

## RViz可视化

在RViz中添加以下显示：

1. **MarkerArray**: `/prediction_markers`
   - 红色球体：当前位置
   - 蓝色线条：预测轨迹
   - 绿色椭圆：不确定性（1.4秒后）
   - 黄色箭头：速度向量

2. **TF**: 查看坐标系

## 话题说明

- **订阅**:
  - `/model/{actor_name}/pose` (Pose) - Gazebo actor位置

- **发布**:
  - `/pedestrian_predictions` (PedestrianPredictions) - GMM预测
  - `/prediction_markers` (MarkerArray) - 可视化标记

## 验证指标

运行`prediction_verifier`后，每5秒打印一次统计信息：

```
Prediction Error Statistics
Actor: walker_center
  @0.5s (120 samples): mean=0.082m, std=0.045m, max=0.156m, min=0.012m
  @1.0s (120 samples): mean=0.165m, std=0.091m, max=0.312m, min=0.024m
  @1.4s (120 samples): mean=0.231m, std=0.128m, max=0.437m, min=0.034m
    ✅ PASS: mean error < 0.3m
```

目标：**@1.4s的平均误差 < 0.3m** (恒速模型在直线场景下)

## 参数调优

在`config/ground_truth_predictor.yaml`中可调整：

- `sigma_0`: 初始不确定性（默认0.1m）
- `sigma_q`: 过程噪声强度（默认0.5 m/s）
- `history_size`: 速度估计窗口（默认5帧）
- `publish_rate`: 发布频率（默认10Hz）

## 故障排除

### 问题1: 没有预测输出

检查actor名称是否正确：
```bash
ros2 topic list | grep /model
```

### 问题2: RViz不显示marker

检查话题：
```bash
ros2 topic echo /prediction_markers
```

确保RViz的Fixed Frame设置为`odom`。

## 下一步

完成ground truth预测后，可以：
1. 集成到MPPI的RiskAwareCritic
2. 实现Costmap-based预测器
3. 扩展为多模态IMM预测
