idf_component_register(SRCS "main_write_bot_info.cpp" "main_robot.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES  Drive
                                 Pairing  
                                 Robot
                                 Utilities
                                 espressif__arduino-esp32 
                                 bt
                                 pr-lib-motor
                                 fastled)

if(CONFIG_ENV_ROBOT)
    target_sources(${COMPONENT_LIB} PRIVATE "main_robot.cpp")
elseif(CONFIG_ENV_WRITE_BOT_INFO)
    target_sources(${COMPONENT_LIB} PRIVATE "main_write_bot_info.cpp")
endif()