set(TEST_SOURCES graphics.cpp vfs.cpp)

foreach (SOURCE ${TEST_SOURCES})
add_executable(${SOURCE} ${SOURCE})
target_link_libraries(${SOURCE} Tonic)
endforeach()