file(GLOB_RECURSE UTILITIES_SOURCES "*.cpp" "*.c")

idf_component_register(
    SRCS 
        ${UTILITIES_SOURCES}
    INCLUDE_DIRS
        "include"
        "../../main"
    REQUIRES
        Types
        espressif__arduino-esp32
        usbsabertooth 
)
