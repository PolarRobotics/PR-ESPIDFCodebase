file(GLOB_RECURSE UTILITIES_SOURCES "*.cpp" "*.c" "*.h")
idf_component_register(SRCS ${UTILITIES_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES Robot Drive Pairing espressif__arduino-esp32)
