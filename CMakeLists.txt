cmake_minimum_required(VERSION 3.10)

########################################################################################################################
### Project definition ###
########################################################################################################################
project(DataAnnotationTools C CXX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 14)

option(WITH_TESTS "Compile with tests." OFF)

add_subdirectory(cpp)