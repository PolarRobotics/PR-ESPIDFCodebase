file(GLOB_RECURSE UTILITIES_SOURCES "*.cpp" "*.c")

# For the depairing/robot environments, allow excluding read/write bot info helpers
if(CONFIG_ENV_DEPAIRING)
    # depairing image: keep only minimal utilities used by depairingStation
    list(FILTER UTILITIES_SOURCES INCLUDE REGEX ".*(Pair\\.cpp|Debouncer\\.cpp|Pair\\.h)$")
elseif(CONFIG_ENV_WRITE_BOT_INFO)
    # Keep write helpers only
    list(FILTER UTILITIES_SOURCES INCLUDE REGEX ".*(WriteBotInfo\\.cpp|WriteBotInfo\\.h)$")
elseif(CONFIG_ENV_READ_BOT_INFO)
    # Keep read helpers only
    list(FILTER UTILITIES_SOURCES INCLUDE REGEX ".*(ReadBotInfo\\.cpp|ReadBotInfo\\.h)$")
else()
    # robot image: exclude nothing by default, but if needed exclude write/read in other envs
    list(FILTER UTILITIES_SOURCES EXCLUDE REGEX ".*/WriteBotInfo\\.cpp$")
    list(FILTER UTILITIES_SOURCES EXCLUDE REGEX ".*/ReadBotInfo\\.cpp$")
endif()

idf_component_register(SRCS ${UTILITIES_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES espressif__arduino-esp32 
                                fastled
                               )
