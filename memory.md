# 项目基础
## 当前项目基于开源项目，地址为：https://github.com/alitekes1/ackermann-vehicle-gzsim-ros2
## Features  功能特性
1. Ackermann Steering  1. 阿克曼转向系统
A custom vehicle model built with realistic Ackermann steering dynamics for accurate maneuverability.
采用真实阿克曼转向动力学构建的定制车辆模型，实现精准操控性能。
2. ROS2 Communication  2. ROS2 通信
All sensor data and control signals are fully integrated into the ROS2 ecosystem for seamless interoperability.
所有传感器数据和控制信号均完全集成至 ROS2 生态系统，实现无缝互操作性。
3. Sensors  3. 传感器
IMU: Provides orientation and angular velocity.
IMU：提供姿态角与角速度数据。
Odometry: Ensures accurate vehicle state feedback.
里程计：确保车辆状态反馈的准确性。
LiDAR: Mounted for obstacle detection and environmental scanning.
激光雷达：用于障碍物检测和环境扫描。
Cameras:   摄像头：
Front-facing  前置摄像头
Rear-facing  后向
Left-side  左侧
Right-side  右侧
Note: By default, only the front camera is bridged to ROS 2.If you want to use all cameras (left, right, rear) in ROS 2,remove the # at the beginning of the relevant camera sections in saye_bringup/config/ros_gz_bridge.yaml to activate them (e.g., /camera/left_raw, /camera/right_raw, /camera/rear_raw).
注意：默认情况下，仅前向摄像头桥接到 ROS 2。若需在 ROS 2 中使用所有摄像头（左、右、后），请移除 saye_bringup/config/ros_gz_bridge.yaml 中对应摄像头配置项开头的 # 以启用它们（例如 /camera/left_raw 、 /camera/right_raw 、 /camera/rear_raw ）。

4. Navigation  4. 导航
Integrated with the Nav2 stack for autonomous navigation.
集成 Nav2 堆栈以实现自主导航。
AMCL (Adaptive Monte Carlo Localization) for improved positional accuracy.
采用自适应蒙特卡洛定位（AMCL）技术提升位置精度。
SLAM techniques implemented for real-time mapping and understanding of the environment.
应用即时定位与地图构建（SLAM）技术实现环境实时测绘与感知。
Fine-tuned parameters for optimized navigation performance.
针对优化导航性能的微调参数。
5. Manual Control (with external joystick)
5. 手动控制（使用外部操纵杆）
Added support for joystick-based manual control in the simulation environment, enabling users to test vehicle movement interactively.
在仿真环境中新增基于操纵杆的手动控制功能，使用户能够以交互方式测试车辆运动。
6. Visualization  6. 可视化
Full model and sensor data visualization in RViz2, providing insights into robot states and environmental feedback.
完整模型与传感器数据在 RViz2 中可视化，提供机器人状态与环境反馈的直观展示
## 系统环境为：在windows，wsl2中，ubuntu24，ros2，jazzy。gazebo版本为Gazebo Harmonic
# 项目目标
## 当前需要基于这个项目测试并优化局部规划器mppi
- 需要修改局部规划器的代码进行优化
- 主要验证动态场景下的mppi
- 需要创建修改word，增加行人或者其他的移动障碍物
- 做好动态场景下rviz的可视化，体现优化的效果
- 排除全局规划器和定位的影响
## 开源项目原始使用功能
1. 基础仿真与手动控制
Launch the simulation:   启动仿真：
ros2 launch saye_bringup saye_spawn.launch.py
Control car:   控制车辆：
ros2 run teleop_twist_keyboard teleop_twist_keyboard
2. SLAM (Simultaneous Localization and Mapping)
2. 同步定位与地图构建（SLAM）
To run SLAM Toolbox for mapping, launch the following after starting the simulation:
要运行 SLAM 工具箱进行建图，请在启动仿真后执行以下命令：
ros2 launch saye_bringup slam.launch.py
3. Navigation with Nav2  3. 使用 Nav2 进行导航
To run the simulation with the Nav2 stack for autonomous navigation, launch the following after starting the simulation:
要使用 Nav2 堆栈进行自主导航运行模拟，请在启动模拟后运行以下命令：
ros2 launch saye_bringup navigation_bringup.launch.py

# 当前进展
- 确立使用followpath的路径代替全局规划
- 自己制作世界并且建图，保存地图
- 使用gazebo actor创建行人

## 具体进展


## 地图及行人设置

    ros2 launch saye_bringup_my saye_spawn.launch.py world:=corridor_empty.sdf

    # 场景1（4个行人）
    ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf

    # 场景2（8个行人）
    ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario2_8ped_mixed.sdf

    # 场景3（12个行人）
    ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario3_12ped_crowded.sdf

    # 场景4（6个静态行人）
    ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario4_6ped_static.sdf

    # 场景5（6个随机行人）
    ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario5_6ped_random.sdf

    ## mppi修改模块

 基于对1.md文档、当前代码和最新研究的分析，以下是完整的技术方案：

  ---
  📊 差距分析：当前实现 vs RAPA-MPPI框架

  | 模块      | 当前实现                     | RAPA-MPPI需求                 | 差距  |
  |---------|--------------------------|-----------------------------|-----|
  | 动态障碍物预测 | ❌ 无预测，静态costmap          | ✅ GMM多模态概率预测 (公式1)          | 需新增 |
  | 风险评估    | ⚠️ CostCritic（基于costmap） | ✅ CP快速剪枝 + CVaR精确评估 (公式2-5) | 需重构 |
  | 采样策略    | ⚠️ 单模态高斯噪声               | ✅ 多辅助控制器（Guide + Brake）     | 需扩展 |
  | 碰撞检测    | ❌ 几何碰撞（costmap查询）        | ✅ 概率碰撞（Monte Carlo近似）       | 需新增 |
  | 优化目标    | ⚠️ 期望成本最小化               | ✅ CVaR条件尾部风险最小化             | 需修改 |
  | 计算效率    | ✅ 2000样本@20Hz            | ✅ 400样本+并行MC@20Hz           | 可优化 |

  ---
  🎯 核心技术实施方案

  1. GMM预测模块 (Prediction Layer)

  架构设计：
  PedestrianTracker (ROS2节点)
  ├─ IMM滤波器 (Interactive Multiple Model)
  │  ├─ 模型1: 恒速直线运动 (CV)
  │  ├─ 模型2: 恒加速运动 (CA)
  │  └─ 模型3: 转弯运动 (CT)
  ├─ GMM生成器
  │  └─ 输出: {w_k, μ_k(t), Σ_k(t)} for t∈[0, T]
  └─ 发布话题: /pedestrian_predictions (自定义消息类型)

  实施方式：
  - 输入: Gazebo actor真实位置 (/model/{actor_name}/pose) 或雷达检测
  - 输出: PedestrianPredictions.msg 包含每个行人的GMM参数
  // 自定义消息
  PedestrianPredictions.msg:
    Header header
    PedestrianPrediction[] predictions

  PedestrianPrediction.msg:
    string id
    GMMComponent[] gmm_modes  // K个高斯模式

  GMMComponent.msg:
    float64 weight            // w_k
    float64[] mean_x          // μ_k(t) for t∈[0,T]
    float64[] mean_y
    float64[] cov_xx          // Σ_k(t)
    float64[] cov_yy
    float64[] cov_xy

  关键算法 (基于DRA-MPPI论文):
  # 简化版恒速预测 + 不确定性传播
  for t in range(T):
      μ(t) = μ(0) + v * t
      Σ(t) = Σ(0) + Q * t²  # 过程噪声累积

  ---
  2. CP-CVaR混合风险评估模块

  阶段1: Monte Carlo碰撞概率 (基于DRA-MPPI算法12)

  // 伪代码
  float compute_collision_probability(
      Trajectory& traj_k,
      GMM& pedestrian_gmm,
      int N_mc = 100)
  {
      // 定义采样区域 R_t
      auto bbox = get_bounding_box(traj_k, collision_radius);

      // 并行评估MC样本
      float cp_sum = 0.0;
      int samples_in_region = 0;

      for (int j = 0; j < N_mc; j++) {
          // 均匀采样
          Point sample = uniform_sample(bbox);

          // 计算联合概率 P_joint = 1 - Π(1 - p_o)
          float p_joint = compute_joint_gmm_pdf(sample, pedestrian_gmm);

          // 检查是否在碰撞圆内
          if (is_in_collision_circle(sample, traj_k, collision_radius)) {
              cp_sum += p_joint;
              samples_in_region++;
          }
      }

      // 公式12的近似
      return (M_PI * r² / samples_in_region) * cp_sum;
  }

  阶段2: CVaR评估 (公式4)

  float compute_cvar(
      Trajectory& traj_k,
      GMM& pedestrian_gmm,
      float alpha = 0.1,  // 尾部比例
      int N_samples = 50)
  {
      vector<float> cost_samples;

      // 嵌套蒙特卡洛：从GMM采样多个行人轨迹
      for (int n = 0; n < N_samples; n++) {
          // 从GMM采样一个可能的行人轨迹
          auto ped_traj = sample_from_gmm(pedestrian_gmm);

          // 计算该采样下的成本
          float cost = evaluate_cost(traj_k, ped_traj);
          cost_samples.push_back(cost);
      }

      // 计算α-CVaR
      sort(cost_samples.begin(), cost_samples.end());
      int var_idx = (int)(alpha * N_samples);
      float var_alpha = cost_samples[var_idx];

      // CVaR = 超过VaR的成本均值
      float cvar = 0.0;
      int count = 0;
      for (float c : cost_samples) {
          if (c >= var_alpha) {
              cvar += c;
              count++;
          }
      }
      return cvar / count;
  }

  集成到Nav2 MPPI (新的Critic)

  // 创建新的critic: RiskAwareCritic
  class RiskAwareCritic : public nav2_mppi_controller::critics::CriticFunction
  {
  public:
      void score(CriticData& data) override {
          // 阶段1: CP剪枝
          for (int k = 0; k < data.trajectories.x.shape(0); k++) {
              float cp = compute_collision_probability(
                  data.trajectories[k],
                  pedestrian_predictions_);

              if (cp > sigma_prune_) {
                  data.costs[k] += P_prune_;  // 大惩罚
              } else {
                  // 阶段2: 精英轨迹CVaR评估
                  float cvar = compute_cvar(
                      data.trajectories[k],
                      pedestrian_predictions_,
                      alpha_);

                  if (cvar > C_u_) {
                      data.costs[k] += A_ * cvar;  // 公式5
                  }
              }
          }
      }

  private:
      PedestrianPredictions pedestrian_predictions_;
      float sigma_prune_ = 0.3;   // CP阈值
      float P_prune_ = 1e6;       // 剪枝惩罚
      float alpha_ = 0.1;         // CVaR α
      float C_u_ = 50.0;          // 风险预算
      float A_ = 100.0;           // CVaR权重
  };

  ---
  3. 多辅助控制器采样策略

  实施方式 (修改noise_generator)

  // 在 nav2_mppi_controller/tools/noise_generator.hpp 中扩展

  class MultiControllerNoiseGenerator {
  public:
      void generate_noises(
          ControlSequence& control_sequence,
          const State& state,
          const Path& path)
      {
          int K_total = batch_size_;  // 2000
          int K_guide = K_total * 0.85;  // 1700个引导样本
          int K_brake = K_total * 0.1;   // 200个刹车样本
          int K_explore = K_total * 0.05; // 100个探索样本

          // 1. Guide控制器样本 (跟踪全局路径)
          ControlSequence u_guide = compute_guide_control(state, path);
          for (int k = 0; k < K_guide; k++) {
              control_sequence[k] = u_guide + gaussian_noise();
          }

          // 2. Brake控制器样本 (安全冗余)
          ControlSequence u_brake = {0.0, 0.0, 0.0};  // 零速度
          for (int k = K_guide; k < K_guide + K_brake; k++) {
              control_sequence[k] = u_brake;
          }

          // 3. 探索性样本 (大噪声)
          for (int k = K_guide + K_brake; k < K_total; k++) {
              control_sequence[k] = large_gaussian_noise();
          }
      }

  private:
      ControlSequence compute_guide_control(
          const State& state,
          const Path& path)
      {
          // 使用Pure Pursuit或Stanley控制器生成引导轨迹
          // 这是一个确定性的、合理的控制序列
          return pure_pursuit_controller_.computeControl(state, path);
      }
  };

  ---
  🔧 实施方案评估

  方案A: 自定义Nav2 MPPI插件 (推荐)

  优点:
  - ✅ 复用Nav2基础设施（costmap, lifecycle, behavior tree）
  - ✅ 模块化：只需新增RiskAwareCritic
  - ✅ 易于集成和调试

  缺点:
  - ⚠️ 受限于Nav2 MPPI架构
  - ⚠️ 需修改采样策略（noise_generator）

  实施步骤:
  1. 创建 rapa_mppi_critics 包，继承 CriticFunction
  2. 创建 pedestrian_tracker 节点发布GMM预测
  3. 修改 mppi_nav2_params.yaml 添加新critic
  4. （可选）Fork nav2_mppi_controller 修改noise_generator

  ---
  方案B: 独立RAPA-MPPI节点

  优点:
  - ✅ 完全自主设计，无Nav2限制
  - ✅ 可GPU加速（CUDA实现）
  - ✅ 灵活的采样和评估策略

  缺点:
  - ❌ 需重新实现基础设施
  - ❌ 开发工作量大

  ---
  方案C: 混合方案 (最优)

  架构:
  pedestrian_tracker (独立节点)
    └─> /pedestrian_predictions

  RiskAwareCritic (Nav2插件)
    ├─ 订阅 /pedestrian_predictions
    ├─ CP计算模块 (C++/GPU)
    └─ CVaR评估模块 (C++)

  MultiControllerSampler (Fork nav2_mppi)
    ├─ Guide控制器
    └─ Brake控制器

  开发顺序:
  1. Phase 1 (2-3周): pedestrian_tracker + 简单CP评估
  2. Phase 2 (2-3周): RiskAwareCritic集成到Nav2 MPPI
  3. Phase 3 (2周): CVaR模块 + 多控制器采样
  4. Phase 4 (1周): 性能优化 + GPU加速

  ---
  📈 预期性能提升

  | 指标   | 当前MPPI      | RAPA-MPPI (预期)   |
  |------|-------------|------------------|
  | 碰撞率  | ~15% (拥挤场景) | <2%              |
  | 冻结问题 | 频繁发生        | 基本消除             |
  | 任务时间 | 基准          | +10-15% (更保守)    |
  | 安全性  | 风险中性        | 风险规避 (α=0.1)     |
  | 计算频率 | 20Hz        | 20Hz (优化后可达50Hz) |


##具体gnn预测方案
📦 完整实施方案：双轨并行 + 统一集成

 一、总体架构设计

 ┌─────────────────────────────────────────────────────────────┐
 │                     saye_prediction 包                       │
 ├─────────────────────────────────────────────────────────────┤
 │                                                              │
 │  ┌──────────────────────────┐  ┌─────────────────────────┐  │
 │  │ Node A: 真值预测器        │  │ Node B: Costmap预测器   │  │
 │  │ ground_truth_predictor   │  │ costmap_predictor       │  │
 │  ├──────────────────────────┤  ├─────────────────────────┤  │
 │  │ 输入:                     │  │ 输入:                   │  │
 │  │ - /model/walker_*/pose   │  │ - /local_costmap/       │  │
 │  │   (Gazebo真值)           │  │   costmap_raw           │  │
 │  │                          │  │ - /map (静态地图)       │  │
 │  ├──────────────────────────┤  ├─────────────────────────┤  │
 │  │ 处理:                     │  │ 处理:                   │  │
 │  │ 1. 速度估计(差分)         │  │ 1. Costmap差分         │  │
 │  │ 2. CV模型预测             │  │ 2. 连通域分析          │  │
 │  │                          │  │ 3. 质心追踪             │  │
 │  │                          │  │ 4. CV模型预测           │  │
 │  ├──────────────────────────┤  ├─────────────────────────┤  │
 │  │ 输出:                     │  │ 输出:                   │  │
 │  │ /pedestrian_predictions  │  │ /pedestrian_predictions │  │
 │  │ (PedestrianPredictions)  │  │ (PedestrianPredictions) │  │
 │  └──────────────────────────┘  └─────────────────────────┘  │
 │                  │                          │                │
 │                  └──────────┬───────────────┘                │
 │                             ▼                                │
 │                ┌──────────────────────────┐                  │
 │                │  统一消息格式             │                  │
 │                │  PedestrianPredictions   │                  │
 │                └────────────┬─────────────┘                  │
 └─────────────────────────────┼────────────────────────────────┘
                               │
                               ▼
               ┌───────────────────────────────┐
               │   MPPI RiskAwareCritic        │
               │   (不关心数据来源)             │
               └───────────────────────────────┘
