########################################################################################################################
### Includes ###
########################################################################################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/cpp/extern)
include_directories(${PROJECT_SOURCE_DIR}/cpp/include)

########################################################################################################################
### Library ###
########################################################################################################################

add_library(DataAnnotationTools-lib
        CommandLineParser.cpp
        ProgramOptions.cpp

        watershed/CommandLineParser.cpp
        watershed/Watersheder.cpp
        watershed/Formatter.cpp

        lane_detection/LaneDetection.cpp

        kdtree.cpp
        Gui.cpp
        Algorithm.cpp
        )

target_include_directories(DataAnnotationTools-lib
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${PROJECT_BINARY_DIR}
        )

target_link_libraries(DataAnnotationTools-lib
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        yaml-cpp
        #        dbscan
        )
