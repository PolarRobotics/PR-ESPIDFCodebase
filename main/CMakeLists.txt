idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES  Drive
                                 Pairing  
                                 Robot
                                 Utilities
                                 espressif__arduino-esp32 
                                 pr-lib-motor
                                 fastled
                                 #bluepad32
                                 #bt)
                                        )
# froijn