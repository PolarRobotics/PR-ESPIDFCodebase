file(GLOB_RECURSE PAIRING_SOURCES "*.cpp" "*.c" "*.h")
idf_component_register(SRCS ${PAIRING_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES Utilities Drive Robot espressif__arduino-esp32)
