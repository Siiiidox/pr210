# CMakeList.txt : CMake project for PR210, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

# Add source to this project's executable.
add_executable(PR210 "../res/appicon.ico" "../res/resources.rc")

# add sources to target
target_sources(
    PR210
    PUBLIC
        "PR210.h"
    PRIVATE
        "PR210.cpp" 
)
# TODO: Add tests and install targets if needed.
