// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice
#include "saye_msgs/msg/detail/gmm_component__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "saye_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "saye_msgs/msg/detail/gmm_component__struct.h"
#include "saye_msgs/msg/detail/gmm_component__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // cov_xx, cov_xy, cov_yy, mean_x, mean_y
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // cov_xx, cov_xy, cov_yy, mean_x, mean_y

// forward declare type support functions


using _GMMComponent__ros_msg_type = saye_msgs__msg__GMMComponent;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_serialize_saye_msgs__msg__GMMComponent(
  const saye_msgs__msg__GMMComponent * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: weight
  {
    cdr << ros_message->weight;
  }

  // Field name: mean_x
  {
    size_t size = ros_message->mean_x.size;
    auto array_ptr = ros_message->mean_x.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: mean_y
  {
    size_t size = ros_message->mean_y.size;
    auto array_ptr = ros_message->mean_y.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_xx
  {
    size_t size = ros_message->cov_xx.size;
    auto array_ptr = ros_message->cov_xx.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_yy
  {
    size_t size = ros_message->cov_yy.size;
    auto array_ptr = ros_message->cov_yy.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_xy
  {
    size_t size = ros_message->cov_xy.size;
    auto array_ptr = ros_message->cov_xy.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_deserialize_saye_msgs__msg__GMMComponent(
  eprosima::fastcdr::Cdr & cdr,
  saye_msgs__msg__GMMComponent * ros_message)
{
  // Field name: weight
  {
    cdr >> ros_message->weight;
  }

  // Field name: mean_x
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->mean_x.data) {
      rosidl_runtime_c__double__Sequence__fini(&ros_message->mean_x);
    }
    if (!rosidl_runtime_c__double__Sequence__init(&ros_message->mean_x, size)) {
      fprintf(stderr, "failed to create array for field 'mean_x'");
      return false;
    }
    auto array_ptr = ros_message->mean_x.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: mean_y
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->mean_y.data) {
      rosidl_runtime_c__double__Sequence__fini(&ros_message->mean_y);
    }
    if (!rosidl_runtime_c__double__Sequence__init(&ros_message->mean_y, size)) {
      fprintf(stderr, "failed to create array for field 'mean_y'");
      return false;
    }
    auto array_ptr = ros_message->mean_y.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: cov_xx
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->cov_xx.data) {
      rosidl_runtime_c__double__Sequence__fini(&ros_message->cov_xx);
    }
    if (!rosidl_runtime_c__double__Sequence__init(&ros_message->cov_xx, size)) {
      fprintf(stderr, "failed to create array for field 'cov_xx'");
      return false;
    }
    auto array_ptr = ros_message->cov_xx.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: cov_yy
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->cov_yy.data) {
      rosidl_runtime_c__double__Sequence__fini(&ros_message->cov_yy);
    }
    if (!rosidl_runtime_c__double__Sequence__init(&ros_message->cov_yy, size)) {
      fprintf(stderr, "failed to create array for field 'cov_yy'");
      return false;
    }
    auto array_ptr = ros_message->cov_yy.data;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: cov_xy
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);

    // Check there are at least 'size' remaining bytes in the CDR stream before resizing
    auto old_state = cdr.get_state();
    bool correct_size = cdr.jump(size);
    cdr.set_state(old_state);
    if (!correct_size) {
      fprintf(stderr, "sequence size exceeds remaining buffer\n");
      return false;
    }

    if (ros_message->cov_xy.data) {
      rosidl_runtime_c__double__Sequence__fini(&ros_message->cov_xy);
    }
    if (!rosidl_runtime_c__double__Sequence__init(&ros_message->cov_xy, size)) {
      fprintf(stderr, "failed to create array for field 'cov_xy'");
      return false;
    }
    auto array_ptr = ros_message->cov_xy.data;
    cdr.deserialize_array(array_ptr, size);
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t get_serialized_size_saye_msgs__msg__GMMComponent(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _GMMComponent__ros_msg_type * ros_message = static_cast<const _GMMComponent__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: weight
  {
    size_t item_size = sizeof(ros_message->weight);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: mean_x
  {
    size_t array_size = ros_message->mean_x.size;
    auto array_ptr = ros_message->mean_x.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: mean_y
  {
    size_t array_size = ros_message->mean_y.size;
    auto array_ptr = ros_message->mean_y.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_xx
  {
    size_t array_size = ros_message->cov_xx.size;
    auto array_ptr = ros_message->cov_xx.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_yy
  {
    size_t array_size = ros_message->cov_yy.size;
    auto array_ptr = ros_message->cov_yy.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_xy
  {
    size_t array_size = ros_message->cov_xy.size;
    auto array_ptr = ros_message->cov_xy.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t max_serialized_size_saye_msgs__msg__GMMComponent(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: weight
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: mean_x
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: mean_y
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_xx
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_yy
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_xy
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = saye_msgs__msg__GMMComponent;
    is_plain =
      (
      offsetof(DataType, cov_xy) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
bool cdr_serialize_key_saye_msgs__msg__GMMComponent(
  const saye_msgs__msg__GMMComponent * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: weight
  {
    cdr << ros_message->weight;
  }

  // Field name: mean_x
  {
    size_t size = ros_message->mean_x.size;
    auto array_ptr = ros_message->mean_x.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: mean_y
  {
    size_t size = ros_message->mean_y.size;
    auto array_ptr = ros_message->mean_y.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_xx
  {
    size_t size = ros_message->cov_xx.size;
    auto array_ptr = ros_message->cov_xx.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_yy
  {
    size_t size = ros_message->cov_yy.size;
    auto array_ptr = ros_message->cov_yy.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: cov_xy
  {
    size_t size = ros_message->cov_xy.size;
    auto array_ptr = ros_message->cov_xy.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serialize_array(array_ptr, size);
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t get_serialized_size_key_saye_msgs__msg__GMMComponent(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _GMMComponent__ros_msg_type * ros_message = static_cast<const _GMMComponent__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: weight
  {
    size_t item_size = sizeof(ros_message->weight);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: mean_x
  {
    size_t array_size = ros_message->mean_x.size;
    auto array_ptr = ros_message->mean_x.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: mean_y
  {
    size_t array_size = ros_message->mean_y.size;
    auto array_ptr = ros_message->mean_y.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_xx
  {
    size_t array_size = ros_message->cov_xx.size;
    auto array_ptr = ros_message->cov_xx.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_yy
  {
    size_t array_size = ros_message->cov_yy.size;
    auto array_ptr = ros_message->cov_yy.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: cov_xy
  {
    size_t array_size = ros_message->cov_xy.size;
    auto array_ptr = ros_message->cov_xy.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_saye_msgs
size_t max_serialized_size_key_saye_msgs__msg__GMMComponent(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: weight
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: mean_x
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: mean_y
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_xx
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_yy
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: cov_xy
  {
    size_t array_size = 0;
    full_bounded = false;
    is_plain = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = saye_msgs__msg__GMMComponent;
    is_plain =
      (
      offsetof(DataType, cov_xy) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _GMMComponent__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const saye_msgs__msg__GMMComponent * ros_message = static_cast<const saye_msgs__msg__GMMComponent *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_saye_msgs__msg__GMMComponent(ros_message, cdr);
}

static bool _GMMComponent__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  saye_msgs__msg__GMMComponent * ros_message = static_cast<saye_msgs__msg__GMMComponent *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_saye_msgs__msg__GMMComponent(cdr, ros_message);
}

static uint32_t _GMMComponent__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_saye_msgs__msg__GMMComponent(
      untyped_ros_message, 0));
}

static size_t _GMMComponent__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_saye_msgs__msg__GMMComponent(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_GMMComponent = {
  "saye_msgs::msg",
  "GMMComponent",
  _GMMComponent__cdr_serialize,
  _GMMComponent__cdr_deserialize,
  _GMMComponent__get_serialized_size,
  _GMMComponent__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _GMMComponent__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_GMMComponent,
  get_message_typesupport_handle_function,
  &saye_msgs__msg__GMMComponent__get_type_hash,
  &saye_msgs__msg__GMMComponent__get_type_description,
  &saye_msgs__msg__GMMComponent__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, saye_msgs, msg, GMMComponent)() {
  return &_GMMComponent__type_support;
}

#if defined(__cplusplus)
}
#endif
