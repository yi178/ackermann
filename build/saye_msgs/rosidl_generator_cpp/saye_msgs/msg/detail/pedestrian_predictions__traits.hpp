// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_predictions.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__TRAITS_HPP_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "saye_msgs/msg/detail/pedestrian_predictions__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'predictions'
#include "saye_msgs/msg/detail/pedestrian_prediction__traits.hpp"

namespace saye_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const PedestrianPredictions & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: predictions
  {
    if (msg.predictions.size() == 0) {
      out << "predictions: []";
    } else {
      out << "predictions: [";
      size_t pending_items = msg.predictions.size();
      for (auto item : msg.predictions) {
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
  const PedestrianPredictions & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: predictions
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.predictions.size() == 0) {
      out << "predictions: []\n";
    } else {
      out << "predictions:\n";
      for (auto item : msg.predictions) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PedestrianPredictions & msg, bool use_flow_style = false)
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
  const saye_msgs::msg::PedestrianPredictions & msg,
  std::ostream & out, size_t indentation = 0)
{
  saye_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use saye_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const saye_msgs::msg::PedestrianPredictions & msg)
{
  return saye_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<saye_msgs::msg::PedestrianPredictions>()
{
  return "saye_msgs::msg::PedestrianPredictions";
}

template<>
inline const char * name<saye_msgs::msg::PedestrianPredictions>()
{
  return "saye_msgs/msg/PedestrianPredictions";
}

template<>
struct has_fixed_size<saye_msgs::msg::PedestrianPredictions>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<saye_msgs::msg::PedestrianPredictions>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<saye_msgs::msg::PedestrianPredictions>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__TRAITS_HPP_
