file(GLOB_RECURSE PAIRING_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${PAIRING_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"  # For PolarRobotics.h
                       REQUIRES Utilities 
                                Robot 
                                espressif__arduino-esp32 
                                fastled 
                                bluepad32
                                bt)
