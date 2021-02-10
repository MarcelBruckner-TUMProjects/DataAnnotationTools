# add the main lib

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(Ceres REQUIRED)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

add_library(libDataAnnotationTools
        Watersheder.cpp)

target_include_directories(libDataAnnotationTools
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(libDataAnnotationTools
        #        Eigen3::Eigen Ceres::ceres
        ${OpenCV_LIBS} ${Boost_LIBRARIES}
        )
