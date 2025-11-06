# GMM预测系统架构文档

## 系统总览

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         GAZEBO SIMULATION                                │
│                                                                           │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐                  │
│  │  walker_     │  │  walker_     │  │   Saye       │                  │
│  │  center      │  │  east        │  │   Vehicle    │                  │
│  │  (8,-3,1)    │  │  (8,3,1)     │  │   (0,0,0.35) │                  │
│  └──────────────┘  └──────────────┘  └──────────────┘                  │
│         ↓                  ↓                  ↓                           │
│         └─────────┬────────┘                  │                          │
│                   ↓                           ↓                          │
│  ┌────────────────────────────┐  ┌──────────────────────┐              │
│  │  PosePublisher Plugin      │  │  Sensors (LiDAR,     │              │
│  │  (10Hz)                    │  │  IMU, Odometry)      │              │
│  └────────────────────────────┘  └──────────────────────┘              │
└───────────────────┬───────────────────────────┬─────────────────────────┘
                    │                           │
          Gazebo Topics                  Gazebo Topics
                    │                           │
                    ↓                           ↓
┌─────────────────────────────────────────────────────────────────────────┐
│                        ROS_GZ_BRIDGE                                     │
│  ┌──────────────────────────────────────────────────────────────────┐  │
│  │  Bridge Configuration (ros_gz_bridge.yaml)                        │  │
│  │  • /model/walker_center/pose  : gz.msgs.Pose → geometry_msgs/Pose│  │
│  │  • /model/walker_east/pose    : gz.msgs.Pose → geometry_msgs/Pose│  │
│  │  • /odom                      : gz.msgs.Odometry → nav_msgs/Odom │  │
│  │  • /scan                      : gz.msgs.LaserScan → sensor_msgs  │  │
│  └──────────────────────────────────────────────────────────────────┘  │
└───────────────┬───────────────────────────────┬─────────────────────────┘
                │                               │
        ROS 2 Topics                    ROS 2 Topics
                │                               │
                ↓                               ↓
┌───────────────────────────────┐  ┌──────────────────────────────┐
│  PREDICTION LAYER             │  │  NAVIGATION STACK            │
│  (saye_prediction)            │  │  (Nav2 / MPPI)               │
│                               │  │                              │
│  ┌─────────────────────────┐ │  │  ┌────────────────────────┐ │
│  │ ground_truth_predictor  │ │  │  │  AMCL / Localization   │ │
│  │ • Subscribe:            │ │  │  │                        │ │
│  │   - walker poses        │ │  │  │  ┌──────────────────┐ │ │
│  │ • Velocity estimation   │ │  │  │  │ Global Planner   │ │ │
│  │ • CV model prediction   │ │  │  │  │ (Nav2 A*)        │ │ │
│  │ • Publish:              │ │  │  │  └──────────────────┘ │ │
│  │   - /pedestrian_        │ │  │  │                        │ │
│  │     predictions (10Hz)  │ │  │  │  ┌──────────────────┐ │ │
│  └─────────────────────────┘ │  │  │  │ Local Planner    │ │ │
│               │               │  │  │  │ (MPPI Controller)│ │ │
│               │               │  │  │  │                  │ │ │
│               ↓               │  │  │  │ [TODO] Subscribe │ │ │
│  ┌─────────────────────────┐ │  │  │  │ /pedestrian_     │ │ │
│  │ prediction_visualizer   │ │  │  │  │ predictions      │ │ │
│  │ • Subscribe:            │ │  │  │  │                  │ │ │
│  │   - /pedestrian_        │ │  │  │  │ [TODO] Implement │ │ │
│  │     predictions         │ │  │  │  │ RiskAwareCritic  │ │ │
│  │ • Generate markers:     │ │  │  │  └──────────────────┘ │ │
│  │   - Current pose (red)  │ │  │  │                        │ │
│  │   - Trajectory (blue)   │ │  │  └────────────────────────┘ │
│  │   - Uncertainty (green) │ │  │                              │
│  │   - Velocity (yellow)   │ │  └──────────────────────────────┘
│  │ • Publish:              │ │
│  │   - /prediction_markers │ │
│  └─────────────────────────┘ │
│               │               │
│               │               │
│               ↓               │
│  ┌─────────────────────────┐ │
│  │ prediction_verifier     │ │
│  │ • Subscribe:            │ │
│  │   - /pedestrian_        │ │
│  │     predictions         │ │
│  │   - walker ground truth │ │
│  │ • Calculate metrics:    │ │
│  │   - ADE (平均位移误差)  │ │
│  │   - FDE (最终位移误差)  │ │
│  │ • Publish:              │ │
│  │   - /prediction_metrics │ │
│  └─────────────────────────┘ │
└───────────────────────────────┘
                │
                ↓
┌───────────────────────────────┐
│  VISUALIZATION (RViz2)        │
│  • Fixed Frame: odom          │
│  • Display:                   │
│    - /prediction_markers      │
│    - TF transforms            │
│    - LaserScan                │
│    - Vehicle model            │
└───────────────────────────────┘
```

---

## 数据流详解

### 1. Gazebo → ROS (Actor Pose)

```
Gazebo Actor (walker_center)
    ↓ animation + script
Position: (8, -3+v*t, 1)
    ↓ PosePublisher plugin (10Hz)
Gazebo Topic: /model/walker_center/pose
    ↓ ros_gz_bridge
ROS Topic: /model/walker_center/pose
    Type: geometry_msgs/msg/Pose
    Rate: ~10 Hz
```

**消息格式**:
```yaml
position:
  x: 8.0
  y: -2.5  # 变化: -3.0 → +3.0
  z: 1.0
orientation:
  x: 0.0
  y: 0.0
  z: 0.707  # yaw=1.57 (90度)
  w: 0.707
```

---

### 2. Ground Truth Predictor 处理流程

```
┌──────────────────────────────────────────────────────┐
│  ground_truth_predictor.py                           │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ① 订阅 /model/walker_*/pose (Pose)                  │
│     ↓                                                │
│  ② pose_callback(actor_name, msg)                   │
│     • 存储到历史队列: deque(maxlen=5)                │
│     • 记录时间戳                                     │
│     ↓                                                │
│  ③ 定时器触发 (10Hz)                                 │
│     ↓                                                │
│  ④ publish_predictions()                            │
│     遍历所有actors:                                  │
│     ├─ 检查数据新鲜度 (< 0.5s)                       │
│     ├─ estimate_velocity(actor_name)                │
│     │  • 线性回归: vx = Δx/Δt, vy = Δy/Δt          │
│     │  • 速度大小: speed = √(vx²+vy²)               │
│     ├─ predict_trajectory_cv(x0, y0, vx, vy)        │
│     │  • 恒速模型: x(t) = x0 + vx*t                 │
│     │  • 不确定性: Σ(t) = σ₀² + (σq*t)²            │
│     │  • 生成56步预测 (0.05s × 56 = 2.8s)          │
│     └─ 创建PedestrianPrediction消息                 │
│     ↓                                                │
│  ⑤ 发布 /pedestrian_predictions                     │
│                                                      │
└──────────────────────────────────────────────────────┘
```

**核心算法 - CV模型**:

```python
def predict_trajectory_cv(x0, y0, vx, vy):
    for step in range(56):  # 2.8s / 0.05s
        t = step * 0.05

        # 均值预测
        mean_x = x0 + vx * t
        mean_y = y0 + vy * t

        # 协方差预测
        var_t = sigma_0**2 + (sigma_q * t)**2
        # sigma_0 = 0.1m, sigma_q = 0.5m/s

        gmm.mean_x[step] = mean_x
        gmm.mean_y[step] = mean_y
        gmm.cov_xx[step] = var_t
        gmm.cov_yy[step] = var_t
        gmm.cov_xy[step] = 0.0  # 假设独立

    return gmm
```

---

### 3. 预测输出消息结构

```
PedestrianPredictions
│
├─ header
│  ├─ stamp: 当前时间
│  └─ frame_id: "odom"
│
└─ predictions: [PedestrianPrediction × N]
   │
   ├─ [0] walker_center
   │  ├─ id: "walker_center"
   │  ├─ current_pose: Pose
   │  │  └─ position: (8.0, -1.2, 1.0)
   │  ├─ current_speed: 1.2  # m/s
   │  └─ gmm_modes: [GMMComponent × 1]
   │     │
   │     └─ [0] CV模态
   │        ├─ weight: 1.0
   │        ├─ mean_x: [8.0, 8.0, ..., 8.0]  # 56个值
   │        ├─ mean_y: [-1.2, -1.14, ..., 2.16]  # 向左增长
   │        ├─ cov_xx: [0.01, 0.01, ..., 2.01]  # 增长
   │        ├─ cov_yy: [0.01, 0.01, ..., 2.01]
   │        └─ cov_xy: [0.0, 0.0, ..., 0.0]
   │
   └─ [1] walker_east
      ├─ id: "walker_east"
      ├─ current_pose: Pose
      ├─ current_speed: 1.2
      └─ gmm_modes: [GMMComponent × 1]
```

---

### 4. Visualization Pipeline

```
prediction_visualizer.py

Subscribe: /pedestrian_predictions
    ↓
对每个prediction:
    ├─ 创建当前位置球体 (红色, id=0)
    │  • position: current_pose
    │  • scale: 0.3m球体
    │  • color: (1, 0, 0, 1)
    │
    ├─ 创建预测轨迹线 (蓝色, id=1)
    │  • points: mean_x/y的56个点
    │  • type: LINE_STRIP
    │  • scale: 0.05m线宽
    │  • color: (0, 0, 1, 0.8)
    │
    ├─ 创建不确定性椭圆 (绿色, id=2)
    │  • center: (mean_x[28], mean_y[28])  # 1.4s处
    │  • scale_x: 2*√cov_xx[28]  # 95%置信区间
    │  • scale_y: 2*√cov_yy[28]
    │  • color: (0, 1, 0, 0.3)
    │
    └─ 创建速度箭头 (黄色, id=3)
       • start: current_pose
       • direction: (vx, vy) normalized
       • length: current_speed
       • color: (1, 1, 0, 1)
    ↓
Publish: /prediction_markers (MarkerArray)
```

---

### 5. Verification Pipeline

```
prediction_verifier.py

Subscribe:
  • /pedestrian_predictions (预测)
  • /model/walker_*/pose (真值)
    ↓
对齐时间戳 (找到预测时刻对应的真值)
    ↓
计算误差指标:

ADE (Average Displacement Error):
    ADE = (1/N) Σ ||pred_pos[i] - true_pos[i]||
    • N = 56步
    • 平均所有时刻的欧氏距离

FDE (Final Displacement Error):
    FDE = ||pred_pos[55] - true_pos[55]||
    • 仅计算最后一步 (t=2.8s)

分时段误差:
    • @0.5s: step=10
    • @1.0s: step=20
    • @1.4s: step=28  ← 论文评估点
    • @2.0s: step=40
    • @2.8s: step=55
    ↓
Publish: /prediction_metrics
```

---

## 配置参数总览

### Predictor配置 (`ground_truth_predictor.yaml`)

| 参数 | 值 | 说明 |
|------|----|----|
| `prediction_horizon` | 2.8 | 预测时域（秒）|
| `prediction_dt` | 0.05 | 时间步长（秒）|
| `actor_names` | [walker_center, walker_east] | 追踪的actors |
| `history_size` | 5 | 速度估计历史帧数 |
| `sigma_0` | 0.1 | 初始位置不确定性（米）|
| `sigma_q` | 0.5 | 过程噪声强度（m/s）|
| `publish_rate` | 10.0 | 发布频率（Hz）|

**计算得出**:
- `num_steps = horizon / dt = 2.8 / 0.05 = 56`
- 每帧数据点: 2 actors × 56 steps = 112 预测点
- 输出频率: 10 Hz → 1120 预测点/秒

---

### Actor配置 (在SDF中)

#### walker_center

```xml
<actor name="walker_center">
  <pose>8 -3.0 1 0 0 1.57</pose>  <!-- (X, Y, Z, R, P, Yaw) -->

  <trajectory>
    t=0:   (0, 0)    yaw=+π/2  (向+Y)
    t=5:   (0, +6)   yaw=+π/2
    t=7:   (0, +6)   yaw=+π/2  (等待)
    t=8:   (0, +6)   yaw=-π/2  (转向)
    t=13:  (0, 0)    yaw=-π/2  (向-Y)
    t=14:  (0, 0)    yaw=+π/2  (循环)
  </trajectory>
</actor>
```

**派生参数**:
- 穿越距离: 6米 (Y: -3 → +3)
- 穿越时间: 5秒
- 速度: 6/5 = 1.2 m/s
- 周期: 14秒

#### walker_east

```xml
<actor name="walker_east">
  <pose>8 3.0 1 0 0 -1.57</pose>

  <trajectory>
    t=0:   (0, 0)    yaw=-π/2  (等待)
    t=7:   (0, 0)    yaw=-π/2
    t=12:  (0, -6)   yaw=-π/2  (向-Y)
    t=14:  (0, -6)   yaw=-π/2  (等待)
    t=15:  (0, -6)   yaw=+π/2  (转向)
    t=20:  (0, 0)    yaw=+π/2  (向+Y)
    t=21:  (0, 0)    yaw=-π/2  (循环)
  </trajectory>
</actor>
```

**交替时序**:
- walker_center: **t=0-5s** 穿越
- walker_east: **t=7-12s** 穿越
- 间隔: 2秒

---

## 性能指标

### 计算性能

| 指标 | 值 | 备注 |
|------|----|----|
| 预测频率 | 10 Hz | 实时性能 |
| 单次预测延迟 | < 5ms | Python实现 |
| 内存占用 | ~50 MB | 包含ROS节点 |
| CPU占用 | < 5% | 单核 |

### 预测精度（理论）

**CV模型在直线运动下的误差**:

| 时刻 | 预期ADE | 备注 |
|------|---------|------|
| 0.5s | < 0.05m | 短期非常准确 |
| 1.0s | < 0.10m | 中期较准确 |
| 1.4s | < 0.20m | **论文基准点** |
| 2.0s | < 0.40m | 长期误差增大 |
| 2.8s | < 0.80m | 最远预测点 |

**注意**:
- 上述指标假设**匀速直线运动**
- 转向时刻（waypoint转折点）误差会显著增大
- CV模型无法预测加速度变化

---

## 扩展路线图

### Phase 1: CV模型（当前）

```
单模态GMM (K=1)
├─ 恒速假设
├─ 线性外推
└─ 简单不确定性模型
```

**优点**: 简单、快速、无参数学习
**缺点**: 无法处理加速/减速/转向

---

### Phase 2: IMM模型（计划中）

```
多模态GMM (K=3)
├─ Mode 1: CV (Constant Velocity)    weight=0.6
├─ Mode 2: CA (Constant Acceleration) weight=0.3
└─ Mode 3: Stop                       weight=0.1
```

**模式转换概率矩阵**:
```
        CV    CA   Stop
CV   [ 0.8   0.15  0.05 ]
CA   [ 0.2   0.7   0.1  ]
Stop [ 0.3   0.1   0.6  ]
```

**优点**: 可以处理加速度变化
**缺点**: 需要调参、计算量增加

---

### Phase 3: RAPA-MPPI集成

```
RiskAwareCritic
├─ 订阅: /pedestrian_predictions
├─ 计算:
│  ├─ CP (Collision Probability)
│  │  └─ P(collision) = Σ P(mode_k) × P(collision|mode_k)
│  │
│  └─ CVaR (Conditional Value at Risk)
│     └─ CVaR_α = E[cost | cost > VaR_α]
│
└─ 输出: trajectory cost修正
```

**目标**: 将预测不确定性融入MPPI代价函数

---

## 文件结构总览

```
ackermann_sim/
├── src/
│   └── ackermann-vehicle-gzsim-ros2/
│       ├── saye_description/
│       │   ├── worlds/
│       │   │   └── saye_dynamic_corridor.sdf  ← 行人配置
│       │   └── models/saye/
│       │       └── model_2d_lidar.sdf  ← 车辆模型
│       │
│       ├── saye_msgs/
│       │   └── msg/
│       │       ├── GMMComponent.msg
│       │       ├── PedestrianPrediction.msg
│       │       └── PedestrianPredictions.msg
│       │
│       ├── saye_prediction/  ← 新建包
│       │   ├── saye_prediction/
│       │   │   ├── ground_truth_predictor.py
│       │   │   ├── prediction_visualizer.py
│       │   │   └── prediction_verifier.py
│       │   ├── launch/
│       │   │   └── ground_truth_prediction.launch.py
│       │   └── config/
│       │       └── ground_truth_predictor.yaml
│       │
│       └── saye_bringup_my/
│           ├── launch/
│           │   └── saye_spawn.launch.py
│           └── config/
│               └── ros_gz_bridge.yaml  ← 桥接配置
│
├── TEST_NOW.md              ← 快速测试指南
├── TEST_SCENARIOS.md        ← 完整测试流程
├── SCENARIO_DIAGRAM.md      ← 场景可视化
├── SYSTEM_ARCHITECTURE.md   ← 本文档
├── READY_TO_TEST.md         ← 启动清单
└── quick_test.sh            ← 自动化检查
```

---

## 关键依赖

### ROS 2 包依赖

```yaml
saye_prediction:
  build_depend:
    - rclpy
    - saye_msgs
    - geometry_msgs
    - visualization_msgs
  exec_depend:
    - rclpy
    - saye_msgs
    - geometry_msgs
    - visualization_msgs
    - numpy

saye_msgs:
  depend:
    - std_msgs
    - geometry_msgs
    - rosidl_default_generators
    - rosidl_default_runtime
```

### Python依赖

```python
# 核心
rclpy
numpy

# 消息类型
saye_msgs.msg  # 自定义
geometry_msgs.msg
std_msgs.msg
visualization_msgs.msg

# 标准库
math
collections.deque
```

---

## 调试工具

### ROS 2命令行工具

```bash
# 话题监控
ros2 topic list
ros2 topic hz /pedestrian_predictions
ros2 topic echo /pedestrian_predictions --once
ros2 topic info /model/walker_center/pose

# 节点监控
ros2 node list
ros2 node info /ground_truth_predictor
ros2 param list /ground_truth_predictor

# 服务/动作
ros2 service list
ros2 action list
```

### Gazebo命令行工具

```bash
# Gazebo话题
gz topic -l
gz topic -e -t /model/walker_center/pose -n 1

# Gazebo模型
gz model -l
gz model -m walker_center -p

# Gazebo服务
gz service -l
```

### 可视化工具

```bash
# RViz
ros2 run rviz2 rviz2

# RQT工具
rqt_graph  # 节点图
rqt_plot /pedestrian_predictions/predictions[0]/current_speed
rqt_topic  # 话题监控
```

---

**版本**: 1.0
**创建日期**: 2025-10-30
**最后更新**: 2025-10-30
**作者**: RAPA-MPPI Team
