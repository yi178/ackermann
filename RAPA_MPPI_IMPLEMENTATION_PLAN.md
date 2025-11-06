# RAPA-MPPI 完整实施方案

## 目录
1. [总体架构设计](#总体架构设计)
2. [Phase 1: 基础框架搭建](#phase-1-基础框架搭建)
3. [Phase 2: CP碰撞概率计算](#phase-2-cp碰撞概率计算)
4. [Phase 3: CVaR风险评估](#phase-3-cvar风险评估)
5. [Phase 4: 多辅助控制器](#phase-4-多辅助控制器)
6. [Phase 5: 集成与调优](#phase-5-集成与调优)
7. [测试与验证方案](#测试与验证方案)
8. [参数调优指南](#参数调优指南)

---

## 总体架构设计

### 系统数据流

```
┌─────────────────────────────────────────────────────────────────┐
│                    Gazebo Simulation                            │
│  walker_center, walker_east (Physics-based Models)             │
└─────────────────┬───────────────────────────────────────────────┘
                  │ /model/walker_*/pose
                  ↓
┌─────────────────────────────────────────────────────────────────┐
│              ground_truth_predictor (已实现)                     │
│  • CV模型预测                                                    │
│  • 输出: /pedestrian_predictions (PedestrianPredictions)        │
│    - header, predictions[]                                      │
│    - 每个prediction包含: id, current_pose, gmm_modes[]         │
│    - 每个gmm_mode: weight, mean_x[], mean_y[], cov_xx[], ...   │
└─────────────────┬───────────────────────────────────────────────┘
                  │
                  ↓
┌─────────────────────────────────────────────────────────────────┐
│                   Nav2 MPPI Controller                          │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │         Trajectory Sampling (已有)                         │ │
│  │  • 生成K个候选轨迹 (默认2000个)                            │ │
│  │  • 使用高斯噪声扰动控制序列                                │ │
│  └─────────────────────┬─────────────────────────────────────┘ │
│                        │                                         │
│                        ↓                                         │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │      Critics (评分函数) - 新增 RiskAwareCritic             │ │
│  │  ┌─────────────────────────────────────────────────────┐  │ │
│  │  │ 已有Critics:                                        │  │ │
│  │  │  - GoalCritic (目标跟踪)                            │  │ │
│  │  │  - PathFollowCritic (路径跟踪)                      │  │ │
│  │  │  - ObstaclesCritic (静态障碍物)                     │  │ │
│  │  └─────────────────────────────────────────────────────┘  │ │
│  │  ┌─────────────────────────────────────────────────────┐  │ │
│  │  │ 新增: RiskAwareCritic ★                            │  │ │
│  │  │  ├─ 订阅 /pedestrian_predictions                   │  │ │
│  │  │  ├─ 阶段1: CP碰撞概率计算 (快速剪枝)                │  │ │
│  │  │  │   • 计算每个轨迹的碰撞概率                       │  │ │
│  │  │  │   • CP > σ_prune → 添加大惩罚                   │  │ │
│  │  │  ├─ 阶段2: CVaR尾部风险评估 (精确评估)             │  │ │
│  │  │  │   • 对低CP轨迹进行嵌套MC采样                    │  │ │
│  │  │  │   • 计算α-CVaR值                                │  │ │
│  │  │  └─ 输出: risk_cost[k] for k=1..K                  │  │ │
│  │  └─────────────────────────────────────────────────────┘  │ │
│  └───────────────────┬───────────────────────────────────────┘ │
│                      │                                           │
│                      ↓                                           │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │         Cost Aggregation & Control Update                 │ │
│  │  • total_cost[k] = perf_cost[k] + risk_cost[k]            │ │
│  │  • weight[k] = exp(-total_cost[k] / λ)                    │ │
│  │  • u* = Σ weight[k] * u[k] / Σ weight[k]                  │ │
│  └───────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

### 关键数据结构

```cpp
// 1. 预测数据（来自/pedestrian_predictions）
struct GMMComponent {
    double weight;
    std::vector<double> mean_x;   // 56步预测
    std::vector<double> mean_y;
    std::vector<double> cov_xx;
    std::vector<double> cov_yy;
    std::vector<double> cov_xy;
};

struct PedestrianPrediction {
    std::string id;
    geometry_msgs::msg::Pose current_pose;
    double current_speed;
    std::vector<GMMComponent> gmm_modes;  // CV模型：1个mode
};

// 2. 轨迹表示（Nav2 MPPI内部）
struct Trajectory {
    xt::xtensor<float, 2> x;    // [K, T] - X位置
    xt::xtensor<float, 2> y;    // [K, T] - Y位置
    xt::xtensor<float, 2> yaw;  // [K, T] - 航向角
};

// 3. 风险评估结果
struct RiskAssessment {
    std::vector<float> collision_prob;  // CP[k] for k=1..K
    std::vector<float> cvar_values;     // CVaR[k] for elite set
    std::vector<float> risk_costs;      // final risk cost[k]
};
```

---

## Phase 1: 基础框架搭建

### 1.1 创建新包 `saye_rapa_mppi`

```bash
cd ~/ackermann_sim/src/ackermann-vehicle-gzsim-ros2
ros2 pkg create saye_rapa_mppi \
  --build-type ament_cmake \
  --dependencies \
    rclcpp \
    nav2_mppi_controller \
    nav2_core \
    nav2_costmap_2d \
    saye_msgs \
    geometry_msgs \
    tf2 \
    tf2_geometry_msgs \
    pluginlib
```

### 1.2 包结构设计

```
saye_rapa_mppi/
├── CMakeLists.txt
├── package.xml
├── plugins.xml                      # pluginlib插件清单
├── include/saye_rapa_mppi/
│   ├── risk_aware_critic.hpp        # 主critic类
│   ├── collision_probability.hpp   # CP计算模块
│   ├── cvar_evaluator.hpp          # CVaR评估模块
│   └── utils/
│       ├── geometry_utils.hpp      # 几何工具
│       └── prediction_cache.hpp    # 预测数据缓存
├── src/
│   ├── risk_aware_critic.cpp
│   ├── collision_probability.cpp
│   └── cvar_evaluator.cpp
├── config/
│   └── rapa_mppi_params.yaml      # 参数配置
└── test/
    ├── test_collision_prob.cpp
    └── test_cvar.cpp
```

### 1.3 基础类定义

**文件: `include/saye_rapa_mppi/risk_aware_critic.hpp`**

```cpp
#ifndef SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_
#define SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_

#include <memory>
#include <string>
#include <vector>

#include "nav2_mppi_controller/critic_function.hpp"
#include "nav2_mppi_controller/models/state.hpp"
#include "nav2_mppi_controller/tools/utils.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "saye_msgs/msg/pedestrian_predictions.hpp"

#include "saye_rapa_mppi/collision_probability.hpp"
#include "saye_rapa_mppi/cvar_evaluator.hpp"

namespace saye_rapa_mppi
{

/**
 * @class RiskAwareCritic
 * @brief RAPA-MPPI风险感知评分函数
 *
 * 实现两阶段风险评估:
 * 1. CP碰撞概率快速剪枝
 * 2. CVaR条件风险值精确评估
 */
class RiskAwareCritic : public nav2_mppi_controller::critics::CriticFunction
{
public:
  /**
   * @brief 初始化critic
   */
  void initialize() override;

  /**
   * @brief 对轨迹进行风险评分
   * @param costs 输出参数：每个轨迹的成本
   */
  void score(CriticData & data) override;

protected:
  /**
   * @brief 订阅行人预测数据
   */
  void predictionCallback(
    const saye_msgs::msg::PedestrianPredictions::SharedPtr msg);

  /**
   * @brief 阶段1: 计算碰撞概率并剪枝
   * @return 精英轨迹索引集合
   */
  std::vector<size_t> computeCollisionProbabilities(
    const CriticData & data,
    std::vector<float> & cp_values);

  /**
   * @brief 阶段2: 计算CVaR风险值
   */
  void computeCVaRValues(
    const CriticData & data,
    const std::vector<size_t> & elite_indices,
    std::vector<float> & cvar_values);

  /**
   * @brief 生成最终风险成本
   */
  void generateRiskCosts(
    const std::vector<float> & cp_values,
    const std::vector<float> & cvar_values,
    const std::vector<size_t> & elite_indices,
    xt::xtensor<float, 1> & costs);

private:
  // ROS通信
  rclcpp::Subscription<saye_msgs::msg::PedestrianPredictions>::SharedPtr
    prediction_sub_;
  saye_msgs::msg::PedestrianPredictions::SharedPtr latest_predictions_;
  std::mutex prediction_mutex_;

  // 功能模块
  std::unique_ptr<CollisionProbabilityCalculator> cp_calculator_;
  std::unique_ptr<CVaREvaluator> cvar_evaluator_;

  // 参数 (从配置文件加载)
  struct Params {
    // CP剪枝参数
    double sigma_prune{0.3};      // CP剪枝阈值
    double prune_penalty{1e6};    // 剪枝惩罚值

    // CVaR参数
    double alpha{0.1};            // CVaR尾部比例
    double risk_budget{50.0};     // CVaR风险预算 C_u
    double cvar_weight{100.0};    // CVaR惩罚权重 A
    int cvar_samples{50};         // CVaR采样数

    // 碰撞检测参数
    double collision_radius{0.6}; // 碰撞半径(m)
    int mc_samples{100};          // MC采样数

    // 性能参数
    bool enable_cvar{true};       // 是否启用CVaR阶段
    bool enable_parallel{true};   // 是否并行计算
  } params_;

  // 统计信息
  struct Stats {
    size_t total_trajectories{0};
    size_t pruned_count{0};
    size_t elite_count{0};
    double avg_cp{0.0};
    double avg_cvar{0.0};
  } stats_;

  // 日志频率控制
  size_t iteration_count_{0};
  static constexpr size_t LOG_THROTTLE = 50;
};

}  // namespace saye_rapa_mppi

#endif  // SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_
```

---

## Phase 2: CP碰撞概率计算

### 2.1 算法原理（基于1.md公式）

根据DRA-MPPI算法12，碰撞概率通过蒙特卡洛近似：

```
CP(τ_k) ≈ (V_R / N_in) × Σ P_GMM(s_j)
```

其中：
- `V_R`: 采样区域体积（轨迹周围的包围盒）
- `N_in`: 落在碰撞半径内的样本数
- `P_GMM(s_j)`: 样本点在GMM分布下的概率密度

### 2.2 实现代码

**文件: `include/saye_rapa_mppi/collision_probability.hpp`**

```cpp
#ifndef SAYE_RAPA_MPPI__COLLISION_PROBABILITY_HPP_
#define SAYE_RAPA_MPPI__COLLISION_PROBABILITY_HPP_

#include <vector>
#include <random>
#include <cmath>
#include "saye_msgs/msg/pedestrian_predictions.hpp"
#include "geometry_msgs/msg/point.hpp"

namespace saye_rapa_mppi
{

/**
 * @brief 碰撞概率计算器
 *
 * 基于蒙特卡洛方法近似计算轨迹与GMM预测障碍物的碰撞概率
 */
class CollisionProbabilityCalculator
{
public:
  struct Config {
    double collision_radius{0.6};  // 碰撞半径(m)
    int mc_samples{100};           // MC采样数
    double sampling_margin{1.0};   // 采样区域边界扩展(m)
  };

  explicit CollisionProbabilityCalculator(const Config & config)
  : config_(config),
    rng_(std::random_device{}())
  {}

  /**
   * @brief 计算单条轨迹的最大碰撞概率
   * @param trajectory_x X坐标序列 [T]
   * @param trajectory_y Y坐标序列 [T]
   * @param predictions 行人预测数据
   * @return 最大碰撞概率 max_t CP(t)
   */
  double computeMaxCollisionProbability(
    const std::vector<float> & trajectory_x,
    const std::vector<float> & trajectory_y,
    const saye_msgs::msg::PedestrianPredictions & predictions);

  /**
   * @brief 批量计算多条轨迹的CP值（可并行）
   * @param trajectories_x [K, T]
   * @param trajectories_y [K, T]
   * @param predictions 行人预测数据
   * @return CP值向量 [K]
   */
  std::vector<float> computeBatchCollisionProbabilities(
    const std::vector<std::vector<float>> & trajectories_x,
    const std::vector<std::vector<float>> & trajectories_y,
    const saye_msgs::msg::PedestrianPredictions & predictions,
    bool parallel = true);

private:
  /**
   * @brief 计算某时刻轨迹点的瞬时碰撞概率
   */
  double computeInstantaneousCP(
    float x_robot, float y_robot,
    const saye_msgs::msg::PedestrianPredictions & predictions,
    size_t time_step);

  /**
   * @brief 计算GMM在某点的联合概率密度
   * P_joint = 1 - Π(1 - P_pedestrian_i)
   */
  double evaluateJointGMMPDF(
    double x, double y,
    const saye_msgs::msg::PedestrianPredictions & predictions,
    size_t time_step);

  /**
   * @brief 计算单个高斯分量的概率密度
   * N(x; μ, Σ)
   */
  double evaluateGaussianPDF(
    double x, double y,
    double mean_x, double mean_y,
    double var_x, double var_y,
    double cov_xy = 0.0);

  /**
   * @brief 生成采样区域的包围盒
   */
  struct BoundingBox {
    double x_min, x_max, y_min, y_max;
    double volume() const { return (x_max - x_min) * (y_max - y_min); }
  };

  BoundingBox computeBoundingBox(
    float x_robot, float y_robot,
    const saye_msgs::msg::PedestrianPredictions & predictions,
    size_t time_step);

  /**
   * @brief 在包围盒内均匀采样点
   */
  std::vector<std::pair<double, double>> samplePointsInBox(
    const BoundingBox & box, int num_samples);

  /**
   * @brief 检查点是否在碰撞圆内
   */
  bool isInCollisionRadius(
    double x, double y,
    float x_robot, float y_robot);

  Config config_;
  std::mt19937 rng_;
  std::uniform_real_distribution<double> uniform_dist_{0.0, 1.0};
};

}  // namespace saye_rapa_mppi

#endif  // SAYE_RAPA_MPPI__COLLISION_PROBABILITY_HPP_
```

**文件: `src/collision_probability.cpp`**

```cpp
#include "saye_rapa_mppi/collision_probability.hpp"
#include <algorithm>
#include <execution>
#include <cmath>

namespace saye_rapa_mppi
{

double CollisionProbabilityCalculator::computeMaxCollisionProbability(
  const std::vector<float> & trajectory_x,
  const std::vector<float> & trajectory_y,
  const saye_msgs::msg::PedestrianPredictions & predictions)
{
  if (predictions.predictions.empty()) {
    return 0.0;
  }

  const size_t T = trajectory_x.size();
  double max_cp = 0.0;

  // 遍历轨迹的每个时间步
  for (size_t t = 0; t < T; ++t) {
    double cp_t = computeInstantaneousCP(
      trajectory_x[t], trajectory_y[t],
      predictions, t);
    max_cp = std::max(max_cp, cp_t);
  }

  return max_cp;
}

std::vector<float> CollisionProbabilityCalculator::computeBatchCollisionProbabilities(
  const std::vector<std::vector<float>> & trajectories_x,
  const std::vector<std::vector<float>> & trajectories_y,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  bool parallel)
{
  const size_t K = trajectories_x.size();
  std::vector<float> cp_values(K);

  if (parallel) {
    // C++17并行算法
    std::vector<size_t> indices(K);
    std::iota(indices.begin(), indices.end(), 0);

    std::for_each(std::execution::par_unseq, indices.begin(), indices.end(),
      [&](size_t k) {
        cp_values[k] = computeMaxCollisionProbability(
          trajectories_x[k], trajectories_y[k], predictions);
      });
  } else {
    for (size_t k = 0; k < K; ++k) {
      cp_values[k] = computeMaxCollisionProbability(
        trajectories_x[k], trajectories_y[k], predictions);
    }
  }

  return cp_values;
}

double CollisionProbabilityCalculator::computeInstantaneousCP(
  float x_robot, float y_robot,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  size_t time_step)
{
  // 1. 计算采样区域（包围盒）
  BoundingBox bbox = computeBoundingBox(x_robot, y_robot, predictions, time_step);

  // 2. 在包围盒内均匀采样
  auto samples = samplePointsInBox(bbox, config_.mc_samples);

  // 3. 蒙特卡洛积分
  double cp_sum = 0.0;
  int samples_in_collision = 0;

  for (const auto & [x, y] : samples) {
    // 检查是否在碰撞半径内
    if (!isInCollisionRadius(x, y, x_robot, y_robot)) {
      continue;
    }

    samples_in_collision++;

    // 计算该点的GMM联合概率密度
    double p_joint = evaluateJointGMMPDF(x, y, predictions, time_step);
    cp_sum += p_joint;
  }

  // 4. 计算碰撞概率（公式12）
  if (samples_in_collision == 0) {
    return 0.0;
  }

  double collision_area = M_PI * config_.collision_radius * config_.collision_radius;
  double cp = (collision_area / samples_in_collision) * cp_sum;

  return std::min(cp, 1.0);  // 限制在[0,1]
}

double CollisionProbabilityCalculator::evaluateJointGMMPDF(
  double x, double y,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  size_t time_step)
{
  // 联合概率: P_joint = 1 - Π(1 - P_i)
  // 即：至少与一个行人碰撞的概率
  double p_no_collision = 1.0;

  for (const auto & pred : predictions.predictions) {
    if (pred.gmm_modes.empty() || time_step >= pred.gmm_modes[0].mean_x.size()) {
      continue;
    }

    // 累积所有GMM mode的概率（CV模型只有1个mode）
    double p_ped = 0.0;
    for (const auto & mode : pred.gmm_modes) {
      double p_mode = mode.weight * evaluateGaussianPDF(
        x, y,
        mode.mean_x[time_step],
        mode.mean_y[time_step],
        mode.cov_xx[time_step],
        mode.cov_yy[time_step],
        mode.cov_xy[time_step]
      );
      p_ped += p_mode;
    }

    p_no_collision *= (1.0 - p_ped);
  }

  return 1.0 - p_no_collision;
}

double CollisionProbabilityCalculator::evaluateGaussianPDF(
  double x, double y,
  double mean_x, double mean_y,
  double var_x, double var_y,
  double cov_xy)
{
  // 二维高斯分布: N(x; μ, Σ)
  double dx = x - mean_x;
  double dy = y - mean_y;

  // 协方差矩阵的行列式
  double det = var_x * var_y - cov_xy * cov_xy;
  if (det <= 1e-10) {
    return 0.0;  // 退化情况
  }

  // Mahalanobis距离
  double inv_var_x = var_y / det;
  double inv_var_y = var_x / det;
  double inv_cov_xy = -cov_xy / det;

  double mahalanobis = dx * dx * inv_var_x +
                       dy * dy * inv_var_y +
                       2.0 * dx * dy * inv_cov_xy;

  // 概率密度
  double pdf = (1.0 / (2.0 * M_PI * std::sqrt(det))) *
               std::exp(-0.5 * mahalanobis);

  return pdf;
}

CollisionProbabilityCalculator::BoundingBox
CollisionProbabilityCalculator::computeBoundingBox(
  float x_robot, float y_robot,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  size_t time_step)
{
  // 初始化为机器人位置周围的碰撞半径
  BoundingBox box;
  box.x_min = x_robot - config_.collision_radius;
  box.x_max = x_robot + config_.collision_radius;
  box.y_min = y_robot - config_.collision_radius;
  box.y_max = y_robot + config_.collision_radius;

  // 扩展到包含所有可能与机器人碰撞的行人位置
  for (const auto & pred : predictions.predictions) {
    if (pred.gmm_modes.empty() || time_step >= pred.gmm_modes[0].mean_x.size()) {
      continue;
    }

    for (const auto & mode : pred.gmm_modes) {
      double mean_x = mode.mean_x[time_step];
      double mean_y = mode.mean_y[time_step];
      double std_x = std::sqrt(mode.cov_xx[time_step]);
      double std_y = std::sqrt(mode.cov_yy[time_step]);

      // 3σ原则 + 采样边界
      double margin = 3.0 * std::max(std_x, std_y) + config_.sampling_margin;

      box.x_min = std::min(box.x_min, mean_x - margin);
      box.x_max = std::max(box.x_max, mean_x + margin);
      box.y_min = std::min(box.y_min, mean_y - margin);
      box.y_max = std::max(box.y_max, mean_y + margin);
    }
  }

  return box;
}

std::vector<std::pair<double, double>>
CollisionProbabilityCalculator::samplePointsInBox(
  const BoundingBox & box, int num_samples)
{
  std::vector<std::pair<double, double>> samples;
  samples.reserve(num_samples);

  for (int i = 0; i < num_samples; ++i) {
    double x = box.x_min + uniform_dist_(rng_) * (box.x_max - box.x_min);
    double y = box.y_min + uniform_dist_(rng_) * (box.y_max - box.y_min);
    samples.emplace_back(x, y);
  }

  return samples;
}

bool CollisionProbabilityCalculator::isInCollisionRadius(
  double x, double y,
  float x_robot, float y_robot)
{
  double dx = x - x_robot;
  double dy = y - y_robot;
  double dist_sq = dx * dx + dy * dy;
  return dist_sq <= (config_.collision_radius * config_.collision_radius);
}

}  // namespace saye_rapa_mppi
```

### 2.3 单元测试

**文件: `test/test_collision_prob.cpp`**

```cpp
#include <gtest/gtest.h>
#include "saye_rapa_mppi/collision_probability.hpp"

using namespace saye_rapa_mppi;

TEST(CollisionProbabilityTest, NoPedestrians) {
  CollisionProbabilityCalculator::Config config;
  CollisionProbabilityCalculator calc(config);

  std::vector<float> traj_x = {0.0, 0.5, 1.0};
  std::vector<float> traj_y = {0.0, 0.0, 0.0};
  saye_msgs::msg::PedestrianPredictions predictions;

  double cp = calc.computeMaxCollisionProbability(traj_x, traj_y, predictions);
  EXPECT_DOUBLE_EQ(cp, 0.0);
}

TEST(CollisionProbabilityTest, DirectCollision) {
  CollisionProbabilityCalculator::Config config;
  config.collision_radius = 0.5;
  config.mc_samples = 200;
  CollisionProbabilityCalculator calc(config);

  // 机器人轨迹：直线前进
  std::vector<float> traj_x = {0.0, 1.0, 2.0, 3.0};
  std::vector<float> traj_y = {0.0, 0.0, 0.0, 0.0};

  // 行人预测：静止在(2.0, 0.0)
  saye_msgs::msg::PedestrianPredictions predictions;
  saye_msgs::msg::PedestrianPrediction ped;
  ped.id = "test_pedestrian";

  saye_msgs::msg::GMMComponent mode;
  mode.weight = 1.0;
  mode.mean_x = {2.0, 2.0, 2.0, 2.0};
  mode.mean_y = {0.0, 0.0, 0.0, 0.0};
  mode.cov_xx = {0.01, 0.01, 0.01, 0.01};  // 低不确定性
  mode.cov_yy = {0.01, 0.01, 0.01, 0.01};
  mode.cov_xy = {0.0, 0.0, 0.0, 0.0};

  ped.gmm_modes.push_back(mode);
  predictions.predictions.push_back(ped);

  double cp = calc.computeMaxCollisionProbability(traj_x, traj_y, predictions);

  // 应该检测到高碰撞概率
  EXPECT_GT(cp, 0.5);
  std::cout << "Collision probability: " << cp << std::endl;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

---

## Phase 3: CVaR风险评估

### 3.1 算法原理

CVaR（Conditional Value at Risk）计算尾部风险：

```
CVaR_α(Z) = E[Z | Z ≥ VaR_α(Z)]
```

实现步骤：
1. 从GMM采样N个行人轨迹 `{ζ_1, ..., ζ_N}`
2. 计算每个采样下的成本 `{z_1, ..., z_N}`
3. 排序并计算 `VaR_α`（第 `α×N` 分位点）
4. 计算超过VaR的成本均值

### 3.2 实现代码

**文件: `include/saye_rapa_mppi/cvar_evaluator.hpp`**

```cpp
#ifndef SAYE_RAPA_MPPI__CVAR_EVALUATOR_HPP_
#define SAYE_RAPA_MPPI__CVAR_EVALUATOR_HPP_

#include <vector>
#include <random>
#include "saye_msgs/msg/pedestrian_predictions.hpp"

namespace saye_rapa_mppi
{

/**
 * @brief CVaR风险评估器
 *
 * 通过嵌套蒙特卡洛采样计算条件风险价值
 */
class CVaREvaluator
{
public:
  struct Config {
    double alpha{0.1};           // CVaR尾部比例
    int num_samples{50};         // 采样数
    double collision_radius{0.6};
    double collision_cost{100.0};  // 碰撞成本
    double proximity_scale{10.0};  // 接近度成本系数
  };

  explicit CVaREvaluator(const Config & config)
  : config_(config),
    rng_(std::random_device{}())
  {}

  /**
   * @brief 计算单条轨迹的CVaR值
   * @param trajectory_x X坐标序列 [T]
   * @param trajectory_y Y坐标序列 [T]
   * @param predictions 行人预测数据
   * @return CVaR_α值
   */
  double computeCVaR(
    const std::vector<float> & trajectory_x,
    const std::vector<float> & trajectory_y,
    const saye_msgs::msg::PedestrianPredictions & predictions);

  /**
   * @brief 批量计算CVaR值
   */
  std::vector<float> computeBatchCVaR(
    const std::vector<std::vector<float>> & trajectories_x,
    const std::vector<std::vector<float>> & trajectories_y,
    const saye_msgs::msg::PedestrianPredictions & predictions,
    const std::vector<size_t> & indices,
    bool parallel = true);

private:
  /**
   * @brief 从GMM采样单个行人轨迹
   * @return [time_step] -> (x, y)
   */
  std::vector<std::pair<double, double>> samplePedestrianTrajectory(
    const saye_msgs::msg::PedestrianPrediction & prediction);

  /**
   * @brief 采样所有行人的轨迹
   */
  std::vector<std::vector<std::pair<double, double>>> sampleAllPedestrians(
    const saye_msgs::msg::PedestrianPredictions & predictions);

  /**
   * @brief 计算机器人轨迹与采样行人轨迹的成本
   */
  double evaluateCost(
    const std::vector<float> & trajectory_x,
    const std::vector<float> & trajectory_y,
    const std::vector<std::vector<std::pair<double, double>>> & ped_trajectories);

  /**
   * @brief 计算两点之间的距离
   */
  inline double distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
  }

  Config config_;
  std::mt19937 rng_;
  std::normal_distribution<double> normal_dist_{0.0, 1.0};
};

}  // namespace saye_rapa_mppi

#endif  // SAYE_RAPA_MPPI__CVAR_EVALUATOR_HPP_
```

**文件: `src/cvar_evaluator.cpp`**

```cpp
#include "saye_rapa_mppi/cvar_evaluator.hpp"
#include <algorithm>
#include <execution>
#include <numeric>
#include <cmath>

namespace saye_rapa_mppi
{

double CVaREvaluator::computeCVaR(
  const std::vector<float> & trajectory_x,
  const std::vector<float> & trajectory_y,
  const saye_msgs::msg::PedestrianPredictions & predictions)
{
  if (predictions.predictions.empty()) {
    return 0.0;
  }

  // 1. 采样N次，生成成本分布
  std::vector<double> cost_samples;
  cost_samples.reserve(config_.num_samples);

  for (int n = 0; n < config_.num_samples; ++n) {
    // 从GMM采样所有行人的轨迹
    auto ped_trajectories = sampleAllPedestrians(predictions);

    // 计算该采样下的成本
    double cost = evaluateCost(trajectory_x, trajectory_y, ped_trajectories);
    cost_samples.push_back(cost);
  }

  // 2. 排序
  std::sort(cost_samples.begin(), cost_samples.end());

  // 3. 计算VaR_α (第α分位数)
  int var_index = static_cast<int>(config_.alpha * config_.num_samples);
  if (var_index >= config_.num_samples) {
    var_index = config_.num_samples - 1;
  }
  double var_alpha = cost_samples[var_index];

  // 4. 计算CVaR_α (超过VaR的成本均值)
  double cvar = 0.0;
  int tail_count = 0;

  for (double cost : cost_samples) {
    if (cost >= var_alpha) {
      cvar += cost;
      tail_count++;
    }
  }

  if (tail_count == 0) {
    return 0.0;
  }

  return cvar / tail_count;
}

std::vector<float> CVaREvaluator::computeBatchCVaR(
  const std::vector<std::vector<float>> & trajectories_x,
  const std::vector<std::vector<float>> & trajectories_y,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  const std::vector<size_t> & indices,
  bool parallel)
{
  const size_t N = indices.size();
  std::vector<float> cvar_values(N);

  if (parallel) {
    std::vector<size_t> seq(N);
    std::iota(seq.begin(), seq.end(), 0);

    std::for_each(std::execution::par_unseq, seq.begin(), seq.end(),
      [&](size_t i) {
        size_t k = indices[i];
        cvar_values[i] = computeCVaR(
          trajectories_x[k], trajectories_y[k], predictions);
      });
  } else {
    for (size_t i = 0; i < N; ++i) {
      size_t k = indices[i];
      cvar_values[i] = computeCVaR(
        trajectories_x[k], trajectories_y[k], predictions);
    }
  }

  return cvar_values;
}

std::vector<std::pair<double, double>>
CVaREvaluator::samplePedestrianTrajectory(
  const saye_msgs::msg::PedestrianPrediction & prediction)
{
  std::vector<std::pair<double, double>> trajectory;

  if (prediction.gmm_modes.empty()) {
    return trajectory;
  }

  // 当前CV模型只有1个mode
  const auto & mode = prediction.gmm_modes[0];
  const size_t T = mode.mean_x.size();
  trajectory.reserve(T);

  for (size_t t = 0; t < T; ++t) {
    // 从二维高斯分布采样
    double mean_x = mode.mean_x[t];
    double mean_y = mode.mean_y[t];
    double std_x = std::sqrt(mode.cov_xx[t]);
    double std_y = std::sqrt(mode.cov_yy[t]);

    double x = mean_x + std_x * normal_dist_(rng_);
    double y = mean_y + std_y * normal_dist_(rng_);

    trajectory.emplace_back(x, y);
  }

  return trajectory;
}

std::vector<std::vector<std::pair<double, double>>>
CVaREvaluator::sampleAllPedestrians(
  const saye_msgs::msg::PedestrianPredictions & predictions)
{
  std::vector<std::vector<std::pair<double, double>>> all_trajectories;
  all_trajectories.reserve(predictions.predictions.size());

  for (const auto & pred : predictions.predictions) {
    all_trajectories.push_back(samplePedestrianTrajectory(pred));
  }

  return all_trajectories;
}

double CVaREvaluator::evaluateCost(
  const std::vector<float> & trajectory_x,
  const std::vector<float> & trajectory_y,
  const std::vector<std::vector<std::pair<double, double>>> & ped_trajectories)
{
  double total_cost = 0.0;
  const size_t T = trajectory_x.size();

  // 遍历时间步
  for (size_t t = 0; t < T; ++t) {
    float robot_x = trajectory_x[t];
    float robot_y = trajectory_y[t];

    // 计算与所有行人的距离
    for (const auto & ped_traj : ped_trajectories) {
      if (t >= ped_traj.size()) {
        continue;
      }

      double ped_x = ped_traj[t].first;
      double ped_y = ped_traj[t].second;

      double dist = distance(robot_x, robot_y, ped_x, ped_y);

      // 碰撞成本
      if (dist < config_.collision_radius) {
        total_cost += config_.collision_cost;
      }
      // 接近度成本（软约束）
      else if (dist < 2.0 * config_.collision_radius) {
        double proximity = 2.0 * config_.collision_radius - dist;
        total_cost += config_.proximity_scale * proximity;
      }
    }
  }

  return total_cost;
}

}  // namespace saye_rapa_mppi
```

---

## Phase 4: 多辅助控制器

### 4.1 实现方式（两种选择）

#### 选项A: 修改Nav2 MPPI的噪声生成器（需要Fork）

**优点**:
- 深度集成到MPPI采样流程
- 性能最优

**缺点**:
- 需要维护Nav2的分支
- 更新困难

**实现位置**: `nav2_mppi_controller/src/tools/noise_generator.cpp`

#### 选项B: 在Critic中注入引导样本（推荐）⭐

**优点**:
- 无需修改Nav2核心代码
- 易于维护和调试

**缺点**:
- 轻微的性能开销

**实现方式**: 在`RiskAwareCritic`中添加一个预处理步骤，修改部分轨迹为引导轨迹

### 4.2 推荐实现（选项B）

**文件: `include/saye_rapa_mppi/guided_sampling.hpp`**

```cpp
#ifndef SAYE_RAPA_MPPI__GUIDED_SAMPLING_HPP_
#define SAYE_RAPA_MPPI__GUIDED_SAMPLING_HPP_

#include <vector>
#include "nav2_mppi_controller/models/state.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"

namespace saye_rapa_mppi
{

/**
 * @brief 引导性采样策略
 *
 * 注入辅助控制器生成的样本：
 * 1. Guide控制器：跟踪全局路径
 * 2. Brake控制器：安全停止
 */
class GuidedSamplingStrategy
{
public:
  struct Config {
    double guide_ratio{0.10};    // 引导样本比例（10%）
    double brake_ratio{0.05};    // 刹车样本比例（5%）
    double lookahead_dist{2.0};  // Pure Pursuit前视距离
    double max_steer_angle{0.5}; // 最大转向角
  };

  explicit GuidedSamplingStrategy(const Config & config)
  : config_(config) {}

  /**
   * @brief 注入引导样本到轨迹集合
   * @param trajectories 输入/输出：MPPI生成的轨迹
   * @param state 当前机器人状态
   * @param path 全局路径
   */
  void injectGuidedSamples(
    models::Trajectories & trajectories,
    const models::State & state,
    const nav_msgs::msg::Path & path);

private:
  /**
   * @brief 生成Guide控制序列（Pure Pursuit）
   */
  models::ControlSequence computeGuideControl(
    const models::State & state,
    const nav_msgs::msg::Path & path);

  /**
   * @brief 生成Brake控制序列
   */
  models::ControlSequence computeBrakeControl();

  /**
   * @brief 使用控制序列推演轨迹
   */
  void rolloutTrajectory(
    const models::State & start_state,
    const models::ControlSequence & controls,
    models::Trajectory & trajectory);

  Config config_;
};

}  // namespace saye_rapa_mppi

#endif  // SAYE_RAPA_MPPI__GUIDED_SAMPLING_HPP_
```

**在RiskAwareCritic中集成：**

```cpp
// 在 RiskAwareCritic::score() 开始处
void RiskAwareCritic::score(CriticData & data)
{
  // 1. 注入引导样本（可选，基于参数）
  if (params_.enable_guided_sampling) {
    guided_sampler_->injectGuidedSamples(
      data.trajectories,
      data.state,
      data.path
    );
  }

  // 2. 风险评估
  // ...
}
```

### 4.3 简化方案（初期实现）

**不实现完整的Guide控制器**，而是在配置中**强制包含一个零速度轨迹**作为安全冗余：

**在 `mppi_nav2_params.yaml` 中：**

```yaml
controller_server:
  ros__parameters:
    FollowPath:
      plugin: "nav2_mppi_controller::MPPIController"

      # 采样参数
      batch_size: 2000
      iteration_count: 1

      # 确保包含零速度样本（安全冗余）
      vx_std: 0.2
      vy_std: 0.0  # Ackermann车辆
      wz_std: 0.3

      # 温度参数（允许更多样化）
      temperature: 0.3

      # 新增: RiskAwareCritic
      critics: [
        "GoalCritic",
        "GoalAngleCritic",
        "PathAlignCritic",
        "PathFollowCritic",
        "ObstaclesCritic",
        "RiskAwareCritic"  # ← 新增
      ]

      # RiskAwareCritic参数
      RiskAwareCritic:
        enabled: true
        cost_weight: 50.0
        cost_power: 1

        # CP参数
        sigma_prune: 0.3
        prune_penalty: 1000.0
        collision_radius: 0.6
        mc_samples: 100

        # CVaR参数
        enable_cvar: true
        alpha: 0.1
        risk_budget: 50.0
        cvar_weight: 100.0
        cvar_samples: 50

        # 性能参数
        enable_parallel: true
```

---

## Phase 5: 集成与调优

### 5.1 完整的RiskAwareCritic实现

**文件: `src/risk_aware_critic.cpp`**

```cpp
#include "saye_rapa_mppi/risk_aware_critic.hpp"
#include <algorithm>

namespace saye_rapa_mppi
{

void RiskAwareCritic::initialize()
{
  // 1. 获取参数
  auto node = parent_->shared_from_this();

  // CP参数
  params_.sigma_prune = getParam(node, "sigma_prune", 0.3);
  params_.prune_penalty = getParam(node, "prune_penalty", 1e6);
  params_.collision_radius = getParam(node, "collision_radius", 0.6);
  params_.mc_samples = getParam(node, "mc_samples", 100);

  // CVaR参数
  params_.enable_cvar = getParam(node, "enable_cvar", true);
  params_.alpha = getParam(node, "alpha", 0.1);
  params_.risk_budget = getParam(node, "risk_budget", 50.0);
  params_.cvar_weight = getParam(node, "cvar_weight", 100.0);
  params_.cvar_samples = getParam(node, "cvar_samples", 50);

  // 性能参数
  params_.enable_parallel = getParam(node, "enable_parallel", true);

  // 2. 初始化功能模块
  CollisionProbabilityCalculator::Config cp_config;
  cp_config.collision_radius = params_.collision_radius;
  cp_config.mc_samples = params_.mc_samples;
  cp_calculator_ = std::make_unique<CollisionProbabilityCalculator>(cp_config);

  CVaREvaluator::Config cvar_config;
  cvar_config.alpha = params_.alpha;
  cvar_config.num_samples = params_.cvar_samples;
  cvar_config.collision_radius = params_.collision_radius;
  cvar_evaluator_ = std::make_unique<CVaREvaluator>(cvar_config);

  // 3. 订阅行人预测数据
  prediction_sub_ = node->create_subscription<
    saye_msgs::msg::PedestrianPredictions>(
    "/pedestrian_predictions",
    rclcpp::SystemDefaultsQoS(),
    std::bind(&RiskAwareCritic::predictionCallback, this, std::placeholders::_1)
  );

  RCLCPP_INFO(
    logger_,
    "RiskAwareCritic initialized with CP_prune=%.2f, CVaR_alpha=%.2f",
    params_.sigma_prune, params_.alpha
  );
}

void RiskAwareCritic::predictionCallback(
  const saye_msgs::msg::PedestrianPredictions::SharedPtr msg)
{
  std::lock_guard<std::mutex> lock(prediction_mutex_);
  latest_predictions_ = msg;
}

void RiskAwareCritic::score(CriticData & data)
{
  iteration_count_++;

  // 1. 检查是否有预测数据
  saye_msgs::msg::PedestrianPredictions::SharedPtr predictions;
  {
    std::lock_guard<std::mutex> lock(prediction_mutex_);
    predictions = latest_predictions_;
  }

  if (!predictions || predictions->predictions.empty()) {
    // 无动态障碍物，不添加额外成本
    if (iteration_count_ % LOG_THROTTLE == 0) {
      RCLCPP_INFO(logger_, "RiskAwareCritic: No pedestrian predictions available");
    }
    return;
  }

  // 2. 提取轨迹数据
  const size_t K = data.trajectories.x.shape(0);  // 轨迹数
  const size_t T = data.trajectories.x.shape(1);  // 时间步数

  std::vector<std::vector<float>> trajectories_x(K);
  std::vector<std::vector<float>> trajectories_y(K);

  for (size_t k = 0; k < K; ++k) {
    trajectories_x[k].resize(T);
    trajectories_y[k].resize(T);
    for (size_t t = 0; t < T; ++t) {
      trajectories_x[k][t] = data.trajectories.x(k, t);
      trajectories_y[k][t] = data.trajectories.y(k, t);
    }
  }

  // 3. 阶段1: 计算CP并剪枝
  std::vector<float> cp_values;
  std::vector<size_t> elite_indices = computeCollisionProbabilities(
    data, *predictions, trajectories_x, trajectories_y, cp_values);

  // 4. 阶段2: 计算CVaR（可选）
  std::vector<float> cvar_values;
  if (params_.enable_cvar && !elite_indices.empty()) {
    computeCVaRValues(
      data, *predictions, trajectories_x, trajectories_y,
      elite_indices, cvar_values);
  }

  // 5. 生成最终风险成本
  generateRiskCosts(cp_values, cvar_values, elite_indices, data.costs);

  // 6. 日志统计
  if (iteration_count_ % LOG_THROTTLE == 0) {
    RCLCPP_INFO(
      logger_,
      "RiskAwareCritic: K=%zu, Pruned=%zu, Elite=%zu, AvgCP=%.3f, AvgCVaR=%.1f",
      K, stats_.pruned_count, stats_.elite_count,
      stats_.avg_cp, stats_.avg_cvar
    );
  }
}

std::vector<size_t> RiskAwareCritic::computeCollisionProbabilities(
  const CriticData & data,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  const std::vector<std::vector<float>> & trajectories_x,
  const std::vector<std::vector<float>> & trajectories_y,
  std::vector<float> & cp_values)
{
  const size_t K = trajectories_x.size();

  // 批量计算CP
  cp_values = cp_calculator_->computeBatchCollisionProbabilities(
    trajectories_x, trajectories_y, predictions, params_.enable_parallel);

  // 筛选精英轨迹
  std::vector<size_t> elite_indices;
  double cp_sum = 0.0;
  size_t pruned = 0;

  for (size_t k = 0; k < K; ++k) {
    cp_sum += cp_values[k];

    if (cp_values[k] < params_.sigma_prune) {
      elite_indices.push_back(k);
    } else {
      pruned++;
    }
  }

  // 更新统计
  stats_.total_trajectories = K;
  stats_.pruned_count = pruned;
  stats_.elite_count = elite_indices.size();
  stats_.avg_cp = cp_sum / K;

  return elite_indices;
}

void RiskAwareCritic::computeCVaRValues(
  const CriticData & data,
  const saye_msgs::msg::PedestrianPredictions & predictions,
  const std::vector<std::vector<float>> & trajectories_x,
  const std::vector<std::vector<float>> & trajectories_y,
  const std::vector<size_t> & elite_indices,
  std::vector<float> & cvar_values)
{
  // 批量计算CVaR
  cvar_values = cvar_evaluator_->computeBatchCVaR(
    trajectories_x, trajectories_y, predictions,
    elite_indices, params_.enable_parallel);

  // 更新统计
  if (!cvar_values.empty()) {
    stats_.avg_cvar = std::accumulate(
      cvar_values.begin(), cvar_values.end(), 0.0) / cvar_values.size();
  }
}

void RiskAwareCritic::generateRiskCosts(
  const std::vector<float> & cp_values,
  const std::vector<float> & cvar_values,
  const std::vector<size_t> & elite_indices,
  xt::xtensor<float, 1> & costs)
{
  const size_t K = cp_values.size();

  for (size_t k = 0; k < K; ++k) {
    float risk_cost = 0.0;

    // CP剪枝惩罚
    if (cp_values[k] >= params_.sigma_prune) {
      risk_cost = params_.prune_penalty;
    }
    // CVaR惩罚
    else if (params_.enable_cvar && !cvar_values.empty()) {
      // 查找k在elite_indices中的位置
      auto it = std::find(elite_indices.begin(), elite_indices.end(), k);
      if (it != elite_indices.end()) {
        size_t idx = std::distance(elite_indices.begin(), it);
        float cvar = cvar_values[idx];

        // 公式5: 超过风险预算时添加惩罚
        if (cvar > params_.risk_budget) {
          risk_cost = params_.cvar_weight * cvar;
        }
      }
    }

    // 累加到总成本
    costs(k) += risk_cost * weight_;  // weight_是critic的权重
  }
}

}  // namespace saye_rapa_mppi

// Pluginlib导出
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(
  saye_rapa_mppi::RiskAwareCritic,
  nav2_mppi_controller::critics::CriticFunction
)
```

### 5.2 CMakeLists.txt配置

```cmake
cmake_minimum_required(VERSION 3.5)
project(saye_rapa_mppi)

# C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项（启用并行和优化）
add_compile_options(-O3 -march=native)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_mppi_controller REQUIRED)
find_package(nav2_core REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(saye_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(pluginlib REQUIRED)

# TBB（用于并行算法）
find_package(TBB REQUIRED)

include_directories(include)

# 库
add_library(${PROJECT_NAME} SHARED
  src/risk_aware_critic.cpp
  src/collision_probability.cpp
  src/cvar_evaluator.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  nav2_mppi_controller
  nav2_core
  nav2_costmap_2d
  saye_msgs
  geometry_msgs
  tf2
  tf2_geometry_msgs
  pluginlib
)

target_link_libraries(${PROJECT_NAME} TBB::tbb)

# 安装
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/
)

install(FILES plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

# 测试
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_add_gtest(test_collision_prob test/test_collision_prob.cpp)
  target_link_libraries(test_collision_prob ${PROJECT_NAME})

  ament_add_gtest(test_cvar test/test_cvar.cpp)
  target_link_libraries(test_cvar ${PROJECT_NAME})
endif()

# 导出
pluginlib_export_plugin_description_file(nav2_mppi_controller plugins.xml)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(
  rclcpp
  nav2_mppi_controller
  nav2_core
  saye_msgs
)

ament_package()
```

### 5.3 plugins.xml配置

```xml
<library path="libsaye_rapa_mppi">
  <class type="saye_rapa_mppi::RiskAwareCritic"
         base_class_type="nav2_mppi_controller::critics::CriticFunction">
    <description>
      RAPA-MPPI Risk-Aware Critic: Two-stage risk assessment using
      Collision Probability (CP) pruning and Conditional Value-at-Risk (CVaR)
      evaluation for dynamic obstacle avoidance.
    </description>
  </class>
</library>
```

---

## 测试与验证方案

### 测试场景设计

#### 场景1: 单行人直线穿越（基础验证）
**文件**: `saye_moving_obstacles.sdf`（已有）

**测试目标**:
- RiskAwareCritic能否成功加载
- CP计算是否正常
- 是否检测到碰撞风险

**验证指标**:
- CP值应在行人接近时增大
- CVaR值应反映尾部风险
- 车辆应减速或绕行

#### 场景2: 双行人交替穿越（中等难度）
**文件**: `scenario1_4ped_crossing.sdf`

**测试目标**:
- 多行人联合概率计算
- CVaR对复杂场景的适应性

#### 场景3: 高密度行人（压力测试）
**文件**: `scenario3_12ped_crowded.sdf`

**测试目标**:
- 计算性能（是否能保持20Hz）
- 剪枝效率
- 安全性（是否产生冻结）

### 测试脚本

**文件**: `test_rapa_mppi.sh`

```bash
#!/bin/bash

echo "=== RAPA-MPPI Integration Test ==="

# 场景列表
scenarios=(
  "saye_moving_obstacles.sdf"
  "scenario1_4ped_crossing.sdf"
  "scenario3_12ped_crowded.sdf"
)

for scene in "${scenarios[@]}"; do
  echo ""
  echo "=== Testing: $scene ==="

  # 启动仿真
  ros2 launch saye_bringup_my saye_spawn.launch.py world:=$scene &
  SIM_PID=$!
  sleep 10

  # 启动预测器
  ros2 launch saye_prediction ground_truth_prediction.launch.py &
  PRED_PID=$!
  sleep 5

  # 启动MPPI导航
  ros2 launch saye_mppi mppi_experiment.launch.py &
  NAV_PID=$!
  sleep 5

  # 发送目标点
  ros2 topic pub --once /goal_pose geometry_msgs/msg/PoseStamped \
    "{header: {frame_id: 'map'}, pose: {position: {x: 15.0, y: 0.0, z: 0.0}}}"

  # 运行60秒
  sleep 60

  # 收集指标
  echo "Collecting metrics..."
  ros2 topic echo /diagnostics --once

  # 停止所有节点
  kill $NAV_PID $PRED_PID $SIM_PID
  sleep 5
done

echo "=== Test Complete ==="
```

### 性能指标采集

**新建节点**: `saye_rapa_mppi/scripts/performance_monitor.py`

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
import numpy as np
import time

class PerformanceMonitor(Node):
    def __init__(self):
        super().__init__('performance_monitor')

        self.odom_sub = self.create_subscription(
            Odometry, '/odom', self.odom_callback, 10)
        self.cmd_sub = self.create_subscription(
            Twist, '/cmd_vel', self.cmd_callback, 10)

        self.positions = []
        self.velocities = []
        self.timestamps = []
        self.start_time = time.time()

    def odom_callback(self, msg):
        t = time.time() - self.start_time
        x = msg.pose.pose.position.x
        y = msg.pose.pose.position.y
        vx = msg.twist.twist.linear.x

        self.positions.append((t, x, y))
        self.velocities.append((t, vx))

    def cmd_callback(self, msg):
        # 检测冻结（速度命令为0且持续时间长）
        if abs(msg.linear.x) < 0.01:
            self.get_logger().warn('Potential freeze detected!')

    def compute_metrics(self):
        if len(self.positions) < 2:
            return

        # 总路程
        positions = np.array(self.positions)
        dists = np.sqrt(np.diff(positions[:, 1])**2 + np.diff(positions[:, 2])**2)
        total_dist = np.sum(dists)

        # 平均速度
        velocities = np.array(self.velocities)
        avg_vel = np.mean(velocities[:, 1])

        # 任务时间
        task_time = positions[-1, 0] - positions[0, 0]

        self.get_logger().info(f"""
        Performance Metrics:
        - Total Distance: {total_dist:.2f} m
        - Task Time: {task_time:.2f} s
        - Average Velocity: {avg_vel:.2f} m/s
        - Smoothness: {self.compute_jerk():.2f}
        """)

    def compute_jerk(self):
        # 计算加加速度（平滑度指标）
        vels = np.array(self.velocities)
        if len(vels) < 3:
            return 0.0
        accs = np.diff(vels[:, 1])
        jerks = np.diff(accs)
        return np.std(jerks)

def main():
    rclpy.init()
    monitor = PerformanceMonitor()

    try:
        rclpy.spin(monitor)
    except KeyboardInterrupt:
        monitor.compute_metrics()
    finally:
        monitor.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
```

---

## 参数调优指南

### 初始参数（保守策略）

```yaml
RiskAwareCritic:
  # 高优先级 - 强剪枝
  sigma_prune: 0.2         # 降低CP阈值，更激进剪枝
  prune_penalty: 2000.0    # 高惩罚

  # CVaR参数
  alpha: 0.05              # 关注最坏的5%
  risk_budget: 30.0        # 较低风险预算
  cvar_weight: 150.0       # 高CVaR惩罚

  # 性能参数
  mc_samples: 80           # 降低采样数提高速度
  cvar_samples: 30
```

### 调优步骤

**Step 1: 调整CP剪枝阈值**
```
sigma_prune: 0.1 → 0.2 → 0.3 → 0.4
```
观察：
- 剪枝轨迹数量
- 是否产生保守行为
- 计算时间

**Step 2: 调整CVaR参数**
```
alpha: 0.05 → 0.10 → 0.15
risk_budget: 30 → 50 → 70
```
观察：
- CVaR评估频率
- 是否有效规避风险

**Step 3: 平衡性能与安全**
```
mc_samples: 50 → 100 → 200
cvar_samples: 20 → 50 → 100
```
观察：
- MPPI控制频率（目标20Hz）
- 风险评估准确性

### 调优工具

**实时参数调整（rqt_reconfigure）**:
```bash
ros2 run rqt_reconfigure rqt_reconfigure
```

**日志分析**:
```bash
ros2 topic echo /diagnostics | grep RiskAwareCritic
```

---

## 完整启动流程

### 终端1: 启动仿真
```bash
cd ~/ackermann_sim
source install/setup.bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_moving_obstacles.sdf
```

### 终端2: 启动预测器
```bash
source install/setup.bash
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

### 终端3: 启动MPPI导航
```bash
source install/setup.bash
ros2 launch saye_mppi mppi_experiment.launch.py
```

### 终端4: 性能监控（可选）
```bash
source install/setup.bash
ros2 run saye_rapa_mppi performance_monitor.py
```

### 终端5: 发送目标
```bash
ros2 topic pub --once /goal_pose geometry_msgs/msg/PoseStamped \
  "{header: {frame_id: 'map'},
    pose: {position: {x: 15.0, y: 0.0, z: 0.0},
           orientation: {w: 1.0}}}"
```

---

## 分阶段实施时间表

| 阶段 | 任务 | 预估时间 | 输出 |
|------|------|----------|------|
| Phase 1 | 搭建框架，空Critic | 2天 | 可编译的包 |
| Phase 2 | CP计算模块 + 单测 | 3天 | CP值可视化 |
| Phase 3 | CVaR评估模块 + 单测 | 3天 | CVaR值可视化 |
| Phase 4 | 集成到MPPI，基础测试 | 2天 | 可运行的系统 |
| Phase 5 | 参数调优，场景测试 | 3天 | 性能数据 |
| Phase 6 | 多辅助控制器（可选） | 3天 | 完整RAPA-MPPI |
| **总计** | | **16-19天** | |

---

## 关键注意事项

1. **坐标系一致性**: 确保所有模块使用相同坐标系（`odom` frame）
2. **时间同步**: 预测数据与轨迹时间步对齐
3. **线程安全**: 预测数据回调使用mutex保护
4. **性能优化**: 启用并行计算，使用C++17的`<execution>`
5. **错误处理**: 处理空预测、轨迹长度不匹配等边界情况
6. **日志节流**: 避免日志洪水影响性能

---

**文档版本**: 1.0
**创建日期**: 2025-11-06
**最后更新**: 2025-11-06
**下一步**: 开始Phase 1 - 创建saye_rapa_mppi包
