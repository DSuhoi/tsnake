cmake_minimum_required(VERSION 3.8)

set(PROJECT term)
set(HEADERS ../display.h ../files.h ../periph.h ../coords.h)
set(SOURCES ../sources/display.cpp ../sources/files.cpp ../sources/periph.cpp
    ../sources/coords.cpp)

project(${PROJECT})

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


