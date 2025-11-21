file(GLOB_RECURSE DRIVE_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${DRIVE_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES Utilities Motor
                                espressif__arduino-esp32
                                
                                )
