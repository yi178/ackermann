// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/gmm_component.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__BUILDER_HPP_
#define SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "saye_msgs/msg/detail/gmm_component__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace saye_msgs
{

namespace msg
{

namespace builder
{

class Init_GMMComponent_cov_xy
{
public:
  explicit Init_GMMComponent_cov_xy(::saye_msgs::msg::GMMComponent & msg)
  : msg_(msg)
  {}
  ::saye_msgs::msg::GMMComponent cov_xy(::saye_msgs::msg::GMMComponent::_cov_xy_type arg)
  {
    msg_.cov_xy = std::move(arg);
    return std::move(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

class Init_GMMComponent_cov_yy
{
public:
  explicit Init_GMMComponent_cov_yy(::saye_msgs::msg::GMMComponent & msg)
  : msg_(msg)
  {}
  Init_GMMComponent_cov_xy cov_yy(::saye_msgs::msg::GMMComponent::_cov_yy_type arg)
  {
    msg_.cov_yy = std::move(arg);
    return Init_GMMComponent_cov_xy(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

class Init_GMMComponent_cov_xx
{
public:
  explicit Init_GMMComponent_cov_xx(::saye_msgs::msg::GMMComponent & msg)
  : msg_(msg)
  {}
  Init_GMMComponent_cov_yy cov_xx(::saye_msgs::msg::GMMComponent::_cov_xx_type arg)
  {
    msg_.cov_xx = std::move(arg);
    return Init_GMMComponent_cov_yy(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

class Init_GMMComponent_mean_y
{
public:
  explicit Init_GMMComponent_mean_y(::saye_msgs::msg::GMMComponent & msg)
  : msg_(msg)
  {}
  Init_GMMComponent_cov_xx mean_y(::saye_msgs::msg::GMMComponent::_mean_y_type arg)
  {
    msg_.mean_y = std::move(arg);
    return Init_GMMComponent_cov_xx(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

class Init_GMMComponent_mean_x
{
public:
  explicit Init_GMMComponent_mean_x(::saye_msgs::msg::GMMComponent & msg)
  : msg_(msg)
  {}
  Init_GMMComponent_mean_y mean_x(::saye_msgs::msg::GMMComponent::_mean_x_type arg)
  {
    msg_.mean_x = std::move(arg);
    return Init_GMMComponent_mean_y(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

class Init_GMMComponent_weight
{
public:
  Init_GMMComponent_weight()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GMMComponent_mean_x weight(::saye_msgs::msg::GMMComponent::_weight_type arg)
  {
    msg_.weight = std::move(arg);
    return Init_GMMComponent_mean_x(msg_);
  }

private:
  ::saye_msgs::msg::GMMComponent msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::saye_msgs::msg::GMMComponent>()
{
  return saye_msgs::msg::builder::Init_GMMComponent_weight();
}

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__BUILDER_HPP_
