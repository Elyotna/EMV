cmake_minimum_required (VERSION 2.6)
project (EMVKernel)

SET( CMAKE_CXX_FLAGS  "-g -Wall -std=c++11" )

include_directories(/usr/include/PCSC include)

FILE(GLOB SRCFILES src/*.cpp)
add_library (emv ${SRCFILES})
add_executable (test main.cpp)

target_link_libraries(emv pcsclite)
target_link_libraries(test emv)
