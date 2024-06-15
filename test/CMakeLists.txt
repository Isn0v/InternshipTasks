set(BINARY ${CMAKE_PROJECT_NAME}_test)



set(TEST_SOURCES DigitalLabTests.cpp)
set(SOLUTION_LIBRARIES DigitalLab)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(GTest)

add_executable(${BINARY} main.cpp ${TEST_SOURCES})

gtest_discover_tests(${BINARY})

target_link_libraries(${BINARY} PUBLIC gtest gmock ${SOLUTION_LIBRARIES})