// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/pedestrian_prediction.h"


#ifndef SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_H_
#define SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'id'
#include "rosidl_runtime_c/string.h"
// Member 'current_pose'
#include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'gmm_modes'
#include "saye_msgs/msg/detail/gmm_component__struct.h"

/// Struct defined in msg/PedestrianPrediction in the package saye_msgs.
/**
  * Prediction for a single pedestrian
  * 单个行人的预测
 */
typedef struct saye_msgs__msg__PedestrianPrediction
{
  /// Unique identifier (e.g., "walker_center")
  /// 唯一标识符（例如："walker_center"）
  rosidl_runtime_c__String id;
  /// Current observed pose
  /// 当前观测位姿
  geometry_msgs__msg__Pose current_pose;
  /// Estimated current speed [m/s]
  /// 估计的当前速度 [m/s]
  double current_speed;
  /// GMM representation of future trajectory
  /// 未来轨迹的GMM表示
  /// For CV model: K=1 (single mode)
  /// For IMM model: K=3 (CV, CA, CT modes)
  /// CV模型：K=1（单模态）
  /// IMM模型：K=3（CV、CA、CT模式）
  saye_msgs__msg__GMMComponent__Sequence gmm_modes;
} saye_msgs__msg__PedestrianPrediction;

// Struct for a sequence of saye_msgs__msg__PedestrianPrediction.
typedef struct saye_msgs__msg__PedestrianPrediction__Sequence
{
  saye_msgs__msg__PedestrianPrediction * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} saye_msgs__msg__PedestrianPrediction__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SAYE_MSGS__MSG__DETAIL__PEDESTRIAN_PREDICTION__STRUCT_H_
