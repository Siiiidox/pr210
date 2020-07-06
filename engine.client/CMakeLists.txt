# define project
project(engine.client C CXX)
SET(TARGET_NAME engine.client)

cmake_minimum_required (VERSION 3.9)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "../res/appicon.ico" "../res/resources.rc" "client.h" "client.cpp" "types.h" "AppWindow.cpp")

# add sources to target
target_sources(
    ${TARGET_NAME}
    PUBLIC
        "appinfo.h"
    PRIVATE
        "main.cpp" 
)

# set additional target properties
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX ".debug")
