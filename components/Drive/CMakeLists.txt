file(GLOB_RECURSE DRIVE_SOURCES "*.cpp" "*.c")

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
