file(GLOB_RECURSE PAIRING_SOURCES "*.cpp" "*.c")

idf_component_register(
    SRCS
        ${PAIRING_SOURCES}
    INCLUDE_DIRS
        "include"
        "../../main"
    REQUIRES
        Types
        espressif__arduino-esp32
        ps5-esp32
)
