file(GLOB_RECURSE ROBOT_SOURCES "*.cpp" "*.c")
idf_component_register(SRCS ${ROBOT_SOURCES}
                       INCLUDE_DIRS "."
                       REQUIRES Utilities Drive Robot)
