cmake_minimum_required(VERSION 3.0.0)
project(VoxelGL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_definitions("-Wall" "-g")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)



add_executable(VoxelGL ${SRC_FILES})
target_include_directories(VoxelGL PRIVATE include/)
target_link_directories(VoxelGL PRIVATE lib/)
target_link_libraries(VoxelGL glfw3 opengl32 gdi32)
