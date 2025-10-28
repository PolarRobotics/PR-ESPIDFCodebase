file(GLOB_RECURSE ADAFRUIT_LIS3MDL_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ADAFRUIT_LIS3MDL_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES
                       adafruit-sensor
                       adafruit-busio
                       )