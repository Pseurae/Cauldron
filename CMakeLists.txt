cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)

if(POLICY CMP0148) 
    cmake_policy(SET CMP0148 OLD) 
endif()

include(cmake/Dependencies.cmake)

add_subdirectory(Tonic)
add_executable(Test test.cpp)
target_link_libraries(Test PUBLIC Tonic)