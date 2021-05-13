cmake_minimum_required(VERSION 3.10)

########################################################################################################################
### Packages ###
########################################################################################################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/cpp/extern)

########################################################################################################################
### TESTING ###
########################################################################################################################
add_executable(Tests
        CommandLineParserTests.cpp
        )
target_link_libraries(Tests PUBLIC DataAnnotationTools-lib
        gtest_main
        ${OpenCV_LIBS}
        )
target_include_directories(Tests PUBLIC tests "${PROJECT_BINARY_DIR}")


add_test(AllTests Tests)
