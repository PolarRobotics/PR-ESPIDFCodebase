file(GLOB_RECURSE PAIRING_SOURCES "src/*.cpp" "src/*.c")

idf_component_register(
    SRCS
        ${PAIRING_SOURCES}
    INCLUDE_DIRS
        "include"
        "../../main"
    REQUIRES
        Utilities
        espressif__arduino-esp32
        ps5-esp32
        bt
        nvs_flash
        esp_system
)
