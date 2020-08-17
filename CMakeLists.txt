project(Peril-Engine)

cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_FLAGS"-Wall -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wcast-qual -Wno-missing-braces")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")



if(BUILD_LIBRARY)
    message(STATUS "Building Peril as library")
    add_library(${PROJECT_NAME} src/core.cpp src/Peril.cpp)
else()
    message(STATUS "Building Peril as application")
    add_executable(${PROJECT_NAME} src/core.cpp src/Peril.cpp src/main.cpp)
endif()

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2GFX REQUIRED SDL2_gfx)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2GFX_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2GFX_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test.map $<TARGET_FILE_DIR:${PROJECT_NAME}>/test.map)
