file(GLOB_RECURSE UTILITIES_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${UTILITIES_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES espressif__arduino-esp32 
                               polarrobotics__pr-lib 
                               bt
                               esp_system)
