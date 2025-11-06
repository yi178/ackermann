// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"
#include "saye_msgs/msg/detail/pedestrian_prediction__struct.hpp"
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

void PedestrianPrediction_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) saye_msgs::msg::PedestrianPrediction(_init);
}

void PedestrianPrediction_fini_function(void * message_memory)
{
  auto typed_message = static_cast<saye_msgs::msg::PedestrianPrediction *>(message_memory);
  typed_message->~PedestrianPrediction();
}

size_t size_function__PedestrianPrediction__gmm_modes(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<saye_msgs::msg::GMMComponent> *>(untyped_member);
  return member->size();
}

const void * get_const_function__PedestrianPrediction__gmm_modes(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<saye_msgs::msg::GMMComponent> *>(untyped_member);
  return &member[index];
}

void * get_function__PedestrianPrediction__gmm_modes(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<saye_msgs::msg::GMMComponent> *>(untyped_member);
  return &member[index];
}

void fetch_function__PedestrianPrediction__gmm_modes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const saye_msgs::msg::GMMComponent *>(
    get_const_function__PedestrianPrediction__gmm_modes(untyped_member, index));
  auto & value = *reinterpret_cast<saye_msgs::msg::GMMComponent *>(untyped_value);
  value = item;
}

void assign_function__PedestrianPrediction__gmm_modes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<saye_msgs::msg::GMMComponent *>(
    get_function__PedestrianPrediction__gmm_modes(untyped_member, index));
  const auto & value = *reinterpret_cast<const saye_msgs::msg::GMMComponent *>(untyped_value);
  item = value;
}

void resize_function__PedestrianPrediction__gmm_modes(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<saye_msgs::msg::GMMComponent> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember PedestrianPrediction_message_member_array[4] = {
  {
    "id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPrediction, id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "current_pose",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<geometry_msgs::msg::Pose>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPrediction, current_pose),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "current_speed",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPrediction, current_speed),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "gmm_modes",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<saye_msgs::msg::GMMComponent>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPrediction, gmm_modes),  // bytes offset in struct
    nullptr,  // default value
    size_function__PedestrianPrediction__gmm_modes,  // size() function pointer
    get_const_function__PedestrianPrediction__gmm_modes,  // get_const(index) function pointer
    get_function__PedestrianPrediction__gmm_modes,  // get(index) function pointer
    fetch_function__PedestrianPrediction__gmm_modes,  // fetch(index, &value) function pointer
    assign_function__PedestrianPrediction__gmm_modes,  // assign(index, value) function pointer
    resize_function__PedestrianPrediction__gmm_modes  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers PedestrianPrediction_message_members = {
  "saye_msgs::msg",  // message namespace
  "PedestrianPrediction",  // message name
  4,  // number of fields
  sizeof(saye_msgs::msg::PedestrianPrediction),
  false,  // has_any_key_member_
  PedestrianPrediction_message_member_array,  // message members
  PedestrianPrediction_init_function,  // function to initialize message memory (memory has to be allocated)
  PedestrianPrediction_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t PedestrianPrediction_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &PedestrianPrediction_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__PedestrianPrediction__get_type_hash,
  &saye_msgs__msg__PedestrianPrediction__get_type_description,
  &saye_msgs__msg__PedestrianPrediction__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace saye_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<saye_msgs::msg::PedestrianPrediction>()
{
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::PedestrianPrediction_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, saye_msgs, msg, PedestrianPrediction)() {
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::PedestrianPrediction_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
