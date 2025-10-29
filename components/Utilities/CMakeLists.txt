file(GLOB_RECURSE UTILITIES_SOURCES "*.cpp" "*.c")
set(srcs
  log_wrap.c
)
idf_component_register(SRCS ${UTILITIES_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES espressif__arduino-esp32 
                                fastled
                                adafruit_lis3mdl
                                adafruit_busio
                                adafruit_sensor
                                log
                               )
