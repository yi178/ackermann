// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "saye_msgs/msg/detail/gmm_component__functions.h"
#include "saye_msgs/msg/detail/gmm_component__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace saye_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void GMMComponent_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) saye_msgs::msg::GMMComponent(_init);
}

void GMMComponent_fini_function(void * message_memory)
{
  auto typed_message = static_cast<saye_msgs::msg::GMMComponent *>(message_memory);
  typed_message->~GMMComponent();
}

size_t size_function__GMMComponent__mean_x(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GMMComponent__mean_x(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__GMMComponent__mean_x(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__GMMComponent__mean_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__GMMComponent__mean_x(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__GMMComponent__mean_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__GMMComponent__mean_x(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__GMMComponent__mean_x(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

size_t size_function__GMMComponent__mean_y(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GMMComponent__mean_y(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__GMMComponent__mean_y(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__GMMComponent__mean_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__GMMComponent__mean_y(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__GMMComponent__mean_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__GMMComponent__mean_y(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__GMMComponent__mean_y(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

size_t size_function__GMMComponent__cov_xx(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GMMComponent__cov_xx(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__GMMComponent__cov_xx(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__GMMComponent__cov_xx(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__GMMComponent__cov_xx(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__GMMComponent__cov_xx(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__GMMComponent__cov_xx(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__GMMComponent__cov_xx(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

size_t size_function__GMMComponent__cov_yy(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GMMComponent__cov_yy(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__GMMComponent__cov_yy(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__GMMComponent__cov_yy(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__GMMComponent__cov_yy(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__GMMComponent__cov_yy(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__GMMComponent__cov_yy(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__GMMComponent__cov_yy(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

size_t size_function__GMMComponent__cov_xy(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GMMComponent__cov_xy(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__GMMComponent__cov_xy(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__GMMComponent__cov_xy(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__GMMComponent__cov_xy(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__GMMComponent__cov_xy(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__GMMComponent__cov_xy(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__GMMComponent__cov_xy(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember GMMComponent_message_member_array[6] = {
  {
    "weight",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, weight),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "mean_x",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, mean_x),  // bytes offset in struct
    nullptr,  // default value
    size_function__GMMComponent__mean_x,  // size() function pointer
    get_const_function__GMMComponent__mean_x,  // get_const(index) function pointer
    get_function__GMMComponent__mean_x,  // get(index) function pointer
    fetch_function__GMMComponent__mean_x,  // fetch(index, &value) function pointer
    assign_function__GMMComponent__mean_x,  // assign(index, value) function pointer
    resize_function__GMMComponent__mean_x  // resize(index) function pointer
  },
  {
    "mean_y",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, mean_y),  // bytes offset in struct
    nullptr,  // default value
    size_function__GMMComponent__mean_y,  // size() function pointer
    get_const_function__GMMComponent__mean_y,  // get_const(index) function pointer
    get_function__GMMComponent__mean_y,  // get(index) function pointer
    fetch_function__GMMComponent__mean_y,  // fetch(index, &value) function pointer
    assign_function__GMMComponent__mean_y,  // assign(index, value) function pointer
    resize_function__GMMComponent__mean_y  // resize(index) function pointer
  },
  {
    "cov_xx",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, cov_xx),  // bytes offset in struct
    nullptr,  // default value
    size_function__GMMComponent__cov_xx,  // size() function pointer
    get_const_function__GMMComponent__cov_xx,  // get_const(index) function pointer
    get_function__GMMComponent__cov_xx,  // get(index) function pointer
    fetch_function__GMMComponent__cov_xx,  // fetch(index, &value) function pointer
    assign_function__GMMComponent__cov_xx,  // assign(index, value) function pointer
    resize_function__GMMComponent__cov_xx  // resize(index) function pointer
  },
  {
    "cov_yy",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, cov_yy),  // bytes offset in struct
    nullptr,  // default value
    size_function__GMMComponent__cov_yy,  // size() function pointer
    get_const_function__GMMComponent__cov_yy,  // get_const(index) function pointer
    get_function__GMMComponent__cov_yy,  // get(index) function pointer
    fetch_function__GMMComponent__cov_yy,  // fetch(index, &value) function pointer
    assign_function__GMMComponent__cov_yy,  // assign(index, value) function pointer
    resize_function__GMMComponent__cov_yy  // resize(index) function pointer
  },
  {
    "cov_xy",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::GMMComponent, cov_xy),  // bytes offset in struct
    nullptr,  // default value
    size_function__GMMComponent__cov_xy,  // size() function pointer
    get_const_function__GMMComponent__cov_xy,  // get_const(index) function pointer
    get_function__GMMComponent__cov_xy,  // get(index) function pointer
    fetch_function__GMMComponent__cov_xy,  // fetch(index, &value) function pointer
    assign_function__GMMComponent__cov_xy,  // assign(index, value) function pointer
    resize_function__GMMComponent__cov_xy  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers GMMComponent_message_members = {
  "saye_msgs::msg",  // message namespace
  "GMMComponent",  // message name
  6,  // number of fields
  sizeof(saye_msgs::msg::GMMComponent),
  false,  // has_any_key_member_
  GMMComponent_message_member_array,  // message members
  GMMComponent_init_function,  // function to initialize message memory (memory has to be allocated)
  GMMComponent_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t GMMComponent_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &GMMComponent_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__GMMComponent__get_type_hash,
  &saye_msgs__msg__GMMComponent__get_type_description,
  &saye_msgs__msg__GMMComponent__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace saye_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<saye_msgs::msg::GMMComponent>()
{
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::GMMComponent_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, saye_msgs, msg, GMMComponent)() {
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::GMMComponent_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
