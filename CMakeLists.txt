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

# Suppress the stringop-truncation warning for esp-modbus
idf_component_get_property(modbus_lib espressif__esp-modbus COMPONENT_LIB)
if(TARGET ${modbus_lib})
    target_compile_options(${modbus_lib} PRIVATE -Wno-stringop-truncation)
endif()
