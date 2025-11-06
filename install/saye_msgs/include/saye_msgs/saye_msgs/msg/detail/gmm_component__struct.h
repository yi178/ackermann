// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "saye_msgs/msg/gmm_component.h"


#ifndef SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_H_
#define SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'mean_x'
// Member 'mean_y'
// Member 'cov_xx'
// Member 'cov_yy'
// Member 'cov_xy'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/GMMComponent in the package saye_msgs.
/**
  * Gaussian Mixture Model Component
  * Represents one mode in a multi-modal prediction
  * 高斯混合模型的一个分量
 */
typedef struct saye_msgs__msg__GMMComponent
{
  /// Weight of this mode, sum of all weights = 1.0
  /// 该模式的权重，所有权重之和为1.0
  double weight;
  /// Predicted mean trajectory (56 time steps, dt=0.05s, total 2.8s)
  /// 预测的均值轨迹（56个时间步，dt=0.05秒，总共2.8秒）
  /// X position means
  rosidl_runtime_c__double__Sequence mean_x;
  /// Y position means
  rosidl_runtime_c__double__Sequence mean_y;
  /// Predicted covariance (uncertainty grows with time)
  /// 预测的协方差（不确定性随时间增长）
  /// Variance in X direction
  rosidl_runtime_c__double__Sequence cov_xx;
  /// Variance in Y direction
  rosidl_runtime_c__double__Sequence cov_yy;
  /// Covariance between X and Y
  rosidl_runtime_c__double__Sequence cov_xy;
} saye_msgs__msg__GMMComponent;

// Struct for a sequence of saye_msgs__msg__GMMComponent.
typedef struct saye_msgs__msg__GMMComponent__Sequence
{
  saye_msgs__msg__GMMComponent * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} saye_msgs__msg__GMMComponent__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SAYE_MSGS__MSG__DETAIL__GMM_COMPONENT__STRUCT_H_
