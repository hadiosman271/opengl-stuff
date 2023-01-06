cmake_minimum_required(VERSION 3.10)

project(cubes C)

file(GLOB SRC src/*.c src/*.h ../lib/glad/src/glad.c)
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE
	src ../lib ../lib/cglm ../lib/glad/include ../lib/glfw/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(../lib/glfw ../../lib/glfw/build EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
