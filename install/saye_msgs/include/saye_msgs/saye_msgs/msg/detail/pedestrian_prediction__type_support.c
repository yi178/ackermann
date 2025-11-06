// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "saye_msgs/msg/detail/pedestrian_prediction__rosidl_typesupport_introspection_c.h"
#include "saye_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"
#include "saye_msgs/msg/detail/pedestrian_prediction__struct.h"


// Include directives for member types
// Member `id`
#include "rosidl_runtime_c/string_functions.h"
// Member `current_pose`
#include "geometry_msgs/msg/pose.h"
// Member `current_pose`
#include "geometry_msgs/msg/detail/pose__rosidl_typesupport_introspection_c.h"
// Member `gmm_modes`
#include "saye_msgs/msg/gmm_component.h"
// Member `gmm_modes`
#include "saye_msgs/msg/detail/gmm_component__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  saye_msgs__msg__PedestrianPrediction__init(message_memory);
}

void saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_fini_function(void * message_memory)
{
  saye_msgs__msg__PedestrianPrediction__fini(message_memory);
}

size_t saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__size_function__PedestrianPrediction__gmm_modes(
  const void * untyped_member)
{
  const saye_msgs__msg__GMMComponent__Sequence * member =
    (const saye_msgs__msg__GMMComponent__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_const_function__PedestrianPrediction__gmm_modes(
  const void * untyped_member, size_t index)
{
  const saye_msgs__msg__GMMComponent__Sequence * member =
    (const saye_msgs__msg__GMMComponent__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_function__PedestrianPrediction__gmm_modes(
  void * untyped_member, size_t index)
{
  saye_msgs__msg__GMMComponent__Sequence * member =
    (saye_msgs__msg__GMMComponent__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__fetch_function__PedestrianPrediction__gmm_modes(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const saye_msgs__msg__GMMComponent * item =
    ((const saye_msgs__msg__GMMComponent *)
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_const_function__PedestrianPrediction__gmm_modes(untyped_member, index));
  saye_msgs__msg__GMMComponent * value =
    (saye_msgs__msg__GMMComponent *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__assign_function__PedestrianPrediction__gmm_modes(
  void * untyped_member, size_t index, const void * untyped_value)
{
  saye_msgs__msg__GMMComponent * item =
    ((saye_msgs__msg__GMMComponent *)
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_function__PedestrianPrediction__gmm_modes(untyped_member, index));
  const saye_msgs__msg__GMMComponent * value =
    (const saye_msgs__msg__GMMComponent *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__resize_function__PedestrianPrediction__gmm_modes(
  void * untyped_member, size_t size)
{
  saye_msgs__msg__GMMComponent__Sequence * member =
    (saye_msgs__msg__GMMComponent__Sequence *)(untyped_member);
  saye_msgs__msg__GMMComponent__Sequence__fini(member);
  return saye_msgs__msg__GMMComponent__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_member_array[4] = {
  {
    "id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPrediction, id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_pose",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPrediction, current_pose),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_speed",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPrediction, current_speed),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "gmm_modes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__PedestrianPrediction, gmm_modes),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__size_function__PedestrianPrediction__gmm_modes,  // size() function pointer
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_const_function__PedestrianPrediction__gmm_modes,  // get_const(index) function pointer
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__get_function__PedestrianPrediction__gmm_modes,  // get(index) function pointer
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__fetch_function__PedestrianPrediction__gmm_modes,  // fetch(index, &value) function pointer
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__assign_function__PedestrianPrediction__gmm_modes,  // assign(index, value) function pointer
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__resize_function__PedestrianPrediction__gmm_modes  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_members = {
  "saye_msgs__msg",  // message namespace
  "PedestrianPrediction",  // message name
  4,  // number of fields
  sizeof(saye_msgs__msg__PedestrianPrediction),
  false,  // has_any_key_member_
  saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_member_array,  // message members
  saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_init_function,  // function to initialize message memory (memory has to be allocated)
  saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_type_support_handle = {
  0,
  &saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__PedestrianPrediction__get_type_hash,
  &saye_msgs__msg__PedestrianPrediction__get_type_description,
  &saye_msgs__msg__PedestrianPrediction__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_saye_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, saye_msgs, msg, PedestrianPrediction)() {
  saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Pose)();
  saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, saye_msgs, msg, GMMComponent)();
  if (!saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_type_support_handle.typesupport_identifier) {
    saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &saye_msgs__msg__PedestrianPrediction__rosidl_typesupport_introspection_c__PedestrianPrediction_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
