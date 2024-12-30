set(TEST_SOURCES camera.cpp eventhandler.cpp services.cpp update_tick.cpp)

foreach (SOURCE ${TEST_SOURCES})
add_executable(${SOURCE} ${SOURCE})
target_link_libraries(${SOURCE} Potion)
endforeach()