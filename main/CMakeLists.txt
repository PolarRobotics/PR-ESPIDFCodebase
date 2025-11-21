idf_component_register( 
    SRCS 
        "main.c"
    INCLUDE_DIRS
        "include"
    REQUIRES
        Drive
        Pairing
        Robot
        Types
        Utilities
        espressif__arduino-esp32 
)

if(CONFIG_ENV_ROBOT)
    target_sources(${COMPONENT_LIB} PRIVATE "main_robot.cpp")
elseif(CONFIG_ENV_WRITE_BOT_INFO)
    target_sources(${COMPONENT_LIB} PRIVATE "main_write_bot_info.cpp")
endif()