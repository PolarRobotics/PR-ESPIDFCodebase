file(GLOB_RECURSE MOTOR_SOURCES "*.cpp" "*.c")

# Legacy combined motor controller (replaced by PWMMotor + SerialMotor)
list(REMOVE_ITEM MOTOR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/MotorControl.cpp")

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
        usbsabertooth
)