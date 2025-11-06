# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_saye_behaviortree_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED saye_behaviortree_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(saye_behaviortree_FOUND FALSE)
  elseif(NOT saye_behaviortree_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(saye_behaviortree_FOUND FALSE)
  endif()
  return()
endif()
set(_saye_behaviortree_CONFIG_INCLUDED TRUE)

# output package information
if(NOT saye_behaviortree_FIND_QUIETLY)
  message(STATUS "Found saye_behaviortree: 0.0.0 (${saye_behaviortree_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'saye_behaviortree' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT saye_behaviortree_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(saye_behaviortree_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${saye_behaviortree_DIR}/${_extra}")
endforeach()
