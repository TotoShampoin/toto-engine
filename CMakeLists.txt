cmake_minimum_required(VERSION 3.12)

project(toto-engine)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(vendor/glad)
find_package(glm REQUIRED)

add_library(toto-engine ${SOURCES})

target_include_directories(toto-engine PUBLIC include)

target_link_libraries(toto-engine PUBLIC glfw OpenGL glad glm)
