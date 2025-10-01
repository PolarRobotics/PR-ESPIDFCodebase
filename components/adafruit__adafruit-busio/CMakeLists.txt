file(GLOB_RECURSE ADAFRUIT_BUSIO_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ADAFRUIT_BUSIO_SOURCES}
                       INCLUDE_DIRS ".")