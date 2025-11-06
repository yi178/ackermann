// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "saye_msgs/msg/detail/gmm_component__rosidl_typesupport_introspection_c.h"
#include "saye_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "saye_msgs/msg/detail/gmm_component__functions.h"
#include "saye_msgs/msg/detail/gmm_component__struct.h"


// Include directives for member types
// Member `mean_x`
// Member `mean_y`
// Member `cov_xx`
// Member `cov_yy`
// Member `cov_xy`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  saye_msgs__msg__GMMComponent__init(message_memory);
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_fini_function(void * message_memory)
{
  saye_msgs__msg__GMMComponent__fini(message_memory);
}

size_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__mean_x(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_x(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_x(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__mean_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_x(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__mean_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_x(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__mean_x(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__mean_y(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_y(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_y(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__mean_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_y(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__mean_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_y(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__mean_y(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_xx(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xx(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xx(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_xx(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xx(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_xx(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xx(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_xx(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_yy(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_yy(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_yy(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_yy(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_yy(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_yy(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_yy(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_yy(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_xy(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xy(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xy(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_xy(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xy(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_xy(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xy(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_xy(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_member_array[6] = {
  {
    "weight",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, weight),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mean_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, mean_x),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__mean_x,  // size() function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_x,  // get_const(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_x,  // get(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__mean_x,  // fetch(index, &value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__mean_x,  // assign(index, value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__mean_x  // resize(index) function pointer
  },
  {
    "mean_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, mean_y),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__mean_y,  // size() function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__mean_y,  // get_const(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__mean_y,  // get(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__mean_y,  // fetch(index, &value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__mean_y,  // assign(index, value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__mean_y  // resize(index) function pointer
  },
  {
    "cov_xx",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, cov_xx),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_xx,  // size() function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xx,  // get_const(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xx,  // get(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_xx,  // fetch(index, &value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_xx,  // assign(index, value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_xx  // resize(index) function pointer
  },
  {
    "cov_yy",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, cov_yy),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_yy,  // size() function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_yy,  // get_const(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_yy,  // get(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_yy,  // fetch(index, &value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_yy,  // assign(index, value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_yy  // resize(index) function pointer
  },
  {
    "cov_xy",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(saye_msgs__msg__GMMComponent, cov_xy),  // bytes offset in struct
    NULL,  // default value
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__size_function__GMMComponent__cov_xy,  // size() function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_const_function__GMMComponent__cov_xy,  // get_const(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__get_function__GMMComponent__cov_xy,  // get(index) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__fetch_function__GMMComponent__cov_xy,  // fetch(index, &value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__assign_function__GMMComponent__cov_xy,  // assign(index, value) function pointer
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__resize_function__GMMComponent__cov_xy  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_members = {
  "saye_msgs__msg",  // message namespace
  "GMMComponent",  // message name
  6,  // number of fields
  sizeof(saye_msgs__msg__GMMComponent),
  false,  // has_any_key_member_
  saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_member_array,  // message members
  saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_init_function,  // function to initialize message memory (memory has to be allocated)
  saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_type_support_handle = {
  0,
  &saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_members,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__GMMComponent__get_type_hash,
  &saye_msgs__msg__GMMComponent__get_type_description,
  &saye_msgs__msg__GMMComponent__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_saye_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, saye_msgs, msg, GMMComponent)() {
  if (!saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_type_support_handle.typesupport_identifier) {
    saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &saye_msgs__msg__GMMComponent__rosidl_typesupport_introspection_c__GMMComponent_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
