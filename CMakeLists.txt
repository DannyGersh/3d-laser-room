cmake_minimum_required(VERSION 3.15)
project(3d-laser-room)
aux_source_directory(. SRC_LIST)

set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

add_subdirectory(depend/glew-cmake)
include_directories(depend/glew-cmake/include)
add_subdirectory(depend/wxWidgets)
include_directories(depend/glm)

add_executable(${PROJECT_NAME} WIN32 Main.cpp)
target_link_libraries(${PROJECT_NAME} libglew_shared glu32 wx::gl wx::core wx::base)
