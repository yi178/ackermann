// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice
#include "saye_msgs/msg/detail/pedestrian_predictions__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `predictions`
#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"

bool
saye_msgs__msg__PedestrianPredictions__init(saye_msgs__msg__PedestrianPredictions * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    saye_msgs__msg__PedestrianPredictions__fini(msg);
    return false;
  }
  // predictions
  if (!saye_msgs__msg__PedestrianPrediction__Sequence__init(&msg->predictions, 0)) {
    saye_msgs__msg__PedestrianPredictions__fini(msg);
    return false;
  }
  return true;
}

void
saye_msgs__msg__PedestrianPredictions__fini(saye_msgs__msg__PedestrianPredictions * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // predictions
  saye_msgs__msg__PedestrianPrediction__Sequence__fini(&msg->predictions);
}

bool
saye_msgs__msg__PedestrianPredictions__are_equal(const saye_msgs__msg__PedestrianPredictions * lhs, const saye_msgs__msg__PedestrianPredictions * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // predictions
  if (!saye_msgs__msg__PedestrianPrediction__Sequence__are_equal(
      &(lhs->predictions), &(rhs->predictions)))
  {
    return false;
  }
  return true;
}

bool
saye_msgs__msg__PedestrianPredictions__copy(
  const saye_msgs__msg__PedestrianPredictions * input,
  saye_msgs__msg__PedestrianPredictions * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // predictions
  if (!saye_msgs__msg__PedestrianPrediction__Sequence__copy(
      &(input->predictions), &(output->predictions)))
  {
    return false;
  }
  return true;
}

saye_msgs__msg__PedestrianPredictions *
saye_msgs__msg__PedestrianPredictions__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPredictions * msg = (saye_msgs__msg__PedestrianPredictions *)allocator.allocate(sizeof(saye_msgs__msg__PedestrianPredictions), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(saye_msgs__msg__PedestrianPredictions));
  bool success = saye_msgs__msg__PedestrianPredictions__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
saye_msgs__msg__PedestrianPredictions__destroy(saye_msgs__msg__PedestrianPredictions * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    saye_msgs__msg__PedestrianPredictions__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
saye_msgs__msg__PedestrianPredictions__Sequence__init(saye_msgs__msg__PedestrianPredictions__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPredictions * data = NULL;

  if (size) {
    data = (saye_msgs__msg__PedestrianPredictions *)allocator.zero_allocate(size, sizeof(saye_msgs__msg__PedestrianPredictions), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = saye_msgs__msg__PedestrianPredictions__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        saye_msgs__msg__PedestrianPredictions__fini(&data[i - 1]);
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
saye_msgs__msg__PedestrianPredictions__Sequence__fini(saye_msgs__msg__PedestrianPredictions__Sequence * array)
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
      saye_msgs__msg__PedestrianPredictions__fini(&array->data[i]);
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

saye_msgs__msg__PedestrianPredictions__Sequence *
saye_msgs__msg__PedestrianPredictions__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__PedestrianPredictions__Sequence * array = (saye_msgs__msg__PedestrianPredictions__Sequence *)allocator.allocate(sizeof(saye_msgs__msg__PedestrianPredictions__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = saye_msgs__msg__PedestrianPredictions__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
saye_msgs__msg__PedestrianPredictions__Sequence__destroy(saye_msgs__msg__PedestrianPredictions__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    saye_msgs__msg__PedestrianPredictions__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
saye_msgs__msg__PedestrianPredictions__Sequence__are_equal(const saye_msgs__msg__PedestrianPredictions__Sequence * lhs, const saye_msgs__msg__PedestrianPredictions__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!saye_msgs__msg__PedestrianPredictions__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
saye_msgs__msg__PedestrianPredictions__Sequence__copy(
  const saye_msgs__msg__PedestrianPredictions__Sequence * input,
  saye_msgs__msg__PedestrianPredictions__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(saye_msgs__msg__PedestrianPredictions);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    saye_msgs__msg__PedestrianPredictions * data =
      (saye_msgs__msg__PedestrianPredictions *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!saye_msgs__msg__PedestrianPredictions__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          saye_msgs__msg__PedestrianPredictions__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!saye_msgs__msg__PedestrianPredictions__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
