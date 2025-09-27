file(GLOB_RECURSE DRIVE_SOURCES "*.cpp" "*.c" "*.h")
idf_component_register(SRCS ${DRIVE_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES Utilities Pairing Robot espressif__arduino-esp32)
