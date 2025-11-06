// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_predictions.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'predictions'
#include "saye_msgs/msg/detail/pedestrian_prediction__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__saye_msgs__msg__PedestrianPredictions __attribute__((deprecated))
#else
# define DEPRECATED__saye_msgs__msg__PedestrianPredictions __declspec(deprecated)
#endif

namespace saye_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct PedestrianPredictions_
{
  using Type = PedestrianPredictions_<ContainerAllocator>;

  explicit PedestrianPredictions_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit PedestrianPredictions_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _predictions_type =
    std::vector<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>>;
  _predictions_type predictions;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__predictions(
    const std::vector<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>> & _arg)
  {
    this->predictions = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> *;
  using ConstRawPtr =
    const saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__saye_msgs__msg__PedestrianPredictions
    std::shared_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__saye_msgs__msg__PedestrianPredictions
    std::shared_ptr<saye_msgs::msg::PedestrianPredictions_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PedestrianPredictions_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->predictions != other.predictions) {
      return false;
    }
    return true;
  }
  bool operator!=(const PedestrianPredictions_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PedestrianPredictions_

// alias to use template instance with default allocator
using PedestrianPredictions =
  saye_msgs::msg::PedestrianPredictions_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_HPP_
