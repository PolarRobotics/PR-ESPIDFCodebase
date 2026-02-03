# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# In this example, sdkconfig file is placed into the build directory.
# This allows building development and production configs side by side,
# without having them influence each other.
set(SDKCONFIG "${CMAKE_BINARY_DIR}/sdkconfig")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)

project(PR-ESPIDFCodebase)

# set(ARDUINO_COMPONENT_DIR "${CMAKE_SOURCE_DIR}/managed_components/espressif__arduino-esp32")
# set(ARDUINO_PATCH_SCRIPT "${CMAKE_SOURCE_DIR}/tools/patch_arduino_adc.py")
# if(EXISTS "${ARDUINO_PATCH_SCRIPT}" AND EXISTS "${ARDUINO_COMPONENT_DIR}/cores/esp32/esp32-hal-adc.c")
#   find_package(Python3 COMPONENTS Interpreter)
#   if(Python3_Interpreter_FOUND)
#     execute_process(
#       COMMAND ${Python3_EXECUTABLE} "${ARDUINO_PATCH_SCRIPT}" --component-dir "${ARDUINO_COMPONENT_DIR}"
#       WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#       RESULT_VARIABLE ARDUINO_PATCH_RESULT
#       OUTPUT_VARIABLE ARDUINO_PATCH_STDOUT
#       ERROR_VARIABLE ARDUINO_PATCH_STDERR
#       OUTPUT_STRIP_TRAILING_WHITESPACE
#       ERROR_STRIP_TRAILING_WHITESPACE
#     )
#     if(ARDUINO_PATCH_STDOUT)
#       message(STATUS "${ARDUINO_PATCH_STDOUT}")
#     endif()
#     if(ARDUINO_PATCH_STDERR)
#       message(STATUS "${ARDUINO_PATCH_STDERR}")
#     endif()
#     if(NOT ARDUINO_PATCH_RESULT EQUAL 0)
#       message(FATAL_ERROR "Failed to patch arduino-esp32 ADC sources. See log above.")
#     endif()
#   else()
#     message(WARNING "Python3 interpreter not found; skipping Arduino ADC compatibility patch.")
#   endif()
# elseif(EXISTS "${ARDUINO_PATCH_SCRIPT}")
#   message(STATUS "Skipping Arduino ADC compatibility patch: component sources not present yet.")
# endif()