set(TEST_SOURCES eventhandler.cpp services.cpp)

foreach (SOURCE ${TEST_SOURCES})
add_executable(${SOURCE} ${SOURCE})
target_link_libraries(${SOURCE} Potion)
endforeach()