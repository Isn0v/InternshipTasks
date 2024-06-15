set(BINARY ${CMAKE_PROJECT_NAME})
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.cpp)

include_directories(.)
add_library(TreasureHunt STATIC ${SOURCES})