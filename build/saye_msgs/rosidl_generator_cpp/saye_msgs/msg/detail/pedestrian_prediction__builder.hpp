// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_prediction.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__BUILDER_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "saye_msgs/msg/detail/pedestrian_prediction__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace saye_msgs
{

namespace msg
{

namespace builder
{

class Init_PedestrianPrediction_gmm_modes
{
public:
  explicit Init_PedestrianPrediction_gmm_modes(::saye_msgs::msg::PedestrianPrediction & msg)
  : msg_(msg)
  {}
  ::saye_msgs::msg::PedestrianPrediction gmm_modes(::saye_msgs::msg::PedestrianPrediction::_gmm_modes_type arg)
  {
    msg_.gmm_modes = std::move(arg);
    return std::move(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPrediction msg_;
};

class Init_PedestrianPrediction_current_speed
{
public:
  explicit Init_PedestrianPrediction_current_speed(::saye_msgs::msg::PedestrianPrediction & msg)
  : msg_(msg)
  {}
  Init_PedestrianPrediction_gmm_modes current_speed(::saye_msgs::msg::PedestrianPrediction::_current_speed_type arg)
  {
    msg_.current_speed = std::move(arg);
    return Init_PedestrianPrediction_gmm_modes(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPrediction msg_;
};

class Init_PedestrianPrediction_current_pose
{
public:
  explicit Init_PedestrianPrediction_current_pose(::saye_msgs::msg::PedestrianPrediction & msg)
  : msg_(msg)
  {}
  Init_PedestrianPrediction_current_speed current_pose(::saye_msgs::msg::PedestrianPrediction::_current_pose_type arg)
  {
    msg_.current_pose = std::move(arg);
    return Init_PedestrianPrediction_current_speed(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPrediction msg_;
};

class Init_PedestrianPrediction_id
{
public:
  Init_PedestrianPrediction_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PedestrianPrediction_current_pose id(::saye_msgs::msg::PedestrianPrediction::_id_type arg)
  {
    msg_.id = std::move(arg);
    return Init_PedestrianPrediction_current_pose(msg_);
  }

private:
  ::saye_msgs::msg::PedestrianPrediction msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::saye_msgs::msg::PedestrianPrediction>()
{
  return saye_msgs::msg::builder::Init_PedestrianPrediction_id();
}

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__BUILDER_HPP_
