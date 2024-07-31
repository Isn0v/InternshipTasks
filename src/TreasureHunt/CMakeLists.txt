include_directories(.)
add_library(TreasureHunt STATIC TreasureHunt.cpp TreasureHunt.hpp)
add_executable(TreasureHunt_run main.cpp TreasureHunt.cpp)