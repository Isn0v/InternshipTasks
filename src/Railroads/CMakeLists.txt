include_directories(.)
add_library(Railroads STATIC Railroads.cpp Railroads.hpp)
add_executable(Railroads_run main.cpp Railroads.cpp)