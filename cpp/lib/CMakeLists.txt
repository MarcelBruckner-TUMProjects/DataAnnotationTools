# add the main lib

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(Ceres REQUIRED)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#include_directories(${GLUT_INCLUDE_DIRS})

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw ${PROJECT_SOURCE_DIR}/glfw/src)

add_library(libDataAnnotationTools
        Watersheder.cpp
        ${PROJECT_SOURCE_DIR}/glad/src/glad.c
        )
add_dependencies(libDataAnnotationTools OpenGL::OpenGL)

target_include_directories(libDataAnnotationTools
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}/glad/include
        )

target_link_libraries(libDataAnnotationTools
        #        Eigen3::Eigen Ceres::ceres
        ${OpenCV_LIBS} ${Boost_LIBRARIES}
        OpenGL::OpenGL
        glfw
        #        ${GLUT_LIBRARY}
        )

file(GLOB SHADERS "*.vert")
file(COPY ${SHADERS} DESTINATION ./)

file(GLOB FRAG_SHADERS "*.frag")
file(COPY ${FRAG_SHADERS} DESTINATION ./)