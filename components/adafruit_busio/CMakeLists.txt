# components/adafruit_busio/CMakeLists.txt
set(BUSIO_UP ${CMAKE_CURRENT_LIST_DIR}/upstream)

file(GLOB BUSIO_SRCS
     "${BUSIO_UP}/src/*.cpp")

idf_component_register(
  SRCS         ${BUSIO_SRCS}
  INCLUDE_DIRS "${BUSIO_UP}/src"
  REQUIRES     espressif__arduino-esp32
)
