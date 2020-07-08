# define project
project(engine.client C CXX)
SET(TARGET_NAME engine.client)

cmake_minimum_required (VERSION 3.9)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "../res/appicon.ico" "../res/resources.rc")

target_link_libraries(${TARGET_NAME} PUBLIC engine.shared)

# add sources to target
target_sources(
    ${TARGET_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/appinfo.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/application.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/applicationWindow.h"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/main.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/application.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/applicationWindow.cpp"
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    "${CMAKE_SOURCE_DIR}/engine.shared/Header"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header"
	"${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

# set additional target properties
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX ".debug")
