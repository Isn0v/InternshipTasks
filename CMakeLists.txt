cmake_minimum_required(VERSION 3.27)
project(InternshipTasks)

enable_testing()
add_compile_definitions(CMAKE_PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 23)

# If Linux or WSL, u may use the following flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -Wall -Wextra -Werror")

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)