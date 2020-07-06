# define project
project(engine.shared C CXX)
SET(TARGET_NAME engine.shared)

# configure executable, properties and linkage
add_library(${TARGET_NAME} STATIC)

# add sources to target
target_sources(
    ${TARGET_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/appinfo.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/types.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/vec3.h"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/vec3.cpp"
)

# set additional target properties
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX ".debug")

# additional includes for target
target_include_directories(${TARGET_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/includes"
)
