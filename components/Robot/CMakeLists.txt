file(GLOB_RECURSE ROBOT_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS 
        ${ROBOT_SOURCES}
    INCLUDE_DIRS 
        "include"
        "../../main"
    REQUIRES 
        Drive
        Motor
        Utilities 
        espressif__arduino-esp32 
        ps5-esp32
        pr-lib-motor
        adafruit_lis3mdl
)
