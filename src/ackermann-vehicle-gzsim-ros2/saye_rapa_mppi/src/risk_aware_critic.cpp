#include "saye_rapa_mppi/risk_aware_critic.hpp"

namespace saye_rapa_mppi
{

void RiskAwareCritic::initialize()
{
  auto node = parent_.lock();
  if (!node) {
    throw std::runtime_error("Failed to lock node");
  }

  auto getParam = parameters_handler_->getParamGetter(name_);
  getParam(cost_weight_, "cost_weight", 1.0);

  // 订阅行人预测数据
  prediction_sub_ = node->create_subscription<saye_msgs::msg::PedestrianPredictions>(
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

void RiskAwareCritic::score(mppi::CriticData & data)
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
  mppi::critics::CriticFunction
)
