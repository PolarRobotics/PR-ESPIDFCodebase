file(GLOB_RECURSE ADAFRUIT_SENSOR_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ADAFRUIT_SENSOR_SOURCES}
                       INCLUDE_DIRS ".")