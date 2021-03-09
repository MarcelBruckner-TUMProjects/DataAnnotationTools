########################################################################################################################
### YAML CPP ###
########################################################################################################################

# Download and unpack yaml-cpp at configure time
configure_file(CMakeLists.txt.in yaml-cpp-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-download)
if (result)
    message(FATAL_ERROR "CMake step for yaml-cpp failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-download)
if (result)
    message(FATAL_ERROR "Build step for yaml-cpp failed: ${result}")
endif ()

# Add yaml-cpp directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-src
        ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-build
        EXCLUDE_FROM_ALL)

########################################################################################################################
### Library ###
########################################################################################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(Ceres REQUIRED)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

add_library(libDataAnnotationTools
        Watersheder.cpp
        ProgramOptions.cpp
        Formatter.cpp
        )

target_include_directories(libDataAnnotationTools
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(libDataAnnotationTools
        #        Eigen3::Eigen Ceres::ceres
        ${OpenCV_LIBS} ${Boost_LIBRARIES}
        yaml-cpp
        )
