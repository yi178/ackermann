// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/gmm_component.hpp"


#ifndef SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__TRAITS_HPP_
#define SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "saye_msgs/msg/detail/gmm_component__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace saye_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const GMMComponent & msg,
  std::ostream & out)
{
  out << "{";
  // member: weight
  {
    out << "weight: ";
    rosidl_generator_traits::value_to_yaml(msg.weight, out);
    out << ", ";
  }

  // member: mean_x
  {
    if (msg.mean_x.size() == 0) {
      out << "mean_x: []";
    } else {
      out << "mean_x: [";
      size_t pending_items = msg.mean_x.size();
      for (auto item : msg.mean_x) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: mean_y
  {
    if (msg.mean_y.size() == 0) {
      out << "mean_y: []";
    } else {
      out << "mean_y: [";
      size_t pending_items = msg.mean_y.size();
      for (auto item : msg.mean_y) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: cov_xx
  {
    if (msg.cov_xx.size() == 0) {
      out << "cov_xx: []";
    } else {
      out << "cov_xx: [";
      size_t pending_items = msg.cov_xx.size();
      for (auto item : msg.cov_xx) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: cov_yy
  {
    if (msg.cov_yy.size() == 0) {
      out << "cov_yy: []";
    } else {
      out << "cov_yy: [";
      size_t pending_items = msg.cov_yy.size();
      for (auto item : msg.cov_yy) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: cov_xy
  {
    if (msg.cov_xy.size() == 0) {
      out << "cov_xy: []";
    } else {
      out << "cov_xy: [";
      size_t pending_items = msg.cov_xy.size();
      for (auto item : msg.cov_xy) {
        rosidl_generator_traits::value_to_yaml(item, out);
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
  const GMMComponent & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: weight
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "weight: ";
    rosidl_generator_traits::value_to_yaml(msg.weight, out);
    out << "\n";
  }

  // member: mean_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.mean_x.size() == 0) {
      out << "mean_x: []\n";
    } else {
      out << "mean_x:\n";
      for (auto item : msg.mean_x) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: mean_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.mean_y.size() == 0) {
      out << "mean_y: []\n";
    } else {
      out << "mean_y:\n";
      for (auto item : msg.mean_y) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: cov_xx
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.cov_xx.size() == 0) {
      out << "cov_xx: []\n";
    } else {
      out << "cov_xx:\n";
      for (auto item : msg.cov_xx) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: cov_yy
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.cov_yy.size() == 0) {
      out << "cov_yy: []\n";
    } else {
      out << "cov_yy:\n";
      for (auto item : msg.cov_yy) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: cov_xy
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.cov_xy.size() == 0) {
      out << "cov_xy: []\n";
    } else {
      out << "cov_xy:\n";
      for (auto item : msg.cov_xy) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GMMComponent & msg, bool use_flow_style = false)
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
  const saye_msgs::msg::GMMComponent & msg,
  std::ostream & out, size_t indentation = 0)
{
  saye_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use saye_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const saye_msgs::msg::GMMComponent & msg)
{
  return saye_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<saye_msgs::msg::GMMComponent>()
{
  return "saye_msgs::msg::GMMComponent";
}

template<>
inline const char * name<saye_msgs::msg::GMMComponent>()
{
  return "saye_msgs/msg/GMMComponent";
}

template<>
struct has_fixed_size<saye_msgs::msg::GMMComponent>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<saye_msgs::msg::GMMComponent>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<saye_msgs::msg::GMMComponent>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__TRAITS_HPP_
