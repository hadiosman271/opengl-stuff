cmake_minimum_required(VERSION 3.10)

project(lighting C)

file(GLOB SRC src/*.c src/*.h)
add_executable(lighting ${SRC} ../dep/glad/src/glad.c)
target_include_directories(lighting PRIVATE
	src ../dep ../dep/glfw/include ../dep/glad/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(../dep/glfw ../../dep/glfw/build)
target_link_libraries(lighting glfw)
