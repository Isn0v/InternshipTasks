cmake_minimum_required(VERSION 3.27)
project(InternshipTasks)

enable_testing()

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(CMAKE_PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

option(ENABLE_DEBUG "Enable debug mode" OFF)
if(ENABLE_DEBUG)
  add_compile_definitions(DEBUG_MODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -Wall -Wextra -Werror")
endif()

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)