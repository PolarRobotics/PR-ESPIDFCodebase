# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# In this example, sdkconfig file is placed into the build directory.
# This allows building development and production configs side by side,
# without having them influence each other.
set(SDKCONFIG "${CMAKE_BINARY_DIR}/sdkconfig")

# Bluedroid HCI host task (hciT) defaults to 2048+EXTRA bytes. PS5 Classic BT HID
# traffic overflows that stack (runtime-confirmed). Bump to 4096 before IDF builds.
if(DEFINED ENV{IDF_PATH})
    set(_PR_HCI_LAYER "$ENV{IDF_PATH}/components/bt/host/bluedroid/hci/hci_layer.c")
    if(EXISTS "${_PR_HCI_LAYER}")
        execute_process(
            COMMAND sed -i
                "s/#define HCI_HOST_TASK_STACK_SIZE        (2048 + BT_TASK_EXTRA_STACK_SIZE)/#define HCI_HOST_TASK_STACK_SIZE        (4096 + BT_TASK_EXTRA_STACK_SIZE)/"
                "${_PR_HCI_LAYER}"
            RESULT_VARIABLE _PR_HCI_PATCH_RESULT
        )
        message(STATUS "PR hciT stack patch applied to ${_PR_HCI_LAYER} (result=${_PR_HCI_PATCH_RESULT})")
    else()
        message(WARNING "PR hciT stack patch skipped; missing ${_PR_HCI_LAYER}")
    endif()
else()
    message(WARNING "PR hciT stack patch skipped; IDF_PATH unset")
endif()

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
