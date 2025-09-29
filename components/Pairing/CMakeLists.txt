file(GLOB_RECURSE PAIRING_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${PAIRING_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"  # For PolarRobotics.h
                       REQUIRES Utilities Drive Robot espressif__arduino-esp32 polarrobotics__pr-lib bt)
