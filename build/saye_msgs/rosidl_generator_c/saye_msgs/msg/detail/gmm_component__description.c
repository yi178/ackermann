// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice

#include "saye_msgs/msg/detail/gmm_component__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_saye_msgs
const rosidl_type_hash_t *
saye_msgs__msg__GMMComponent__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x93, 0xc6, 0x33, 0x0a, 0x90, 0x8a, 0x09, 0xa8,
      0x88, 0x3b, 0xd9, 0x42, 0x04, 0x14, 0x41, 0xae,
      0x7a, 0x4a, 0xb8, 0xd1, 0xe3, 0xce, 0xd5, 0x31,
      0x40, 0x18, 0x41, 0x3f, 0x7a, 0xa4, 0xe1, 0xf3,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char saye_msgs__msg__GMMComponent__TYPE_NAME[] = "saye_msgs/msg/GMMComponent";

// Define type names, field names, and default values
static char saye_msgs__msg__GMMComponent__FIELD_NAME__weight[] = "weight";
static char saye_msgs__msg__GMMComponent__FIELD_NAME__mean_x[] = "mean_x";
static char saye_msgs__msg__GMMComponent__FIELD_NAME__mean_y[] = "mean_y";
static char saye_msgs__msg__GMMComponent__FIELD_NAME__cov_xx[] = "cov_xx";
static char saye_msgs__msg__GMMComponent__FIELD_NAME__cov_yy[] = "cov_yy";
static char saye_msgs__msg__GMMComponent__FIELD_NAME__cov_xy[] = "cov_xy";

static rosidl_runtime_c__type_description__Field saye_msgs__msg__GMMComponent__FIELDS[] = {
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__weight, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__mean_x, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__mean_y, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__cov_xx, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__cov_yy, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__FIELD_NAME__cov_xy, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
saye_msgs__msg__GMMComponent__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {saye_msgs__msg__GMMComponent__TYPE_NAME, 26, 26},
      {saye_msgs__msg__GMMComponent__FIELDS, 6, 6},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Gaussian Mixture Model Component\n"
  "# Represents one mode in a multi-modal prediction\n"
  "# \\xe9\\xab\\x98\\xe6\\x96\\xaf\\xe6\\xb7\\xb7\\xe5\\x90\\x88\\xe6\\xa8\\xa1\\xe5\\x9e\\x8b\\xe7\\x9a\\x84\\xe4\\xb8\\x80\\xe4\\xb8\\xaa\\xe5\\x88\\x86\\xe9\\x87\\x8f\n"
  "\n"
  "float64 weight              # Weight of this mode, sum of all weights = 1.0\n"
  "                            # \\xe8\\xaf\\xa5\\xe6\\xa8\\xa1\\xe5\\xbc\\x8f\\xe7\\x9a\\x84\\xe6\\x9d\\x83\\xe9\\x87\\x8d\\xef\\xbc\\x8c\\xe6\\x89\\x80\\xe6\\x9c\\x89\\xe6\\x9d\\x83\\xe9\\x87\\x8d\\xe4\\xb9\\x8b\\xe5\\x92\\x8c\\xe4\\xb8\\xba1.0\n"
  "\n"
  "# Predicted mean trajectory (56 time steps, dt=0.05s, total 2.8s)\n"
  "# \\xe9\\xa2\\x84\\xe6\\xb5\\x8b\\xe7\\x9a\\x84\\xe5\\x9d\\x87\\xe5\\x80\\xbc\\xe8\\xbd\\xa8\\xe8\\xbf\\xb9\\xef\\xbc\\x8856\\xe4\\xb8\\xaa\\xe6\\x97\\xb6\\xe9\\x97\\xb4\\xe6\\xad\\xa5\\xef\\xbc\\x8cdt=0.05\\xe7\\xa7\\x92\\xef\\xbc\\x8c\\xe6\\x80\\xbb\\xe5\\x85\\xb12.8\\xe7\\xa7\\x92\\xef\\xbc\\x89\n"
  "float64[] mean_x            # X position means [m]\n"
  "float64[] mean_y            # Y position means [m]\n"
  "\n"
  "# Predicted covariance (uncertainty grows with time)\n"
  "# \\xe9\\xa2\\x84\\xe6\\xb5\\x8b\\xe7\\x9a\\x84\\xe5\\x8d\\x8f\\xe6\\x96\\xb9\\xe5\\xb7\\xae\\xef\\xbc\\x88\\xe4\\xb8\\x8d\\xe7\\xa1\\xae\\xe5\\xae\\x9a\\xe6\\x80\\xa7\\xe9\\x9a\\x8f\\xe6\\x97\\xb6\\xe9\\x97\\xb4\\xe5\\xa2\\x9e\\xe9\\x95\\xbf\\xef\\xbc\\x89\n"
  "float64[] cov_xx            # Variance in X direction [m\\xc2\\xb2]\n"
  "float64[] cov_yy            # Variance in Y direction [m\\xc2\\xb2]\n"
  "float64[] cov_xy            # Covariance between X and Y [m\\xc2\\xb2]";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
saye_msgs__msg__GMMComponent__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {saye_msgs__msg__GMMComponent__TYPE_NAME, 26, 26},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 681, 681},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
saye_msgs__msg__GMMComponent__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *saye_msgs__msg__GMMComponent__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
