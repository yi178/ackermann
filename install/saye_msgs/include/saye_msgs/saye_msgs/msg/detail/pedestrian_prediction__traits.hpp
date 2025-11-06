// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_prediction.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__TRAITS_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "saye_msgs/msg/detail/pedestrian_prediction__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'current_pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"
// Member 'gmm_modes'
#include "saye_msgs/msg/detail/gmm_component__traits.hpp"

namespace saye_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const PedestrianPrediction & msg,
  std::ostream & out)
{
  out << "{";
  // member: id
  {
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << ", ";
  }

  // member: current_pose
  {
    out << "current_pose: ";
    to_flow_style_yaml(msg.current_pose, out);
    out << ", ";
  }

  // member: current_speed
  {
    out << "current_speed: ";
    rosidl_generator_traits::value_to_yaml(msg.current_speed, out);
    out << ", ";
  }

  // member: gmm_modes
  {
    if (msg.gmm_modes.size() == 0) {
      out << "gmm_modes: []";
    } else {
      out << "gmm_modes: [";
      size_t pending_items = msg.gmm_modes.size();
      for (auto item : msg.gmm_modes) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PedestrianPrediction & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "id: ";
    rosidl_generator_traits::value_to_yaml(msg.id, out);
    out << "\n";
  }

  // member: current_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_pose:\n";
    to_block_style_yaml(msg.current_pose, out, indentation + 2);
  }

  // member: current_speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_speed: ";
    rosidl_generator_traits::value_to_yaml(msg.current_speed, out);
    out << "\n";
  }

  // member: gmm_modes
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.gmm_modes.size() == 0) {
      out << "gmm_modes: []\n";
    } else {
      out << "gmm_modes:\n";
      for (auto item : msg.gmm_modes) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PedestrianPrediction & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace saye_msgs

namespace rosidl_generator_traits
{

[[deprecated("use saye_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const saye_msgs::msg::PedestrianPrediction & msg,
  std::ostream & out, size_t indentation = 0)
{
  saye_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use saye_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const saye_msgs::msg::PedestrianPrediction & msg)
{
  return saye_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<saye_msgs::msg::PedestrianPrediction>()
{
  return "saye_msgs::msg::PedestrianPrediction";
}

template<>
inline const char * name<saye_msgs::msg::PedestrianPrediction>()
{
  return "saye_msgs/msg/PedestrianPrediction";
}

template<>
struct has_fixed_size<saye_msgs::msg::PedestrianPrediction>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<saye_msgs::msg::PedestrianPrediction>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<saye_msgs::msg::PedestrianPrediction>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__TRAITS_HPP_
