cmake_minimum_required(VERSION 2.8)
project(shapeshift)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLSLANGValidator REQUIRED)
find_package(GLM REQUIRED)
add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)
if(FREEGLUT_STATIC)
    add_definitions(-DFREEGLUT_STATIC)
endif()
set(SHADERS
    test.vert
    test.frag
)
glslangvalidator_generate_cpp(SHADER_SRCS SHADER_HDRS ${SHADERS})
set(SRCS
    main.cpp
    shader.cpp
    program.cpp
    cubelet.cpp
    cube.cpp
    scene.cpp
    gamescene.cpp
    uiscene.cpp
    ${SHADER_SRCS}
)
set(HDRS
    shader.h
    program.h
    cubelet.h
    cube.h
    material.h
    scene.h
    gamescene.h
    uiscene.h
    ${SHADER_HDRS}
)
include_directories(
	${OPENGL_INCLUDE_DIR}
	${FREEGLUT_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(shapeshift ${SRCS} ${HDRS} ${SHADERS})
target_link_libraries(shapeshift
	${FREEGLUT_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)
