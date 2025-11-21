idf_component_register( 
    SRCS 
        "src/main.c"
    INCLUDE_DIRS
        "include"
    REQUIRES
        Drive
        Pairing
        Robot
        Utilities
        espressif__arduino-esp32 
        bt
        pr-lib-motor
)

if(CONFIG_ENV_ROBOT)
    target_sources(${COMPONENT_LIB} PRIVATE "src/main_robot.cpp")
elseif(CONFIG_ENV_WRITE_BOT_INFO)
    target_sources(${COMPONENT_LIB} PRIVATE "src/main_write_bot_info.cpp")
endif()