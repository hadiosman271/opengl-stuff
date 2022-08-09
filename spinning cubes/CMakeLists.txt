cmake_minimum_required(VERSION 3.10)

project(cubes C)

file(GLOB SRC src/*.c src/*.h)
add_executable(cubes ${SRC} ../dep/glad/src/glad.c)
target_include_directories(cubes PRIVATE
	src ../dep ../dep/cglm ../dep/glfw/include ../dep/glad/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(../dep/glfw ../../dep/glfw/build)
target_link_libraries(cubes glfw)
