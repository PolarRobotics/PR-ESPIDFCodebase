file(GLOB_RECURSE UTILITIES_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS 
        ${UTILITIES_SOURCES}
    INCLUDE_DIRS
        "include"
        "../../main"
    REQUIRES 
        espressif__arduino-esp32 
)
