cmake_minimum_required(VERSION 3.10)

########################################################################################################################
### Packages ###
########################################################################################################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/cpp/extern)

########################################################################################################################
### Executables ###
########################################################################################################################

add_executable(Watersheder Watersheder.cpp)
target_link_libraries(Watersheder PUBLIC DataAnnotationTools-lib ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(Watersheder PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(LaneDetector LaneDetector.cpp)
target_link_libraries(LaneDetector PUBLIC DataAnnotationTools-lib ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(LaneDetector PUBLIC "${PROJECT_BINARY_DIR}")
