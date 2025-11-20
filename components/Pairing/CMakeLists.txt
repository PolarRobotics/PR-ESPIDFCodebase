file(GLOB_RECURSE PAIRING_SOURCES "*.cpp" "*.c")

# Filter pairing sources based on selected build environment (main/Kconfig)
# Supported configs: ENV_ROBOT (default), ENV_DEPAIRING, ENV_WRITE_BOT_INFO, ENV_READ_BOT_INFO
if(CONFIG_ENV_DEPAIRING)
       # Keep only pairing and depairingStation sources for depairing image
       list(FILTER PAIRING_SOURCES INCLUDE REGEX ".*(pairing\\.cpp|depairingStation\\.cpp)$")
elseif(CONFIG_ENV_WRITE_BOT_INFO)
       # Write-bot-info environment needs only pairing.cpp from this component
       list(FILTER PAIRING_SOURCES INCLUDE REGEX ".*pairing\\.cpp$")
elseif(CONFIG_ENV_READ_BOT_INFO)
       # Read-bot-info environment doesn't need Pairing sources
       set(PAIRING_SOURCES "")
else()
       # Default robot image: exclude depairingStation and getAddress
       list(FILTER PAIRING_SOURCES EXCLUDE REGEX ".*/depairingStation\\.cpp$")
       list(FILTER PAIRING_SOURCES EXCLUDE REGEX ".*/getAddress\\.cpp$")
endif()

idf_component_register(SRCS ${PAIRING_SOURCES}
                                      INCLUDE_DIRS "."
                                                               "../../main"  # For PolarRobotics.h
                                      REQUIRES Utilities 
                                                        Robot 
                                                        espressif__arduino-esp32
                                                        ps5-esp32
                                                        bt
                                                        nvs_flash
                                                        esp_system
                                                                  )
