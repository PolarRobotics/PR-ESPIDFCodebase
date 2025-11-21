file(GLOB_RECURSE DRIVE_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS 
        ${DRIVE_SOURCES}
    INCLUDE_DIRS 
        "include"
        "../../main"
    REQUIRES 
        Motor
        Types
        espressif__arduino-esp32
)
