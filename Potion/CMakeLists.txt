project(Potion)
cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)

include(CMakeSources.cmake)

add_library(${PROJECT_NAME} STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC Tonic)

list(TRANSFORM POTION_SOURCES PREPEND "source/")
list(TRANSFORM POTION_HEADERS PREPEND "include/")

target_sources(${PROJECT_NAME} PUBLIC ${POTION_SOURCES} ${POTION_HEADERS})

add_subdirectory(tests)