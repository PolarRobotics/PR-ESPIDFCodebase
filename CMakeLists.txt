# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
project(PR-ESPIDFCodebase)

idf_component_get_property(fastled_lib fastled COMPONENT_LIB)
if(fastled_lib)
  target_compile_options(${fastled_lib} PRIVATE
    -Wno-error=narrowing
    -Wno-error=missing-field-initializers
  )
endif()