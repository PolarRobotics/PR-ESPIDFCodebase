file(GLOB_RECURSE MOTOR_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${MOTOR_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES espressif__arduino-esp32 
                               polarrobotics__pr-lib 
                               fastled__fastled
                               bt
                               esp_system)
