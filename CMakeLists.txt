cmake_minimum_required(VERSION 3.27)
project(Cauldron)
set(CMAKE_CXX_STANDARD 20)

if(POLICY CMP0148) 
    cmake_policy(SET CMP0148 OLD) 
endif()

include(cmake/Dependencies.cmake)

set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(Ethyl)
add_subdirectory(Tonic)
