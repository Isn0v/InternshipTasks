set(BINARY ${CMAKE_PROJECT_NAME})

add_subdirectory(DigitalLab)
add_subdirectory(TreasureHunt)
add_subdirectory(RGBGame)

add_executable(${BINARY}_run main.cpp)
target_link_libraries(${BINARY}_run DigitalLab TreasureHunt RGBGame)