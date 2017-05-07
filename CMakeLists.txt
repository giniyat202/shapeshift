cmake_minimum_required(VERSION 2.8)
project(shapeshift)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)
if(FREEGLUT_STATIC)
    add_definitions(-DFREEGLUT_STATIC)
endif()
set(SRCS
    main.cpp
    cubelet.cpp
    cube.cpp
)
set(HDRS
    cubelet.h
    cube.h
)
include_directories(${OPENGL_INCLUDE_DIR} ${FREEGLUT_INCLUDE_DIRS})
add_executable(shapeshift ${SRCS} ${HDRS})
target_link_libraries(shapeshift ${FREEGLUT_LIBRARIES} ${OPENGL_LIBRARIES})
