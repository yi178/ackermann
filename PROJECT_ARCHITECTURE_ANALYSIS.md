# Ackermann 车辆仿真项目架构分析

## 项目概述

**项目位置：** `/home/user/ackermann/src/ackermann-vehicle-gzsim-ros2/`
**项目规模：** 115 MB，10个ROS 2功能包
**技术栈：** ROS 2 Jazzy + Gazebo Harmonic + Nav2 + MPPI控制器
**核心目标：** 在动态行人场景中测试和优化MPPI（模型预测路径积分）局部规划器

---

## 一、项目架构总览

### 1.1 整体架构层次

```
┌─────────────────────────────────────────────────────────┐
│                    用户交互层                              │
│  Launch Files / RViz / Configuration Parameters          │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│                    应用逻辑层                              │
│  • saye_mppi (实验控制)                                    │
│  • saye_prediction (行人预测)                              │
│  • saye_rapa_mppi (风险感知评价器)                          │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│                   导航与控制层                             │
│  Nav2 Stack + MPPI Controller + Custom Critics           │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│                   仿真与传感层                             │
│  Gazebo Harmonic + Sensors + Actor System                │
│  • saye_description (模型/世界/插件)                        │
└─────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────┐
│                    消息与接口层                            │
│  saye_msgs (自定义消息) + ROS 2 Standard Messages         │
└─────────────────────────────────────────────────────────┘
```

### 1.2 功能包依赖关系图

```
                    ┌──────────────┐
                    │  saye_msgs   │ (消息定义基础)
                    └──────┬───────┘
                           │
        ┌──────────────────┼──────────────────┐
        ↓                  ↓                   ↓
┌───────────────┐  ┌──────────────┐  ┌─────────────────┐
│saye_prediction│  │ saye_control │  │ saye_rapa_mppi  │
│  (行人预测)     │  │  (车辆控制)   │  │  (风险评价器)    │
└───────┬───────┘  └──────────────┘  └────────┬────────┘
        │                                      │
        │          ┌──────────────────┐        │
        └─────────→│   saye_mppi      │←───────┘
                   │  (MPPI实验框架)   │
                   └────────┬─────────┘
                            │
        ┌───────────────────┼───────────────────┐
        ↓                   ↓                   ↓
┌──────────────┐   ┌────────────────┐  ┌──────────────┐
│saye_bringup  │   │saye_bringup_my │  │saye_description│
│ (完整导航栈)   │   │  (2D激光版本)   │  │ (模型/世界)    │
└──────────────┘   └────────────────┘  └──────────────┘
```

---

## 二、核心功能包详细分析

### 2.1 saye_msgs - 消息定义包

**作用：** 定义行人预测系统的自定义消息类型

**关键消息：**

#### PedestrianPredictions.msg（行人预测集合）
```
std_msgs/Header header
PedestrianPrediction[] predictions  # 多个行人的预测
```

#### PedestrianPrediction.msg（单个行人预测）
```
string id                          # 行人唯一标识
geometry_msgs/Pose current_pose    # 当前位置
float64 current_speed              # 当前速度 (m/s)
GMMComponent[] gmm_modes           # GMM模式（高斯混合模型）
```

#### GMMComponent.msg（GMM分量）
```
float64 weight                     # 模式权重
float64[] mean_x                   # X轴均值序列 [56个时间步]
float64[] mean_y                   # Y轴均值序列
float64[] cov_xx                   # X方差序列
float64[] cov_yy                   # Y方差序列
float64[] cov_xy                   # XY协方差序列
```

**时间跨度：** 2.8秒预测（56步 × 0.05秒/步）

**设计理念：**
- GMM表示多模态不确定性（例如：行人可能左转、右转或直行）
- 协方差随时间增长，反映预测不确定性
- 与MPPI时间窗口完全对齐

---

### 2.2 saye_description - 仿真环境包

**作用：** 提供车辆模型、世界文件和Gazebo插件

#### 车辆模型

**标准版本：** `models/saye/model.sdf`
- 3D激光雷达（360° × 32线）
- 4个摄像头（前、后、左、右）
- IMU + 里程计
- Ackermann转向约束

**2D激光版本：** `models/saye/model_2d_lidar.sdf`（实验常用）
- 单层2D激光雷达（360° × 1线，降低计算负担）
- 其他配置同标准版本

#### 世界文件结构

**基础世界：**
```
saye_world.sdf              # 仓库环境（原始）
saye_long_corridor.sdf      # 长走廊（前进测试）
```

**动态走廊世界（MPPI测试专用）：**
```
saye_dynamic_corridor.sdf           # 内联行人Actor（工作中）✓
saye_dynamic_corridor_plugin.sdf    # 插件生成行人（失效）✗
saye_moving_obstacles.sdf           # 移动箱体障碍物
```

**场景世界（自动生成）：**
```
scenario0.sdf    # 空走廊（用于SLAM建图）
scenario1.sdf    # 4个行人（交叉模式）
scenario2.sdf    # 8个行人（混合模式）
scenario3.sdf    # 12个行人（拥挤环境）
scenario4.sdf    # 6个行人+静态障碍物
scenario5.sdf    # 6个行人（随机移动）
```

**世界生成流程：**
```python
# scripts/generate_world.py
Python脚本 + Jinja2模板 + YAML配置
        ↓
worlds/templates/saye_dynamic_corridor.sdf.jinja2
        +
config/pedestrians_*.yaml
        ↓
生成 worlds/scenario*.sdf
```

CMake在构建时自动调用生成脚本。

#### Gazebo插件

**1. ActorPosePublisherSystem（C++系统插件）**
```cpp
功能：从Gazebo中提取行人Actor的位姿
输出：
  - 单独话题：/actor_pose/{actor_name}
  - 聚合话题：/world/{world}/pose/info (Pose_V消息)
配置：SDF中指定话题前缀和发布频率
状态：✓ 正常工作
```

**2. PedestrianManagerSystem（C++系统插件）**
```cpp
功能：从YAML配置动态生成行人
配置：plugins/pedestrian_manager/config/pedestrian_actors_plugin.yaml
状态：✗ 编译通过但不生成Actor
替代方案：使用内联<actor>标签
```

**插件编译产物：**
- `libactor_pose_publisher_system.so`
- `libpedestrian_manager_system.so`

---

### 2.3 saye_mppi - MPPI实验框架

**作用：** 提供MPPI控制器的独立测试环境（无AMCL、无全局规划器）

#### Python节点

**1. path_commander.py（直线路径指挥器）**
```python
类名：StraightPathCommander
订阅：/odom (nav_msgs/Odometry)
动作：/follow_path (nav2_msgs/action/FollowPath)

功能：
1. 订阅里程计获取当前位置
2. 生成从当前位置到目标的直线路径
3. 发送FollowPath动作到controller_server

配置文件：config/path_commander.yaml
goal: [4.0, 0.0, 0.0]  # [x, y, yaw] in odom frame
path_resolution: 0.25   # 路径点间距
```

**关键设计：**
- 使用里程计而非TF查找，无需AMCL
- 绕过全局规划器，直接测试局部控制器
- 适合走廊等简单几何环境

**2. moving_obstacles.py（移动障碍物生成器）**
```python
类名：MovingObstacles
服务：/world/{world}/spawn_entity
      /world/{world}/set_entity_pose

功能：
1. 生成简单箱体障碍物
2. 以恒定速度沿直线移动
3. 用于测试动态避障

状态：当前在launch文件中注释掉（使用场景世界中的行人代替）
```

#### Launch文件

**mppi_experiment.launch.py（主实验入口）**
```python
启动节点：
1. static_transform_publisher  # map->odom静态变换（绕过AMCL）
2. controller_server           # Nav2控制器（仅MPPI）
3. behavior_server             # 行为树服务器
4. velocity_smoother           # 速度平滑器
5. collision_monitor           # 碰撞监控器
6. lifecycle_manager           # 生命周期管理器
7. [延迟5秒] straight_path_commander  # 路径指挥器

配置文件：config/mppi_nav2_params.yaml
```

#### 关键配置文件

**config/mppi_nav2_params.yaml（核心配置）**
```yaml
controller_server:
  controller_frequency: 20.0
  controller_plugins: ["FollowPath"]

  FollowPath:
    plugin: "nav2_mppi_controller::MPPIController"

    # === MPPI核心参数 ===
    time_steps: 56              # 预测步数
    model_dt: 0.05              # 时间步长 (秒)
    batch_size: 2000            # 轨迹采样数量

    # === 控制约束 ===
    vx_max: 0.5                 # 最大前向速度 (m/s)
    vx_min: -0.3                # 最大倒车速度
    wz_max: 1.8                 # 最大角速度 (rad/s)

    # === 噪声参数 ===
    vx_std: 0.2                 # 前向速度噪声标准差
    vy_std: 0.1                 # 侧向速度噪声
    wz_std: 0.4                 # 角速度噪声

    # === 运动模型 ===
    motion_model: "Ackermann"   # Ackermann转向约束
    AckermannConstraints:
      min_turning_r: 0.25       # 最小转弯半径 (m)

    # === 评价器（Critics）列表 ===
    critics: [
      "ConstraintCritic",       # 约束检查（Ackermann）
      "CostCritic",             # 代价地图碰撞
      "GoalCritic",             # 目标接近度
      "PathAlignCritic",        # 路径对齐
      "PathFollowCritic",       # 路径跟随
      "PreferForwardCritic",    # 偏好前进
      "RiskAwareCritic"         # 风险感知（自定义）
    ]

    # === 自定义评价器配置 ===
    RiskAwareCritic:
      plugin: "saye_rapa_mppi::RiskAwareCritic"  # 关键：插件全限定名
      enabled: True
      cost_power: 1
      cost_weight: 1.0
```

**配置关键点：**
1. **critics列表**：使用简单名称 `"RiskAwareCritic"`
2. **配置块**：必须包含 `plugin` 字段指定完整类名
3. **时间窗口对齐**：56步×0.05秒 = 2.8秒（与预测系统一致）

---

### 2.4 saye_rapa_mppi - 风险感知评价器插件

**作用：** Nav2 MPPI控制器的自定义评价器插件，用于行人避障

#### 实现状态

**当前状态：** Phase 1 - 框架实现（无成本计算）

**已实现功能：**
- ✓ 插件加载和注册
- ✓ 订阅 `/pedestrian_predictions` 话题
- ✓ 线程安全的数据访问（mutex保护）
- ✓ 初始化和生命周期管理
- ✓ 日志输出

**未实现功能（待Phase 2）：**
- ✗ 碰撞概率计算
- ✗ CVaR风险评估
- ✗ 轨迹成本计算

#### 代码结构

**include/saye_rapa_mppi/risk_aware_critic.hpp**
```cpp
class RiskAwareCritic : public mppi::critics::CriticFunction {
public:
  void initialize() override;           // 初始化（订阅预测）
  void score(mppi::CriticData& data) override;  // 评分（计算成本）

protected:
  void predictionCallback(
    const saye_msgs::msg::PedestrianPredictions::SharedPtr msg);

private:
  rclcpp::Subscription<...>::SharedPtr prediction_sub_;
  saye_msgs::msg::PedestrianPredictions::SharedPtr latest_predictions_;
  std::mutex prediction_mutex_;
  double cost_weight_{1.0};
  size_t iteration_count_{0};
};
```

**src/risk_aware_critic.cpp**
```cpp
void RiskAwareCritic::initialize() {
  auto node = parent_.lock();  // 获取父节点

  // 加载参数
  auto getParam = parameters_handler_->getParamGetter(name_);
  getParam(cost_weight_, "cost_weight", 1.0);

  // 订阅行人预测
  prediction_sub_ = node->create_subscription<...>(
    "/pedestrian_predictions",
    rclcpp::SystemDefaultsQoS(),
    std::bind(&RiskAwareCritic::predictionCallback, this, _1)
  );
}

void RiskAwareCritic::score(mppi::CriticData& data) {
  iteration_count_++;

  // Phase 1: 仅验证框架，不计算成本
  if (iteration_count_ % 50 == 0) {
    std::lock_guard<std::mutex> lock(prediction_mutex_);
    if (latest_predictions_) {
      RCLCPP_INFO(logger_, "收到%zu个行人的预测",
                  latest_predictions_->predictions.size());
    }
  }

  // Phase 2将在此处添加成本计算逻辑
}
```

#### 插件注册

**plugins.xml**
```xml
<library path="libsaye_rapa_mppi">
  <class type="saye_rapa_mppi::RiskAwareCritic"
         base_class_type="mppi::critics::CriticFunction">
    <description>
      RAPA-MPPI风险感知评价器：使用碰撞概率(CP)剪枝
      和条件风险价值(CVaR)进行两阶段风险评估
    </description>
  </class>
</library>
```

**CMakeLists.txt关键部分**
```cmake
add_library(saye_rapa_mppi SHARED src/risk_aware_critic.cpp)
ament_target_dependencies(saye_rapa_mppi
  rclcpp nav2_mppi_controller saye_msgs pluginlib)

# 导出插件描述
pluginlib_export_plugin_description_file(nav2_mppi_controller plugins.xml)

install(TARGETS saye_rapa_mppi
  LIBRARY DESTINATION lib)
```

#### Phase 2实现计划

**预期功能（参考RAPA_MPPI_IMPLEMENTATION_PLAN.md）：**

```cpp
void RiskAwareCritic::score(mppi::CriticData& data) {
  std::lock_guard<std::mutex> lock(prediction_mutex_);
  if (!latest_predictions_) return;

  // 对每条候选轨迹
  for (size_t i = 0; i < data.costs.shape(0); i++) {
    double risk_cost = 0.0;

    // 对每个行人
    for (const auto& pred : latest_predictions_->predictions) {
      // 1. 计算碰撞概率（CP）
      double cp = computeCollisionProbability(
        data.trajectories[i], pred);

      // 2. 如果高于阈值，使用CVaR评估
      if (cp > cp_threshold_) {
        double cvar = computeCVaR(data.trajectories[i], pred);
        risk_cost += cvar_weight_ * cvar;
      } else {
        risk_cost += cp_weight_ * cp;
      }
    }

    // 添加到总成本
    data.costs[i] += cost_weight_ * risk_cost;
  }
}
```

---

### 2.5 saye_prediction - 行人预测系统

**作用：** 基于GMM的行人轨迹预测

#### Python节点

**ground_truth_predictor.py（真值预测器）**
```python
类名：GroundTruthPredictor
订阅：/world/*/pose/info (tf2_msgs/TFMessage)
发布：/pedestrian_predictions (saye_msgs/PedestrianPredictions)

算法：恒速(CV)模型 + 高斯不确定性

工作流程：
1. 从Gazebo接收Actor位姿流
2. 通过Z坐标识别行人（Z ≈ 0.3m，车辆Z ≈ 0）
3. 维护历史位置队列（5帧）
4. 计算速度：v = (pos_t - pos_{t-1}) / dt
5. 预测56步：
   - 均值：x(t) = x0 + v*t
   - 方差：sigma²(t) = sigma_0² + sigma_q²*t
6. 封装为GMM（当前1个模式）
7. 发布PedestrianPredictions消息

配置参数：
prediction_horizon: 2.8     # 预测时长（秒）
prediction_dt: 0.05         # 时间步（秒）
actor_names: ['walker_center', 'walker_east']
history_size: 5             # 速度估计历史帧数
sigma_0: 0.1                # 初始位置不确定性（米）
sigma_q: 0.5                # 过程噪声强度
publish_rate: 10.0          # 发布频率（Hz）
```

**算法特点：**
- 使用真值（ground truth）而非感知
- 适合算法验证和对比
- 可扩展为IMM（交互式多模型）以支持多模态

**未来扩展方向：**
- 多模态GMM（K=3）：直行、左转、右转
- IMM模型：CV + CA（恒加速度）+ CT（恒转向）
- 基于历史轨迹的意图推断

---

### 2.6 saye_bringup 和 saye_bringup_my

**作用：** 仿真启动和配置管理

**两个包的区别：**

| 特性 | saye_bringup | saye_bringup_my |
|-----|--------------|-----------------|
| 车辆模型 | model.sdf (3D激光) | model_2d_lidar.sdf (2D激光) |
| 世界参数 | 固定世界文件 | 可通过launch参数选择 |
| 使用场景 | 原始仿真 | MPPI实验（推荐）|
| Actor测试 | 无专用launch | 包含actor_pose_test.launch.py |

#### 关键Launch文件

**saye_spawn.launch.py（主仿真启动）**
```python
def generate_launch_description():
    # 参数
    world_arg = DeclareLaunchArgument(
        'world',
        default_value='saye_dynamic_corridor.sdf',
        description='世界文件名称'
    )

    # 节点
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gz_sim_pkg, 'launch', 'gz_sim.launch.py')
        ),
        launch_arguments={'gz_args': world_file}.items()
    )

    robot_state_publisher = Node(...)  # 发布TF树
    ros_gz_bridge = Node(...)          # Gazebo↔ROS 2桥接
    spawn_robot = Node(...)            # 延迟3秒生成车辆
    rviz = Node(...)                   # 延迟5秒启动RViz

    return LaunchDescription([...])
```

**使用方法：**
```bash
# 默认世界（动态走廊）
ros2 launch saye_bringup_my saye_spawn.launch.py

# 指定世界
ros2 launch saye_bringup_my saye_spawn.launch.py \
    world:=scenario2_8ped_mixed.sdf

# 无RViz
ros2 launch saye_bringup_my saye_spawn.launch.py \
    rviz:=false
```

---

## 三、系统数据流分析

### 3.1 完整数据流图

```
Gazebo仿真 (1000 Hz)
    ├─ 车辆传感器
    │   ├─ 里程计 → /odom (20 Hz)
    │   ├─ IMU → /imu (100 Hz)
    │   ├─ 激光 → /scan (10 Hz)
    │   └─ 摄像头 → /camera/* (30 Hz)
    │
    └─ Actor位姿 → ActorPosePublisherSystem (20 Hz)
            ↓
        /world/*/pose/info (Pose_V)
            ↓
    ┌───────────────────────────────┐
    │ ground_truth_predictor.py     │ (10 Hz)
    │ • 提取行人位置                  │
    │ • 计算速度（历史差分）            │
    │ • 生成GMM预测（56步，2.8秒）     │
    └───────────────────────────────┘
            ↓
        /pedestrian_predictions
            ↓
    ┌───────────────────────────────┐
    │ RiskAwareCritic (插件)         │ (20 Hz)
    │ • 订阅预测消息                  │
    │ • [Phase 1] 日志验证            │
    │ • [Phase 2] 计算碰撞概率成本     │
    └───────────────────────────────┘
            ↓
        轨迹成本 (Trajectory Costs)
            ↓
    ┌───────────────────────────────┐
    │ MPPI Controller                │ (20 Hz)
    │ • 生成2000条候选轨迹             │
    │ • 所有评价器评分                 │
    │ • 选择最优轨迹                   │
    └───────────────────────────────┘
            ↓
        cmd_vel (速度命令)
            ↓
    velocity_smoother → collision_monitor → Gazebo
```

### 3.2 控制循环时序分析

```
时刻t=0:
├─ straight_path_commander订阅/odom
├─ 读取当前位置：[0, 0, 0]
├─ 生成直线路径：[0,0] → [0.25,0] → [0.5,0] → ... → [4,0]
└─ 发送FollowPath动作

时刻t=0.05s (50ms):
├─ controller_server接收FollowPath目标
├─ MPPI Controller开始轨迹优化：
│   ├─ 生成2000条随机轨迹（噪声采样）
│   ├─ 对每条轨迹评分：
│   │   ├─ ConstraintCritic: 检查Ackermann约束
│   │   ├─ CostCritic: 查询代价地图（障碍物）
│   │   ├─ GoalCritic: 计算到目标距离
│   │   ├─ PathAlignCritic: 路径对齐度
│   │   ├─ PathFollowCritic: 横向偏差
│   │   ├─ PreferForwardCritic: 前进偏好
│   │   └─ RiskAwareCritic: 行人避障成本
│   │       └─ [读取latest_predictions_]
│   │       └─ [Phase 2: 计算碰撞概率]
│   ├─ 加权求和所有成本
│   ├─ 选择成本最低的轨迹
│   └─ 输出第一步速度命令
└─ 发送到velocity_smoother

时刻t=0.10s:
├─ ground_truth_predictor发布新预测
├─ RiskAwareCritic接收并更新latest_predictions_
└─ MPPI重新规划（使用最新预测）

[循环持续，直到到达目标]
```

---

## 四、关键技术实现

### 4.1 Ackermann转向约束

**运动学模型：**
```
转弯半径 R = L / tan(δ)
L = 轴距
δ = 前轮转角

约束：
R_min = 0.25m  (最小转弯半径)
→ δ_max = atan(L / R_min)
```

**MPPI中的实现：**
```yaml
motion_model: "Ackermann"
AckermannConstraints:
  min_turning_r: 0.25
```

MPPI Controller在生成轨迹时自动应用转弯半径约束。

### 4.2 GMM不确定性建模

**数学模型：**
```
预测分布：p(x_t|x_0,v_0) = Σ w_k · N(μ_k(t), Σ_k(t))

对于CV模型（K=1）：
μ(t) = x_0 + v_0 · t
Σ(t) = Σ_0 + Q · t

其中：
Σ_0 = diag([σ_0², σ_0²])  初始不确定性
Q = diag([σ_q², σ_q²])    过程噪声
```

**实现：**
```python
def predict_position(self, x0, v0, dt, steps):
    means_x = [x0[0] + v0[0] * (i*dt) for i in range(steps)]
    means_y = [x0[1] + v0[1] * (i*dt) for i in range(steps)]

    cov_xx = [self.sigma_0**2 + self.sigma_q**2 * (i*dt)
              for i in range(steps)]
    cov_yy = cov_xx  # 假设各向同性
    cov_xy = [0.0] * steps

    return GMMComponent(
        weight=1.0,
        mean_x=means_x,
        mean_y=means_y,
        cov_xx=cov_xx,
        cov_yy=cov_yy,
        cov_xy=cov_xy
    )
```

### 4.3 Pluginlib动态加载机制

**注册流程：**

1. **定义插件（C++）**
```cpp
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(
  saye_rapa_mppi::RiskAwareCritic,
  mppi::critics::CriticFunction
)
```

2. **XML描述文件（plugins.xml）**
```xml
<library path="libsaye_rapa_mppi">
  <class type="saye_rapa_mppi::RiskAwareCritic"
         base_class_type="mppi::critics::CriticFunction">
    <description>...</description>
  </class>
</library>
```

3. **package.xml导出**
```xml
<export>
  <nav2_mppi_controller plugin="${prefix}/plugins.xml" />
</export>
```

4. **CMakeLists.txt导出**
```cmake
pluginlib_export_plugin_description_file(
  nav2_mppi_controller plugins.xml)
```

5. **YAML配置使用**
```yaml
critics: ["RiskAwareCritic"]  # 简单名称
RiskAwareCritic:
  plugin: "saye_rapa_mppi::RiskAwareCritic"  # 完整类名
```

---

## 五、配置系统架构

### 5.1 配置文件层次

```
顶层Launch参数
    ↓
Launch文件默认值
    ↓
YAML配置文件
    ├─ mppi_nav2_params.yaml (MPPI控制器)
    ├─ path_commander.yaml (路径目标)
    ├─ ground_truth_predictor.yaml (预测器)
    └─ ros_gz_bridge.yaml (传感器桥接)
    ↓
节点运行时参数
```

### 5.2 关键参数对照表

| 参数 | 文件 | 默认值 | 作用 |
|-----|------|--------|------|
| `time_steps` | mppi_nav2_params.yaml | 56 | MPPI预测步数 |
| `model_dt` | mppi_nav2_params.yaml | 0.05 | 时间步长(秒) |
| `batch_size` | mppi_nav2_params.yaml | 2000 | 轨迹采样数 |
| `motion_model` | mppi_nav2_params.yaml | "Ackermann" | 运动模型 |
| `min_turning_r` | mppi_nav2_params.yaml | 0.25 | 最小转弯半径(米) |
| `cost_weight` | mppi_nav2_params.yaml | 1.0 | RiskAwareCritic权重 |
| `prediction_horizon` | ground_truth_predictor.yaml | 2.8 | 预测时长(秒) |
| `sigma_0` | ground_truth_predictor.yaml | 0.1 | 初始不确定性(米) |
| `sigma_q` | ground_truth_predictor.yaml | 0.5 | 过程噪声 |
| `goal` | path_commander.yaml | [4.0,0.0,0.0] | 目标位置 |

**关键对齐：**
- `time_steps * model_dt = 56 * 0.05 = 2.8秒`
- `prediction_horizon = 2.8秒`
- 两者必须一致，确保预测覆盖MPPI规划窗口

---

## 六、已知问题与解决方案

### 问题1：RiskAwareCritic插件配置错误

**问题描述：**
```
[lifecycle_manager-6] [ERROR] Failed to change state for node: controller_server
```

**根本原因：**
- critics列表使用完整类名：`"saye_rapa_mppi::RiskAwareCritic"`
- 配置块缺少 `plugin` 字段

**解决方案：**
```yaml
# 正确配置
critics: ["RiskAwareCritic"]  # 使用简单名称
RiskAwareCritic:
  plugin: "saye_rapa_mppi::RiskAwareCritic"  # 添加plugin字段
  enabled: True
  ...
```

**修复状态：** ✓ 已修复（提交a0a8ce6）

### 问题2：PedestrianManagerSystem插件不生成Actor

**问题描述：**
- 插件编译成功
- 世界文件加载插件
- Actor不出现在仿真中

**临时解决方案：**
使用内联`<actor>`标签（`saye_dynamic_corridor.sdf`）

**状态：** 已有可用替代方案

### 问题3：摄像头话题默认未桥接

**问题描述：**
只有前置摄像头默认桥接到ROS 2

**解决方案：**
编辑 `config/ros_gz_bridge.yaml`，取消注释左/右/后摄像头

**状态：** 已文档化

---

## 七、开发工作流

### 7.1 构建流程

```bash
# 完整构建
cd ~/ackermann_sim
colcon build
source install/setup.bash

# 单包构建（快速迭代）
colcon build --packages-select saye_rapa_mppi
source install/setup.bash

# Python符号链接安装（修改.py无需重新构建）
colcon build --symlink-install
source install/setup.bash
```

### 7.2 测试流程

**1. 仿真测试（3终端）**
```bash
# Terminal 1: 启动Gazebo仿真
ros2 launch saye_bringup_my saye_spawn.launch.py \
    world:=saye_dynamic_corridor.sdf

# Terminal 2: 启动MPPI控制器
ros2 launch saye_mppi mppi_experiment.launch.py

# Terminal 3: (可选) 启动预测系统
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

**2. 诊断脚本**
```bash
./diagnose_mppi.sh  # 检查插件配置
./test_mppi_fix.sh  # 快速重新构建和测试
```

### 7.3 调试方法

**查看话题：**
```bash
ros2 topic list
ros2 topic echo /pedestrian_predictions
ros2 topic hz /odom
```

**查看节点：**
```bash
ros2 node list
ros2 node info /controller_server
```

**查看插件：**
```bash
ros2 plugin list nav2_mppi_controller
```

**查看生命周期状态：**
```bash
ros2 lifecycle get /controller_server
```

---

## 八、未来扩展方向

### 8.1 Phase 2: RiskAwareCritic成本计算

**任务清单：**
1. 实现碰撞概率计算
   - 轨迹点与GMM预测的距离
   - 马氏距离考虑不确定性
2. 实现CVaR风险评估
   - 蒙特卡洛采样
   - 尾部风险度量
3. 参数调优
   - `cost_weight`（风险权重）
   - `collision_radius`（碰撞半径）

### 8.2 多模态预测

**当前：** CV模型（1个GMM模式）
**目标：** IMM模型（3个模式）

```
模式1：CV（直行） - weight: 0.6
模式2：CT_left（左转） - weight: 0.2
模式3：CT_right（右转） - weight: 0.2
```

### 8.3 感知集成

**当前：** 使用Gazebo真值
**目标：** 集成视觉/激光感知

```
激光雷达 → 聚类 → 跟踪 → 预测
摄像头 → 检测 → 关联 → 预测
```

### 8.4 复杂场景测试

**已有场景：**
- scenario1-5（4-12个行人）

**待添加：**
- 交叉路口
- T型路口
- 环岛
- 动态车辆

---

## 九、总结

### 9.1 架构优势

1. **模块化设计**
   - 每个包职责单一
   - 易于独立测试和替换

2. **可扩展性**
   - 插件架构支持自定义评价器
   - 预测系统支持多种模型

3. **可配置性**
   - YAML配置文件集中管理参数
   - Launch参数支持不同场景

4. **实验友好**
   - 绕过AMCL和全局规划器
   - 快速迭代MPPI参数

### 9.2 关键技术点

1. **Pluginlib机制**：实现Nav2 MPPI评价器扩展
2. **GMM不确定性**：表示多模态预测和不确定性
3. **Ackermann约束**：正确建模转向受限车辆
4. **时间窗口对齐**：预测与规划时间一致

### 9.3 当前状态

**Phase 1完成度：** 100%
- ✓ 框架搭建
- ✓ 插件加载
- ✓ 消息订阅
- ✓ 配置修复

**Phase 2准备度：** 90%
- ✓ 预测系统运行
- ✓ 数据流通畅
- ✓ 参数配置就绪
- ✗ 成本计算算法（待实现）

---

## 十、快速参考

### 常用命令

```bash
# 构建
colcon build --packages-select <pkg>

# 启动仿真
ros2 launch saye_bringup_my saye_spawn.launch.py

# 启动MPPI
ros2 launch saye_mppi mppi_experiment.launch.py

# 查看预测
ros2 topic echo /pedestrian_predictions

# 诊断插件
./diagnose_mppi.sh
```

### 关键文件路径

```
配置：
src/ackermann-vehicle-gzsim-ros2/saye_mppi/config/mppi_nav2_params.yaml

插件：
src/ackermann-vehicle-gzsim-ros2/saye_rapa_mppi/src/risk_aware_critic.cpp

世界：
src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor.sdf

预测：
src/ackermann-vehicle-gzsim-ros2/saye_prediction/saye_prediction/ground_truth_predictor.py
```

### 重要话题

```
/odom                        # 里程计
/scan                        # 激光雷达
/pedestrian_predictions      # 行人预测
/follow_path                 # MPPI动作服务器
/cmd_vel                     # 速度命令
/world/*/pose/info           # Actor位姿
```

---

**文档版本：** 1.0
**生成日期：** 2025-11-06
**对应代码版本：** commit 21651f4
