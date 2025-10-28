set(PS5_UP ${CMAKE_CURRENT_SOURCE_DIR}/upstream)
file (GLOB PS5_SRCS "${PS5_UP}/src/*.cpp" "${PS5_UP}/src/*.c")

idf_component_register(SRCS ${PS5_SRCS}
                       INCLUDE_DIRS "${PS5_UP}/src"
                       REQUIRES espressif__arduino-esp32 bt nvs_flash
                                       )
