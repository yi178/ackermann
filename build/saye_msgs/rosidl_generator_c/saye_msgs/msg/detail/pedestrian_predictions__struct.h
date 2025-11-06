// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_predictions.h"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_H_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'predictions'
#include "saye_msgs/msg/detail/pedestrian_prediction__struct.h"

/// Struct defined in msg/PedestrianPredictions in the package saye_msgs.
/**
  * Collection of all pedestrian predictions
  * 所有行人预测的集合
 */
typedef struct saye_msgs__msg__PedestrianPredictions
{
  /// Timestamp for synchronization
  /// 用于同步的时间戳
  std_msgs__msg__Header header;
  /// List of all tracked pedestrians
  /// 所有被追踪行人的列表
  saye_msgs__msg__PedestrianPrediction__Sequence predictions;
} saye_msgs__msg__PedestrianPredictions;

// Struct for a sequence of saye_msgs__msg__PedestrianPredictions.
typedef struct saye_msgs__msg__PedestrianPredictions__Sequence
{
  saye_msgs__msg__PedestrianPredictions * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} saye_msgs__msg__PedestrianPredictions__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTIONS__STRUCT_H_
