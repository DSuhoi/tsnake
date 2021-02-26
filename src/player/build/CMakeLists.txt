cmake_minimum_required(VERSION 3.8)

set(PROJECT player)
set(SOURCES ../src/snake.cpp)

include_directories(../include)
include_directories(../../term/include)

project(${PROJECT})

add_library(${PROJECT} STATIC ${SOURCES})

set_target_properties(${PROJECT} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


