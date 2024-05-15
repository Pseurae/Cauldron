project(Tonic)
cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)

include(CMakeSources.cmake)

add_library(${PROJECT_NAME} STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw gl3w glm)

list(TRANSFORM TONIC_SOURCES PREPEND "source/")
list(TRANSFORM TONIC_HEADERS PREPEND "include/")

target_sources(${PROJECT_NAME} PUBLIC ${TONIC_SOURCES} ${TONIC_HEADERS})