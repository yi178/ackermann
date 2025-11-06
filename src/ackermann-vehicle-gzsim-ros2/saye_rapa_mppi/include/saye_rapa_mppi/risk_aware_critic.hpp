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

class RiskAwareCritic : public mppi::critics::CriticFunction
{
public:
  void initialize() override;
  void score(mppi::CriticData & data) override;

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
