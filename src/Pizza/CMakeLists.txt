file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.cpp)

include_directories(.)
add_library(Pizza STATIC ${SOURCES})