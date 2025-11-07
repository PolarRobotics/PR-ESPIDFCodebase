file(GLOB_RECURSE ROBOT_SOURCES "*.cpp" "*.c")

# Exclude robot-only legacy sources for the depairing build
if(CONFIG_ENV_DEPAIRING)
    # depairing image is minimal; exclude large/legacy sources
    list(FILTER ROBOT_SOURCES EXCLUDE REGEX ".*/Quarterback_old\\.cpp$")
elseif(CONFIG_ENV_WRITE_BOT_INFO OR CONFIG_ENV_READ_BOT_INFO)
    # Minimal robot code for read/write helpers: exclude heavy gameplay files
    list(FILTER ROBOT_SOURCES EXCLUDE REGEX ".*/Quarterback.*\\.cpp$")
    list(FILTER ROBOT_SOURCES EXCLUDE REGEX ".*/MecanumCenter\\.cpp$")
endif()

idf_component_register(SRCS ${ROBOT_SOURCES}
                       INCLUDE_DIRS "."
                                    "../../main"
                       REQUIRES Utilities 
                                Drive
                                Motor
                                espressif__arduino-esp32 
                                ps5-esp32
                                pr-lib-motor
                                adafruit_lis3mdl
                                fastled)
