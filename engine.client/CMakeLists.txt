# define project
project(engine.client C CXX)
SET(TARGET_NAME engine.client)

cmake_minimum_required (VERSION 3.9)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "../res/appicon.ico" "../res/resources.rc")

# add sources to target
target_sources(
    ${TARGET_NAME}
    PUBLIC
        "Header/appinfo.h"
        "Header/application.h"
        "Header/applicationWindow.h"
        "Header/types.h"
    PRIVATE
        "CPP/main.cpp" 
        "CPP/application.cpp"
        "CPP/applicationWindow.cpp"
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Header"
)

# set additional target properties
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX ".debug")
