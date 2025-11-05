file(GLOB_RECURSE ROBOT_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ROBOT_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES Utilities 
                                Drive
                                Motor
                                espressif__arduino-esp32 
                                ps5-esp32
                                pr-lib-motor
                                adafruit_lis3mdl
                                fastled)
