include_directories(.)
add_library(RGBGame STATIC RGBGame.cpp RGBGame.hpp)
add_executable(RGBGame_run main.cpp RGBGame.cpp)