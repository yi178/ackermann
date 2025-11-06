// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_prediction.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'current_pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"
// Member 'gmm_modes'
#include "saye_msgs/msg/detail/gmm_component__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__saye_msgs__msg__PedestrianPrediction __attribute__((deprecated))
#else
# define DEPRECATED__saye_msgs__msg__PedestrianPrediction __declspec(deprecated)
#endif

namespace saye_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct PedestrianPrediction_
{
  using Type = PedestrianPrediction_<ContainerAllocator>;

  explicit PedestrianPrediction_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : current_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = "";
      this->current_speed = 0.0;
    }
  }

  explicit PedestrianPrediction_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : id(_alloc),
    current_pose(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->id = "";
      this->current_speed = 0.0;
    }
  }

  // field types and members
  using _id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _id_type id;
  using _current_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _current_pose_type current_pose;
  using _current_speed_type =
    double;
  _current_speed_type current_speed;
  using _gmm_modes_type =
    std::vector<saye_msgs::msg::GMMComponent_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<saye_msgs::msg::GMMComponent_<ContainerAllocator>>>;
  _gmm_modes_type gmm_modes;

  // setters for named parameter idiom
  Type & set__id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->id = _arg;
    return *this;
  }
  Type & set__current_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->current_pose = _arg;
    return *this;
  }
  Type & set__current_speed(
    const double & _arg)
  {
    this->current_speed = _arg;
    return *this;
  }
  Type & set__gmm_modes(
    const std::vector<saye_msgs::msg::GMMComponent_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<saye_msgs::msg::GMMComponent_<ContainerAllocator>>> & _arg)
  {
    this->gmm_modes = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> *;
  using ConstRawPtr =
    const saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__saye_msgs__msg__PedestrianPrediction
    std::shared_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__saye_msgs__msg__PedestrianPrediction
    std::shared_ptr<saye_msgs::msg::PedestrianPrediction_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PedestrianPrediction_ & other) const
  {
    if (this->id != other.id) {
      return false;
    }
    if (this->current_pose != other.current_pose) {
      return false;
    }
    if (this->current_speed != other.current_speed) {
      return false;
    }
    if (this->gmm_modes != other.gmm_modes) {
      return false;
    }
    return true;
  }
  bool operator!=(const PedestrianPrediction_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PedestrianPrediction_

// alias to use template instance with default allocator
using PedestrianPrediction =
  saye_msgs::msg::PedestrianPrediction_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace saye_msgs

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_HPP_
