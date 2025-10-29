# ESP-IDF wrapper for Adafruit LIS3MDL (Arduino library)
set(LIS3_UP ${CMAKE_CURRENT_LIST_DIR}/upstream)

# Sources are under src/
file(GLOB LIS3_SRCS
     "${LIS3_UP}/*.c"
     "${LIS3_UP}/*.cpp")

idf_component_register(
  SRCS         ${LIS3_SRCS}
  INCLUDE_DIRS "${LIS3_UP}/"
  REQUIRES
    espressif__arduino-esp32
    adafruit_busio
    adafruit_sensor
)