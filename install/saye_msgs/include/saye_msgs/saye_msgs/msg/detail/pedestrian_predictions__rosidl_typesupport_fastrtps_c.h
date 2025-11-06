// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice
#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "saye_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_serialize_saye_msgs__msg__PedestrianPredictions(
  const saye_msgs__msg__PedestrianPredictions * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_deserialize_saye_msgs__msg__PedestrianPredictions(
  eprosima::fastcdr::Cdr &,
  saye_msgs__msg__PedestrianPredictions * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t get_serialized_size_saye_msgs__msg__PedestrianPredictions(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t max_serialized_size_saye_msgs__msg__PedestrianPredictions(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_serialize_key_saye_msgs__msg__PedestrianPredictions(
  const saye_msgs__msg__PedestrianPredictions * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t get_serialized_size_key_saye_msgs__msg__PedestrianPredictions(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t max_serialized_size_key_saye_msgs__msg__PedestrianPredictions(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, saye_msgs, msg, PedestrianPredictions)();

#ifdef __cplusplus
}
#endif

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
