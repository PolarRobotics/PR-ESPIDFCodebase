file(GLOB_RECURSE MOTOR_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS 
        ${MOTOR_SOURCES}
    INCLUDE_DIRS 
        "include"
        "../../main"
    REQUIRES
        Types
        espressif__arduino-esp32 
        pr-lib-motor
)