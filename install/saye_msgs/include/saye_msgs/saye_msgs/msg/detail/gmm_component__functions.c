// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from saye_msgs:msg/GMMComponent.idl
// generated code does not contain a copyright notice
#include "saye_msgs/msg/detail/gmm_component__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `mean_x`
// Member `mean_y`
// Member `cov_xx`
// Member `cov_yy`
// Member `cov_xy`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
saye_msgs__msg__GMMComponent__init(saye_msgs__msg__GMMComponent * msg)
{
  if (!msg) {
    return false;
  }
  // weight
  // mean_x
  if (!rosidl_runtime_c__double__Sequence__init(&msg->mean_x, 0)) {
    saye_msgs__msg__GMMComponent__fini(msg);
    return false;
  }
  // mean_y
  if (!rosidl_runtime_c__double__Sequence__init(&msg->mean_y, 0)) {
    saye_msgs__msg__GMMComponent__fini(msg);
    return false;
  }
  // cov_xx
  if (!rosidl_runtime_c__double__Sequence__init(&msg->cov_xx, 0)) {
    saye_msgs__msg__GMMComponent__fini(msg);
    return false;
  }
  // cov_yy
  if (!rosidl_runtime_c__double__Sequence__init(&msg->cov_yy, 0)) {
    saye_msgs__msg__GMMComponent__fini(msg);
    return false;
  }
  // cov_xy
  if (!rosidl_runtime_c__double__Sequence__init(&msg->cov_xy, 0)) {
    saye_msgs__msg__GMMComponent__fini(msg);
    return false;
  }
  return true;
}

void
saye_msgs__msg__GMMComponent__fini(saye_msgs__msg__GMMComponent * msg)
{
  if (!msg) {
    return;
  }
  // weight
  // mean_x
  rosidl_runtime_c__double__Sequence__fini(&msg->mean_x);
  // mean_y
  rosidl_runtime_c__double__Sequence__fini(&msg->mean_y);
  // cov_xx
  rosidl_runtime_c__double__Sequence__fini(&msg->cov_xx);
  // cov_yy
  rosidl_runtime_c__double__Sequence__fini(&msg->cov_yy);
  // cov_xy
  rosidl_runtime_c__double__Sequence__fini(&msg->cov_xy);
}

bool
saye_msgs__msg__GMMComponent__are_equal(const saye_msgs__msg__GMMComponent * lhs, const saye_msgs__msg__GMMComponent * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // weight
  if (lhs->weight != rhs->weight) {
    return false;
  }
  // mean_x
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->mean_x), &(rhs->mean_x)))
  {
    return false;
  }
  // mean_y
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->mean_y), &(rhs->mean_y)))
  {
    return false;
  }
  // cov_xx
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->cov_xx), &(rhs->cov_xx)))
  {
    return false;
  }
  // cov_yy
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->cov_yy), &(rhs->cov_yy)))
  {
    return false;
  }
  // cov_xy
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->cov_xy), &(rhs->cov_xy)))
  {
    return false;
  }
  return true;
}

bool
saye_msgs__msg__GMMComponent__copy(
  const saye_msgs__msg__GMMComponent * input,
  saye_msgs__msg__GMMComponent * output)
{
  if (!input || !output) {
    return false;
  }
  // weight
  output->weight = input->weight;
  // mean_x
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->mean_x), &(output->mean_x)))
  {
    return false;
  }
  // mean_y
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->mean_y), &(output->mean_y)))
  {
    return false;
  }
  // cov_xx
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->cov_xx), &(output->cov_xx)))
  {
    return false;
  }
  // cov_yy
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->cov_yy), &(output->cov_yy)))
  {
    return false;
  }
  // cov_xy
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->cov_xy), &(output->cov_xy)))
  {
    return false;
  }
  return true;
}

saye_msgs__msg__GMMComponent *
saye_msgs__msg__GMMComponent__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__GMMComponent * msg = (saye_msgs__msg__GMMComponent *)allocator.allocate(sizeof(saye_msgs__msg__GMMComponent), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(saye_msgs__msg__GMMComponent));
  bool success = saye_msgs__msg__GMMComponent__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
saye_msgs__msg__GMMComponent__destroy(saye_msgs__msg__GMMComponent * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    saye_msgs__msg__GMMComponent__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
saye_msgs__msg__GMMComponent__Sequence__init(saye_msgs__msg__GMMComponent__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__GMMComponent * data = NULL;

  if (size) {
    data = (saye_msgs__msg__GMMComponent *)allocator.zero_allocate(size, sizeof(saye_msgs__msg__GMMComponent), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = saye_msgs__msg__GMMComponent__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        saye_msgs__msg__GMMComponent__fini(&data[i - 1]);
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
saye_msgs__msg__GMMComponent__Sequence__fini(saye_msgs__msg__GMMComponent__Sequence * array)
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
      saye_msgs__msg__GMMComponent__fini(&array->data[i]);
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

saye_msgs__msg__GMMComponent__Sequence *
saye_msgs__msg__GMMComponent__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  saye_msgs__msg__GMMComponent__Sequence * array = (saye_msgs__msg__GMMComponent__Sequence *)allocator.allocate(sizeof(saye_msgs__msg__GMMComponent__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = saye_msgs__msg__GMMComponent__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
saye_msgs__msg__GMMComponent__Sequence__destroy(saye_msgs__msg__GMMComponent__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    saye_msgs__msg__GMMComponent__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
saye_msgs__msg__GMMComponent__Sequence__are_equal(const saye_msgs__msg__GMMComponent__Sequence * lhs, const saye_msgs__msg__GMMComponent__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!saye_msgs__msg__GMMComponent__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
saye_msgs__msg__GMMComponent__Sequence__copy(
  const saye_msgs__msg__GMMComponent__Sequence * input,
  saye_msgs__msg__GMMComponent__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(saye_msgs__msg__GMMComponent);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    saye_msgs__msg__GMMComponent * data =
      (saye_msgs__msg__GMMComponent *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!saye_msgs__msg__GMMComponent__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          saye_msgs__msg__GMMComponent__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!saye_msgs__msg__GMMComponent__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
