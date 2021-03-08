cmake_minimum_required(VERSION 3.8)

set(PROJECT term)
set(SOURCES ../src/term_windows.cpp ../src/files.cpp ../src/periph.cpp
    ../src/coords.cpp)

include_directories(../include)
include_directories(../../menu/include)

project(${PROJECT})

add_library(${PROJECT} SHARED ${SOURCES})

set_target_properties(${PROJECT} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


