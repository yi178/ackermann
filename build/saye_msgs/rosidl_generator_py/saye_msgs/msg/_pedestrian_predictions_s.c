// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from saye_msgs:msg/PedestrianPredictions.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__struct.h"
#include "saye_msgs/msg/detail/pedestrian_predictions__functions.h"

#include "rosidl_runtime_c/primitives_sequence.h"
#include "rosidl_runtime_c/primitives_sequence_functions.h"

// Nested array functions includes
#include "saye_msgs/msg/detail/pedestrian_prediction__functions.h"
// end nested array functions include
ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__header__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__header__convert_to_py(void * raw_ros_message);
bool saye_msgs__msg__pedestrian_prediction__convert_from_py(PyObject * _pymsg, void * _ros_message);
PyObject * saye_msgs__msg__pedestrian_prediction__convert_to_py(void * raw_ros_message);

ROSIDL_GENERATOR_C_EXPORT
bool saye_msgs__msg__pedestrian_predictions__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[60];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("saye_msgs.msg._pedestrian_predictions.PedestrianPredictions", full_classname_dest, 59) == 0);
  }
  saye_msgs__msg__PedestrianPredictions * ros_message = _ros_message;
  {  // header
    PyObject * field = PyObject_GetAttrString(_pymsg, "header");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__header__convert_from_py(field, &ros_message->header)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // predictions
    PyObject * field = PyObject_GetAttrString(_pymsg, "predictions");
    if (!field) {
      return false;
    }
    PyObject * seq_field = PySequence_Fast(field, "expected a sequence in 'predictions'");
    if (!seq_field) {
      Py_DECREF(field);
      return false;
    }
    Py_ssize_t size = PySequence_Size(field);
    if (-1 == size) {
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    if (!saye_msgs__msg__PedestrianPrediction__Sequence__init(&(ros_message->predictions), size)) {
      PyErr_SetString(PyExc_RuntimeError, "unable to create saye_msgs__msg__PedestrianPrediction__Sequence ros_message");
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    saye_msgs__msg__PedestrianPrediction * dest = ros_message->predictions.data;
    for (Py_ssize_t i = 0; i < size; ++i) {
      if (!saye_msgs__msg__pedestrian_prediction__convert_from_py(PySequence_Fast_GET_ITEM(seq_field, i), &dest[i])) {
        Py_DECREF(seq_field);
        Py_DECREF(field);
        return false;
      }
    }
    Py_DECREF(seq_field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * saye_msgs__msg__pedestrian_predictions__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of PedestrianPredictions */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("saye_msgs.msg._pedestrian_predictions");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "PedestrianPredictions");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  saye_msgs__msg__PedestrianPredictions * ros_message = (saye_msgs__msg__PedestrianPredictions *)raw_ros_message;
  {  // header
    PyObject * field = NULL;
    field = std_msgs__msg__header__convert_to_py(&ros_message->header);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "header", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // predictions
    PyObject * field = NULL;
    size_t size = ros_message->predictions.size;
    field = PyList_New(size);
    if (!field) {
      return NULL;
    }
    saye_msgs__msg__PedestrianPrediction * item;
    for (size_t i = 0; i < size; ++i) {
      item = &(ros_message->predictions.data[i]);
      PyObject * pyitem = saye_msgs__msg__pedestrian_prediction__convert_to_py(item);
      if (!pyitem) {
        Py_DECREF(field);
        return NULL;
      }
      int rc = PyList_SetItem(field, i, pyitem);
      (void)rc;
      assert(rc == 0);
    }
    assert(PySequence_Check(field));
    {
      int rc = PyObject_SetAttrString(_pymessage, "predictions", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
