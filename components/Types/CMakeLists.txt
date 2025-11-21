file(GLOB_RECURSE TYPES_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS 
        ${TYPES_SOURCES}
    INCLUDE_DIRS
        "include"
        "../../main"
    REQUIRES 
        espressif__arduino-esp32 
)