file(GLOB_RECURSE ROBOT_SOURCES "*.cpp" "*.c" "*.h")
idf_component_register(SRCS ${ROBOT_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES Utilities Drive Pairing espressif__arduino-esp32)
