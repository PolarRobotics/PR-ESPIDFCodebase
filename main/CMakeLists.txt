idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES  Drive
                                 Pairing  
                                 Robot
                                 Utilities
                                 espressif__arduino-esp32 
                                 polarrobotics__pr-lib
                                 fastled__fastled
                                 bt)
