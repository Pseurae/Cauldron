set(TEST_SOURCES eventbus.cpp framebuffer.cpp graphics.cpp serialization.cpp vfs.cpp)

foreach (SOURCE ${TEST_SOURCES})
add_executable(${SOURCE} ${SOURCE})
target_link_libraries(${SOURCE} Tonic)
endforeach()