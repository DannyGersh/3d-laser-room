project(3d-laser-room)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

#find_package(GLEW REQUIRED)
#include_directories(${GLEW_INCLUDE_DIRS})
#link_libraries(${GLEW_LIBRARIES})

add_subdirectory(depend/glew-cmake)
include_directories(depend/glew-cmake/include)
add_subdirectory(depend/wxWidgets)
include_directories(depend/glm)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} libglew_shared glu32 wx::gl wx::core wx::base)
