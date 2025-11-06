// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_predictions.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__BUILDER_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "saye_msgs/msg/detail/pedestrian_predictions__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace saye_msgs
{

namespace msg
{

namespace builder
{

class Init_PedestrianPredictions_predictions
{
public:
  explicit Init_PedestrianPredictions_predictions(::saye_msgs::msg::PedestrianPredictions & msg)
  : msg_(msg)
  {}
  ::saye_msgs::msg::PedestrianPredictions predictions(::saye_msgs::msg::PedestrianPredictions::_predictions_type arg)
  {
    msg_.predictions = std::move(arg);
    return std::move(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPredictions msg_;
};

class Init_PedestrianPredictions_header
{
public:
  Init_PedestrianPredictions_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PedestrianPredictions_predictions header(::saye_msgs::msg::PedestrianPredictions::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_PedestrianPredictions_predictions(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPredictions msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::saye_msgs::msg::PedestrianPredictions>()
{
  return saye_msgs::msg::builder::Init_PedestrianPredictions_header();
}

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__BUILDER_HPP_
