// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/gmm_component.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_HPP_
#define SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__saye_msgs__msg__GMMComponent __attribute__((deprecated))
#else
# define DEPRECATED__saye_msgs__msg__GMMComponent __declspec(deprecated)
#endif

namespace saye_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GMMComponent_
{
  using Type = GMMComponent_<ContainerAllocator>;

  explicit GMMComponent_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->weight = 0.0;
    }
  }

  explicit GMMComponent_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->weight = 0.0;
    }
  }

  // field types and members
  using _weight_type =
    double;
  _weight_type weight;
  using _mean_x_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _mean_x_type mean_x;
  using _mean_y_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _mean_y_type mean_y;
  using _cov_xx_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _cov_xx_type cov_xx;
  using _cov_yy_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _cov_yy_type cov_yy;
  using _cov_xy_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _cov_xy_type cov_xy;

  // setters for named parameter idiom
  Type & set__weight(
    const double & _arg)
  {
    this->weight = _arg;
    return *this;
  }
  Type & set__mean_x(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->mean_x = _arg;
    return *this;
  }
  Type & set__mean_y(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->mean_y = _arg;
    return *this;
  }
  Type & set__cov_xx(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->cov_xx = _arg;
    return *this;
  }
  Type & set__cov_yy(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->cov_yy = _arg;
    return *this;
  }
  Type & set__cov_xy(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->cov_xy = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    saye_msgs::msg::GMMComponent_<ContainerAllocator> *;
  using ConstRawPtr =
    const saye_msgs::msg::GMMComponent_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::GMMComponent_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::GMMComponent_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__saye_msgs__msg__GMMComponent
    std::shared_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__saye_msgs__msg__GMMComponent
    std::shared_ptr<saye_msgs::msg::GMMComponent_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GMMComponent_ & other) const
  {
    if (this->weight != other.weight) {
      return false;
    }
    if (this->mean_x != other.mean_x) {
      return false;
    }
    if (this->mean_y != other.mean_y) {
      return false;
    }
    if (this->cov_xx != other.cov_xx) {
      return false;
    }
    if (this->cov_yy != other.cov_yy) {
      return false;
    }
    if (this->cov_xy != other.cov_xy) {
      return false;
    }
    return true;
  }
  bool operator!=(const GMMComponent_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GMMComponent_

// alias to use template instance with default allocator
using GMMComponent =
  saye_msgs::msg::GMMComponent_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_HPP_
