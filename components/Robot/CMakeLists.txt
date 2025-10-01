file(GLOB_RECURSE ROBOT_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ROBOT_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES Utilities Drive
                               espressif__arduino-esp32 
                               polarrobotics__pr-lib 
                               fastled__fastled
                               adafruit__adafruit-lis3mdl
                               bt
                               esp_system)
