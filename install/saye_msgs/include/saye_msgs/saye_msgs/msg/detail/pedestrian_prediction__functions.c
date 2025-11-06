// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from saye_msgs:msg/PedestrianPrediction.idl
// generated code does not contain a copyright notice
#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `id`
#include "rosidl_runtime_c/string_functions.h"
// Member `current_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `gmm_modes`
#include "saye_msgs/msg/detail/gmm_component__functions.h"

bool
saye_msgs__msg__PedestrianPrediction__init(saye_msgs__msg__PedestrianPrediction * msg)
{
  if (!msg) {
    return false;
  }
  // id
  if (!rosidl_runtime_c__String__init(&msg->id)) {
    saye_msgs__msg__PedestrianPrediction__fini(msg);
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__init(&msg->current_pose)) {
    saye_msgs__msg__PedestrianPrediction__fini(msg);
    return false;
  }
  // current_speed
  // gmm_modes
  if (!saye_msgs__msg__GMMComponent__Sequence__init(&msg->gmm_modes, 0)) {
    saye_msgs__msg__PedestrianPrediction__fini(msg);
    return false;
  }
  return true;
}

void
saye_msgs__msg__PedestrianPrediction__fini(saye_msgs__msg__PedestrianPrediction * msg)
{
  if (!msg) {
    return;
  }
  // id
  rosidl_runtime_c__String__fini(&msg->id);
  // current_pose
  geometry_msgs__msg__Pose__fini(&msg->current_pose);
  // current_speed
  // gmm_modes
  saye_msgs__msg__GMMComponent__Sequence__fini(&msg->gmm_modes);
}

bool
saye_msgs__msg__PedestrianPrediction__are_equal(const saye_msgs__msg__PedestrianPrediction * lhs, const saye_msgs__msg__PedestrianPrediction * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->id), &(rhs->id)))
  {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->current_pose), &(rhs->current_pose)))
  {
    return false;
  }
  // current_speed
  if (lhs->current_speed != rhs->current_speed) {
    return false;
  }
  // gmm_modes
  if (!saye_msgs__msg__GMMComponent__Sequence__are_equal(
      &(lhs->gmm_modes), &(rhs->gmm_modes)))
  {
    return false;
  }
  return true;
}

bool
saye_msgs__msg__PedestrianPrediction__copy(
  const saye_msgs__msg__PedestrianPrediction * input,
  saye_msgs__msg__PedestrianPrediction * output)
{
  if (!input || !output) {
    return false;
  }
  // id
  if (!rosidl_runtime_c__String__copy(
      &(input->id), &(output->id)))
  {
    return false;
  }
  // current_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->current_pose), &(output->current_pose)))
  {
    return false;
  }
  // current_speed
  output->current_speed = input->current_speed;
  // gmm_modes
  if (!saye_msgs__msg__GMMComponent__Sequence__copy(
      &(input->gmm_modes), &(output->gmm_modes)))
  {
    return false;
  }
  return true;
}

saye_msgs__msg__PedestrianPrediction *
saye_msgs__msg__PedestrianPrediction__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPrediction * msg = (saye_msgs__msg__PedestrianPrediction *)allocator.allocate(sizeof(saye_msgs__msg__PedestrianPrediction), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(saye_msgs__msg__PedestrianPrediction));
  bool success = saye_msgs__msg__PedestrianPrediction__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
saye_msgs__msg__PedestrianPrediction__destroy(saye_msgs__msg__PedestrianPrediction * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    saye_msgs__msg__PedestrianPrediction__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
saye_msgs__msg__PedestrianPrediction__Sequence__init(saye_msgs__msg__PedestrianPrediction__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPrediction * data = NULL;

  if (size) {
    data = (saye_msgs__msg__PedestrianPrediction *)allocator.zero_allocate(size, sizeof(saye_msgs__msg__PedestrianPrediction), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = saye_msgs__msg__PedestrianPrediction__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        saye_msgs__msg__PedestrianPrediction__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
saye_msgs__msg__PedestrianPrediction__Sequence__fini(saye_msgs__msg__PedestrianPrediction__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      saye_msgs__msg__PedestrianPrediction__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

saye_msgs__msg__PedestrianPrediction__Sequence *
saye_msgs__msg__PedestrianPrediction__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPrediction__Sequence * array = (saye_msgs__msg__PedestrianPrediction__Sequence *)allocator.allocate(sizeof(saye_msgs__msg__PedestrianPrediction__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = saye_msgs__msg__PedestrianPrediction__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
saye_msgs__msg__PedestrianPrediction__Sequence__destroy(saye_msgs__msg__PedestrianPrediction__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    saye_msgs__msg__PedestrianPrediction__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
saye_msgs__msg__PedestrianPrediction__Sequence__are_equal(const saye_msgs__msg__PedestrianPrediction__Sequence * lhs, const saye_msgs__msg__PedestrianPrediction__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!saye_msgs__msg__PedestrianPrediction__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
saye_msgs__msg__PedestrianPrediction__Sequence__copy(
  const saye_msgs__msg__PedestrianPrediction__Sequence * input,
  saye_msgs__msg__PedestrianPrediction__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(saye_msgs__msg__PedestrianPrediction);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    saye_msgs__msg__PedestrianPrediction * data =
      (saye_msgs__msg__PedestrianPrediction *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!saye_msgs__msg__PedestrianPrediction__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          saye_msgs__msg__PedestrianPrediction__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!saye_msgs__msg__PedestrianPrediction__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
