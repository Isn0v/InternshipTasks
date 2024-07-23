include_directories(.)
add_library(DigitalLab STATIC DigitalLab.cpp DigitalLab.hpp)
add_executable(DigitalLab_run main.cpp DigitalLab.cpp)