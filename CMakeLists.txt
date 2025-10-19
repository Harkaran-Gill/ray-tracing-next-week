cmake_minimum_required(VERSION 3.31)
project(RayTracingNextWeek)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Change this path to your installed location of SDL2
set(CMAKE_PREFIX_PATH "C:/SDL2")

find_package(SDL2 REQUIRED CONFIG)

add_executable(RayTracing main.cpp)

# Release Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(RayTracing PRIVATE -O3 -march=native -ffast-math -DNDEBUG)
endif ()

target_link_libraries(RayTracing PRIVATE SDL2::SDL2 SDL2::SDL2main)

