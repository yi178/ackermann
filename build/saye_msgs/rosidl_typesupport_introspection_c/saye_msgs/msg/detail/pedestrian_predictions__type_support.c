// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "saye_msgs/msg/detail/pedestrian_predictions__rosidl_typesupport_introspection_c.h"
#include "saye_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__functions.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `predictions`
#include "saye_msgs/msg/pedestrian_prediction.h"
// Member `predictions`
#include "saye_msgs/msg/detail/pedestrian_prediction__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  saye_msgs__msg__PedestrianPredictions__init(message_memory);
}

void saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_fini_function(void * message_memory)
{
  saye_msgs__msg__PedestrianPredictions__fini(message_memory);
}

size_t saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__size_function__PedestrianPredictions__predictions(
  const void * untyped_member)
{
  const saye_msgs__msg__PedestrianPrediction__Sequence * member =
    (const saye_msgs__msg__PedestrianPrediction__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_const_function__PedestrianPredictions__predictions(
  const void * untyped_member, size_t index)
{
  const saye_msgs__msg__PedestrianPrediction__Sequence * member =
    (const saye_msgs__msg__PedestrianPrediction__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_function__PedestrianPredictions__predictions(
  void * untyped_member, size_t index)
{
  saye_msgs__msg__PedestrianPrediction__Sequence * member =
    (saye_msgs__msg__PedestrianPrediction__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__fetch_function__PedestrianPredictions__predictions(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const saye_msgs__msg__PedestrianPrediction * item =
    ((const saye_msgs__msg__PedestrianPrediction *)
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_const_function__PedestrianPredictions__predictions(untyped_member, index));
  saye_msgs__msg__PedestrianPrediction * value =
    (saye_msgs__msg__PedestrianPrediction *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__assign_function__PedestrianPredictions__predictions(
  void * untyped_member, size_t index, const void * untyped_value)
{
  saye_msgs__msg__PedestrianPrediction * item =
    ((saye_msgs__msg__PedestrianPrediction *)
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_function__PedestrianPredictions__predictions(untyped_member, index));
  const saye_msgs__msg__PedestrianPrediction * value =
    (const saye_msgs__msg__PedestrianPrediction *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__resize_function__PedestrianPredictions__predictions(
  void * untyped_member, size_t size)
{
  saye_msgs__msg__PedestrianPrediction__Sequence * member =
    (saye_msgs__msg__PedestrianPrediction__Sequence *)(untyped_member);
  saye_msgs__msg__PedestrianPrediction__Sequence__fini(member);
  return saye_msgs__msg__PedestrianPrediction__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPredictions, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "predictions",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPredictions, predictions),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__size_function__PedestrianPredictions__predictions,  // size() function pointer
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_const_function__PedestrianPredictions__predictions,  // get_const(index) function pointer
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__get_function__PedestrianPredictions__predictions,  // get(index) function pointer
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__fetch_function__PedestrianPredictions__predictions,  // fetch(index, &value) function pointer
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__assign_function__PedestrianPredictions__predictions,  // assign(index, value) function pointer
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__resize_function__PedestrianPredictions__predictions  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_members = {
  "saye_msgs__msg",  // message namespace
  "PedestrianPredictions",  // message name
  2,  // number of fields
  sizeof(saye_msgs__msg__PedestrianPredictions),
  false,  // has_any_key_member_
  saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_member_array,  // message members
  saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_init_function,  // function to initialize message memory (memory has to be allocated)
  saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_type_support_handle = {
  0,
  &saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__PedestrianPredictions__get_type_hash,
  &saye_msgs__msg__PedestrianPredictions__get_type_description,
  &saye_msgs__msg__PedestrianPredictions__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_saye_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, saye_msgs, msg, PedestrianPredictions)() {
  saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, saye_msgs, msg, PedestrianPrediction)();
  if (!saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_type_support_handle.typesupport_identifier) {
    saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &saye_msgs__msg__PedestrianPredictions__rosidl_typesupport_introspection_c__PedestrianPredictions_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
