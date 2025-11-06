#!/bin/bash

# RAPA-MPPI Phase 1 自动化设置脚本
# 功能：创建saye_rapa_mppi包的基础框架

set -e  # 遇到错误立即退出

echo "=========================================="
echo "  RAPA-MPPI Phase 1 Setup Script"
echo "=========================================="
echo ""

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 工作目录
WORKSPACE_ROOT="$HOME/ackermann_sim"
PKG_ROOT="$WORKSPACE_ROOT/src/ackermann-vehicle-gzsim-ros2/saye_rapa_mppi"

echo -e "${YELLOW}步骤 1/6: 检查工作空间${NC}"
if [ ! -d "$WORKSPACE_ROOT" ]; then
    echo -e "${RED}错误: 工作空间不存在: $WORKSPACE_ROOT${NC}"
    exit 1
fi
cd "$WORKSPACE_ROOT/src/ackermann-vehicle-gzsim-ros2"
echo -e "${GREEN}✓ 工作空间路径正确${NC}"

echo ""
echo -e "${YELLOW}步骤 2/6: 创建ROS 2包${NC}"
if [ -d "$PKG_ROOT" ]; then
    echo -e "${YELLOW}警告: 包已存在，跳过创建${NC}"
else
    ros2 pkg create saye_rapa_mppi \
      --build-type ament_cmake \
      --dependencies \
        rclcpp \
        nav2_mppi_controller \
        nav2_core \
        nav2_costmap_2d \
        saye_msgs \
        geometry_msgs \
        nav_msgs \
        tf2 \
        tf2_geometry_msgs \
        pluginlib
    echo -e "${GREEN}✓ 包创建成功${NC}"
fi

echo ""
echo -e "${YELLOW}步骤 3/6: 创建目录结构${NC}"
mkdir -p "$PKG_ROOT/include/saye_rapa_mppi/utils"
mkdir -p "$PKG_ROOT/src"
mkdir -p "$PKG_ROOT/config"
mkdir -p "$PKG_ROOT/test"
mkdir -p "$PKG_ROOT/scripts"
echo -e "${GREEN}✓ 目录结构创建完成${NC}"

echo ""
echo -e "${YELLOW}步骤 4/6: 创建配置文件${NC}"

# plugins.xml
cat > "$PKG_ROOT/plugins.xml" << 'EOF'
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
EOF

# 参数配置文件
cat > "$PKG_ROOT/config/rapa_mppi_params.yaml" << 'EOF'
controller_server:
  ros__parameters:
    controller_frequency: 20.0
    FollowPath:
      plugin: "nav2_mppi_controller::MPPIController"
      time_steps: 56
      model_dt: 0.05
      batch_size: 2000
      vx_std: 0.2
      vy_std: 0.0
      wz_std: 0.3
      vx_max: 2.0
      vx_min: -0.5
      vy_max: 0.0
      wz_max: 1.0
      iteration_count: 1
      temperature: 0.3
      gamma: 0.015

      motion_model: "Ackermann"

      critics: [
        "ConstraintCritic",
        "GoalCritic",
        "GoalAngleCritic",
        "PathAlignCritic",
        "PathFollowCritic",
        "PathAngleCritic",
        "ObstaclesCritic",
        "RiskAwareCritic"
      ]

      ConstraintCritic:
        enabled: true
        cost_power: 1
        cost_weight: 4.0

      GoalCritic:
        enabled: true
        cost_power: 1
        cost_weight: 5.0
        threshold_to_consider: 1.4

      GoalAngleCritic:
        enabled: true
        cost_power: 1
        cost_weight: 3.0
        threshold_to_consider: 0.5

      PathAlignCritic:
        enabled: true
        cost_power: 1
        cost_weight: 14.0
        max_path_occupancy_ratio: 0.05
        trajectory_point_step: 3
        threshold_to_consider: 0.5

      PathFollowCritic:
        enabled: true
        cost_power: 1
        cost_weight: 5.0
        offset_from_path: 1.0
        threshold_to_consider: 1.4

      PathAngleCritic:
        enabled: true
        cost_power: 1
        cost_weight: 2.0
        offset_from_path: 0.8
        threshold_to_consider: 0.5
        max_angle_to_furthest: 1.0

      ObstaclesCritic:
        enabled: true
        cost_power: 1
        cost_weight: 3.0
        inflation_radius: 0.8
        collision_cost: 10000.0
        collision_margin_distance: 0.3
        near_goal_distance: 0.5

      # 新增: RiskAwareCritic
      RiskAwareCritic:
        enabled: true
        cost_power: 1
        cost_weight: 50.0

        # CP剪枝参数
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
        enable_guided_sampling: false
EOF

echo -e "${GREEN}✓ 配置文件创建完成${NC}"

echo ""
echo -e "${YELLOW}步骤 5/6: 创建占位头文件${NC}"

# risk_aware_critic.hpp (简化版，仅框架)
cat > "$PKG_ROOT/include/saye_rapa_mppi/risk_aware_critic.hpp" << 'EOF'
#ifndef SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_
#define SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_

#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "nav2_mppi_controller/critic_function.hpp"
#include "nav2_mppi_controller/models/state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "saye_msgs/msg/pedestrian_predictions.hpp"

namespace saye_rapa_mppi
{

class RiskAwareCritic : public nav2_mppi_controller::critics::CriticFunction
{
public:
  void initialize() override;
  void score(CriticData & data) override;

protected:
  void predictionCallback(
    const saye_msgs::msg::PedestrianPredictions::SharedPtr msg);

private:
  rclcpp::Subscription<saye_msgs::msg::PedestrianPredictions>::SharedPtr
    prediction_sub_;
  saye_msgs::msg::PedestrianPredictions::SharedPtr latest_predictions_;
  std::mutex prediction_mutex_;

  // 参数（Phase 1暂时简化）
  double cost_weight_{1.0};
  size_t iteration_count_{0};
};

}  // namespace saye_rapa_mppi

#endif  // SAYE_RAPA_MPPI__RISK_AWARE_CRITIC_HPP_
EOF

echo -e "${GREEN}✓ 头文件创建完成${NC}"

echo ""
echo -e "${YELLOW}步骤 6/6: 创建最小实现${NC}"

# risk_aware_critic.cpp (最小可编译版本)
cat > "$PKG_ROOT/src/risk_aware_critic.cpp" << 'EOF'
#include "saye_rapa_mppi/risk_aware_critic.hpp"

namespace saye_rapa_mppi
{

void RiskAwareCritic::initialize()
{
  auto node = parent_->shared_from_this();

  cost_weight_ = getParam(node, "cost_weight", 1.0);

  // 订阅行人预测数据
  prediction_sub_ = node->create_subscription<
    saye_msgs::msg::PedestrianPredictions>(
    "/pedestrian_predictions",
    rclcpp::SystemDefaultsQoS(),
    std::bind(&RiskAwareCritic::predictionCallback, this, std::placeholders::_1)
  );

  RCLCPP_INFO(
    logger_,
    "RiskAwareCritic initialized (Phase 1 - Framework Only)"
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

  // Phase 1: 暂不添加任何成本，仅验证框架
  if (iteration_count_ % 50 == 0) {
    std::lock_guard<std::mutex> lock(prediction_mutex_);
    if (latest_predictions_ && !latest_predictions_->predictions.empty()) {
      RCLCPP_INFO(
        logger_,
        "RiskAwareCritic: Received predictions for %zu pedestrians",
        latest_predictions_->predictions.size()
      );
    }
  }

  // 当前不修改costs，待Phase 2实现
}

}  // namespace saye_rapa_mppi

// Pluginlib导出
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(
  saye_rapa_mppi::RiskAwareCritic,
  nav2_mppi_controller::critics::CriticFunction
)
EOF

echo -e "${GREEN}✓ 源文件创建完成${NC}"

# 更新CMakeLists.txt
cat > "$PKG_ROOT/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.5)
project(saye_rapa_mppi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_mppi_controller REQUIRED)
find_package(nav2_core REQUIRED)
find_package(nav2_costmap_2d REQUIRED)
find_package(saye_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(pluginlib REQUIRED)

include_directories(include)

# 库
add_library(${PROJECT_NAME} SHARED
  src/risk_aware_critic.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  nav2_mppi_controller
  nav2_core
  nav2_costmap_2d
  saye_msgs
  geometry_msgs
  nav_msgs
  tf2
  tf2_geometry_msgs
  pluginlib
)

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
EOF

# 更新package.xml
cat > "$PKG_ROOT/package.xml" << 'EOF'
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>saye_rapa_mppi</name>
  <version>0.1.0</version>
  <description>RAPA-MPPI Risk-Aware Controller for Dynamic Obstacle Avoidance</description>
  <maintainer email="your@email.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <depend>rclcpp</depend>
  <depend>nav2_mppi_controller</depend>
  <depend>nav2_core</depend>
  <depend>nav2_costmap_2d</depend>
  <depend>saye_msgs</depend>
  <depend>geometry_msgs</depend>
  <depend>nav_msgs</depend>
  <depend>tf2</depend>
  <depend>tf2_geometry_msgs</depend>
  <depend>pluginlib</depend>

  <export>
    <build_type>ament_cmake</build_type>
    <nav2_mppi_controller plugin="${prefix}/plugins.xml" />
  </export>
</package>
EOF

echo -e "${GREEN}✓ 构建配置文件更新完成${NC}"

echo ""
echo -e "${YELLOW}开始编译...${NC}"
cd "$WORKSPACE_ROOT"
colcon build --packages-select saye_rapa_mppi --cmake-args -DCMAKE_BUILD_TYPE=Release

if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}=========================================="
    echo -e "  Phase 1 设置成功完成！"
    echo -e "==========================================${NC}"
    echo ""
    echo "下一步操作："
    echo "1. source install/setup.bash"
    echo "2. 查看详细实施方案: cat RAPA_MPPI_IMPLEMENTATION_PLAN.md"
    echo "3. 验证插件加载: ros2 pkg list | grep saye_rapa_mppi"
    echo ""
    echo "测试命令（终端1）："
    echo "  ros2 launch saye_bringup_my saye_spawn.launch.py world:=saye_moving_obstacles.sdf"
    echo ""
    echo "测试命令（终端2）："
    echo "  ros2 launch saye_prediction ground_truth_prediction.launch.py"
    echo ""
    echo "测试命令（终端3 - 使用新Critic）："
    echo "  ros2 launch saye_mppi mppi_experiment.launch.py \\"
    echo "    controller_params:=\$(ros2 pkg prefix saye_rapa_mppi)/share/saye_rapa_mppi/config/rapa_mppi_params.yaml"
    echo ""
else
    echo -e "${RED}编译失败，请检查错误信息${NC}"
    exit 1
fi
EOF
chmod +x "$WORKSPACE_ROOT/setup_phase1.sh"
