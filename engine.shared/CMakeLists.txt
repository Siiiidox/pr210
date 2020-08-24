# define project
project(engine.shared C CXX)
SET(TARGET_NAME engine.shared)

# configure executable, properties and linkage
add_library(${TARGET_NAME} STATIC)

# add sources to target
target_sources(
    ${TARGET_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/vec2.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/vec3.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/types.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/matrix4x4.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/quaternion.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/mathUtils.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/utils.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/color.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/mesh.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/camera.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/vertex.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/singelton.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/transform.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Header/stb_image.h"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/vec2.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/vec3.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/quaternion.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/matrix4x4.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/CPP/mesh.cpp"
)

# set additional target properties
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX ".debug")

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Header"
)
