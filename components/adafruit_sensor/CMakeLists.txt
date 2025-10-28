# ESP-IDF wrapper for Adafruit Unified Sensor (Arduino library)
set(ADAFRUIT_SENSOR_UP ${CMAKE_CURRENT_LIST_DIR}/upstream)

# kill me
file(GLOB ADAFRUIT_SENSOR_SRCS
     "${ADAFRUIT_SENSOR_UP}/*.c"
     "${ADAFRUIT_SENSOR_UP}/*.cpp")

idf_component_register(
  SRCS         ${ADAFRUIT_SENSOR_SRCS}
  INCLUDE_DIRS "${ADAFRUIT_SENSOR_UP}"
  REQUIRES     espressif__arduino-esp32
)