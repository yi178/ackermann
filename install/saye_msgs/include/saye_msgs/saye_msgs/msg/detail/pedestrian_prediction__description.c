// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice

#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_saye_msgs
const rosidl_type_hash_t *
saye_msgs__msg__PedestrianPrediction__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x6c, 0x4e, 0x19, 0xd0, 0x6a, 0xcc, 0xae, 0x66,
      0x51, 0x94, 0x08, 0xe2, 0xff, 0x0a, 0x31, 0x00,
      0x7f, 0xf5, 0xb0, 0xa6, 0x59, 0x03, 0xf4, 0x4c,
      0x00, 0xef, 0x7f, 0xf3, 0x13, 0xe3, 0xf2, 0xc0,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "geometry_msgs/msg/detail/point__functions.h"
#include "saye_msgs/msg/detail/gmm_component__functions.h"
#include "geometry_msgs/msg/detail/quaternion__functions.h"
#include "geometry_msgs/msg/detail/pose__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t geometry_msgs__msg__Point__EXPECTED_HASH = {1, {
    0x69, 0x63, 0x08, 0x48, 0x42, 0xa9, 0xb0, 0x44,
    0x94, 0xd6, 0xb2, 0x94, 0x1d, 0x11, 0x44, 0x47,
    0x08, 0xd8, 0x92, 0xda, 0x2f, 0x4b, 0x09, 0x84,
    0x3b, 0x9c, 0x43, 0xf4, 0x2a, 0x7f, 0x68, 0x81,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Pose__EXPECTED_HASH = {1, {
    0xd5, 0x01, 0x95, 0x4e, 0x94, 0x76, 0xce, 0xa2,
    0x99, 0x69, 0x84, 0xe8, 0x12, 0x05, 0x4b, 0x68,
    0x02, 0x6a, 0xe0, 0xbf, 0xae, 0x78, 0x9d, 0x9a,
    0x10, 0xb2, 0x3d, 0xaf, 0x35, 0xcc, 0x90, 0xfa,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Quaternion__EXPECTED_HASH = {1, {
    0x8a, 0x76, 0x5f, 0x66, 0x77, 0x8c, 0x8f, 0xf7,
    0xc8, 0xab, 0x94, 0xaf, 0xcc, 0x59, 0x0a, 0x2e,
    0xd5, 0x32, 0x5a, 0x1d, 0x9a, 0x07, 0x6f, 0xff,
    0xf3, 0x8f, 0xbc, 0xe3, 0x6f, 0x45, 0x86, 0x84,
  }};
static const rosidl_type_hash_t saye_msgs__msg__GMMComponent__EXPECTED_HASH = {1, {
    0x93, 0xc6, 0x33, 0x0a, 0x90, 0x8a, 0x09, 0xa8,
    0x88, 0x3b, 0xd9, 0x42, 0x04, 0x14, 0x41, 0xae,
    0x7a, 0x4a, 0xb8, 0xd1, 0xe3, 0xce, 0xd5, 0x31,
    0x40, 0x18, 0x41, 0x3f, 0x7a, 0xa4, 0xe1, 0xf3,
  }};
#endif

static char saye_msgs__msg__PedestrianPrediction__TYPE_NAME[] = "saye_msgs/msg/PedestrianPrediction";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";
static char geometry_msgs__msg__Pose__TYPE_NAME[] = "geometry_msgs/msg/Pose";
static char geometry_msgs__msg__Quaternion__TYPE_NAME[] = "geometry_msgs/msg/Quaternion";
static char saye_msgs__msg__GMMComponent__TYPE_NAME[] = "saye_msgs/msg/GMMComponent";

// Define type names, field names, and default values
static char saye_msgs__msg__PedestrianPrediction__FIELD_NAME__id[] = "id";
static char saye_msgs__msg__PedestrianPrediction__FIELD_NAME__current_pose[] = "current_pose";
static char saye_msgs__msg__PedestrianPrediction__FIELD_NAME__current_speed[] = "current_speed";
static char saye_msgs__msg__PedestrianPrediction__FIELD_NAME__gmm_modes[] = "gmm_modes";

static rosidl_runtime_c__type_description__Field saye_msgs__msg__PedestrianPrediction__FIELDS[] = {
  {
    {saye_msgs__msg__PedestrianPrediction__FIELD_NAME__id, 2, 2},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__PedestrianPrediction__FIELD_NAME__current_pose, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {geometry_msgs__msg__Pose__TYPE_NAME, 22, 22},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__PedestrianPrediction__FIELD_NAME__current_speed, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__PedestrianPrediction__FIELD_NAME__gmm_modes, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {saye_msgs__msg__GMMComponent__TYPE_NAME, 26, 26},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription saye_msgs__msg__PedestrianPrediction__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Pose__TYPE_NAME, 22, 22},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Quaternion__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {saye_msgs__msg__GMMComponent__TYPE_NAME, 26, 26},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
saye_msgs__msg__PedestrianPrediction__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {saye_msgs__msg__PedestrianPrediction__TYPE_NAME, 34, 34},
      {saye_msgs__msg__PedestrianPrediction__FIELDS, 4, 4},
    },
    {saye_msgs__msg__PedestrianPrediction__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Pose__EXPECTED_HASH, geometry_msgs__msg__Pose__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Pose__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Quaternion__EXPECTED_HASH, geometry_msgs__msg__Quaternion__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = geometry_msgs__msg__Quaternion__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&saye_msgs__msg__GMMComponent__EXPECTED_HASH, saye_msgs__msg__GMMComponent__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = saye_msgs__msg__GMMComponent__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Prediction for a single pedestrian\n"
  "# \\xe5\\x8d\\x95\\xe4\\xb8\\xaa\\xe8\\xa1\\x8c\\xe4\\xba\\xba\\xe7\\x9a\\x84\\xe9\\xa2\\x84\\xe6\\xb5\\x8b\n"
  "\n"
  "string id                           # Unique identifier (e.g., \"walker_center\")\n"
  "                                    # \\xe5\\x94\\xaf\\xe4\\xb8\\x80\\xe6\\xa0\\x87\\xe8\\xaf\\x86\\xe7\\xac\\xa6\\xef\\xbc\\x88\\xe4\\xbe\\x8b\\xe5\\xa6\\x82\\xef\\xbc\\x9a\"walker_center\"\\xef\\xbc\\x89\n"
  "\n"
  "geometry_msgs/Pose current_pose     # Current observed pose\n"
  "                                    # \\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe8\\xa7\\x82\\xe6\\xb5\\x8b\\xe4\\xbd\\x8d\\xe5\\xa7\\xbf\n"
  "\n"
  "float64 current_speed               # Estimated current speed [m/s]\n"
  "                                    # \\xe4\\xbc\\xb0\\xe8\\xae\\xa1\\xe7\\x9a\\x84\\xe5\\xbd\\x93\\xe5\\x89\\x8d\\xe9\\x80\\x9f\\xe5\\xba\\xa6 [m/s]\n"
  "\n"
  "# GMM representation of future trajectory\n"
  "# \\xe6\\x9c\\xaa\\xe6\\x9d\\xa5\\xe8\\xbd\\xa8\\xe8\\xbf\\xb9\\xe7\\x9a\\x84GMM\\xe8\\xa1\\xa8\\xe7\\xa4\\xba\n"
  "# For CV model: K=1 (single mode)\n"
  "# For IMM model: K=3 (CV, CA, CT modes)\n"
  "# CV\\xe6\\xa8\\xa1\\xe5\\x9e\\x8b\\xef\\xbc\\x9aK=1\\xef\\xbc\\x88\\xe5\\x8d\\x95\\xe6\\xa8\\xa1\\xe6\\x80\\x81\\xef\\xbc\\x89\n"
  "# IMM\\xe6\\xa8\\xa1\\xe5\\x9e\\x8b\\xef\\xbc\\x9aK=3\\xef\\xbc\\x88CV\\xe3\\x80\\x81CA\\xe3\\x80\\x81CT\\xe6\\xa8\\xa1\\xe5\\xbc\\x8f\\xef\\xbc\\x89\n"
  "GMMComponent[] gmm_modes";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
saye_msgs__msg__PedestrianPrediction__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {saye_msgs__msg__PedestrianPrediction__TYPE_NAME, 34, 34},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 614, 614},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
saye_msgs__msg__PedestrianPrediction__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *saye_msgs__msg__PedestrianPrediction__get_individual_type_description_source(NULL),
    sources[1] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Pose__get_individual_type_description_source(NULL);
    sources[3] = *geometry_msgs__msg__Quaternion__get_individual_type_description_source(NULL);
    sources[4] = *saye_msgs__msg__GMMComponent__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
