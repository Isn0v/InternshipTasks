include_directories(.)
add_library(Pizza STATIC Pizza.cpp Pizza.hpp)
add_executable(Pizza_run main.cpp Pizza.cpp)