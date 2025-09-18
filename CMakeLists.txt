cmake_minimum_required(VERSION 3.31)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)

# Change this path to your installed location of SDL2
set(CMAKE_PREFIX_PATH "C:/SDL2")

find_package(SDL2 REQUIRED CONFIG)

add_executable(RayTracing main.cpp)

target_link_libraries(RayTracing PRIVATE SDL2::SDL2 SDL2::SDL2main)



