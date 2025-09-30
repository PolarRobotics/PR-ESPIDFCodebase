file(GLOB_RECURSE DRIVE_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${DRIVE_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES Utilities Robot
                               espressif__arduino-esp32 
                               polarrobotics__pr-lib 
                               fastled__fastled
                               bt
                               esp_system)
