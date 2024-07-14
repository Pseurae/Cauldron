set(TEST_SOURCES framebuffer.cpp graphics.cpp vfs.cpp)

foreach (SOURCE ${TEST_SOURCES})
add_executable(${SOURCE} ${SOURCE})
target_link_libraries(${SOURCE} Tonic)
endforeach()