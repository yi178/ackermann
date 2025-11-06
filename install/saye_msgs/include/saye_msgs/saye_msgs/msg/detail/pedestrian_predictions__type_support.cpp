// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__functions.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__struct.hpp"
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

void PedestrianPredictions_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) saye_msgs::msg::PedestrianPredictions(_init);
}

void PedestrianPredictions_fini_function(void * message_memory)
{
  auto typed_message = static_cast<saye_msgs::msg::PedestrianPredictions *>(message_memory);
  typed_message->~PedestrianPredictions();
}

size_t size_function__PedestrianPredictions__predictions(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<saye_msgs::msg::PedestrianPrediction> *>(untyped_member);
  return member->size();
}

const void * get_const_function__PedestrianPredictions__predictions(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<saye_msgs::msg::PedestrianPrediction> *>(untyped_member);
  return &member[index];
}

void * get_function__PedestrianPredictions__predictions(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<saye_msgs::msg::PedestrianPrediction> *>(untyped_member);
  return &member[index];
}

void fetch_function__PedestrianPredictions__predictions(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const saye_msgs::msg::PedestrianPrediction *>(
    get_const_function__PedestrianPredictions__predictions(untyped_member, index));
  auto & value = *reinterpret_cast<saye_msgs::msg::PedestrianPrediction *>(untyped_value);
  value = item;
}

void assign_function__PedestrianPredictions__predictions(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<saye_msgs::msg::PedestrianPrediction *>(
    get_function__PedestrianPredictions__predictions(untyped_member, index));
  const auto & value = *reinterpret_cast<const saye_msgs::msg::PedestrianPrediction *>(untyped_value);
  item = value;
}

void resize_function__PedestrianPredictions__predictions(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<saye_msgs::msg::PedestrianPrediction> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember PedestrianPredictions_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPredictions, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "predictions",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<saye_msgs::msg::PedestrianPrediction>(),  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs::msg::PedestrianPredictions, predictions),  // bytes offset in struct
    nullptr,  // default value
    size_function__PedestrianPredictions__predictions,  // size() function pointer
    get_const_function__PedestrianPredictions__predictions,  // get_const(index) function pointer
    get_function__PedestrianPredictions__predictions,  // get(index) function pointer
    fetch_function__PedestrianPredictions__predictions,  // fetch(index, &value) function pointer
    assign_function__PedestrianPredictions__predictions,  // assign(index, value) function pointer
    resize_function__PedestrianPredictions__predictions  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers PedestrianPredictions_message_members = {
  "saye_msgs::msg",  // message namespace
  "PedestrianPredictions",  // message name
  2,  // number of fields
  sizeof(saye_msgs::msg::PedestrianPredictions),
  false,  // has_any_key_member_
  PedestrianPredictions_message_member_array,  // message members
  PedestrianPredictions_init_function,  // function to initialize message memory (memory has to be allocated)
  PedestrianPredictions_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t PedestrianPredictions_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &PedestrianPredictions_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__PedestrianPredictions__get_type_hash,
  &saye_msgs__msg__PedestrianPredictions__get_type_description,
  &saye_msgs__msg__PedestrianPredictions__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace saye_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<saye_msgs::msg::PedestrianPredictions>()
{
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::PedestrianPredictions_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, saye_msgs, msg, PedestrianPredictions)() {
  return &::saye_msgs::msg::rosidl_typesupport_introspection_cpp::PedestrianPredictions_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
