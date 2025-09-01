cmake_minimum_required(VERSION 3.31)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)

# For mingw:
 set(SDL2_DIR "C:/SDL2/SDL2-2.0.12/x86_64-w64-mingw32/")
include_directories(${SDL2_DIR}/include/SDL2)
link_directories(${SDL2_DIR}/lib)

# For VS
#set(SDL2_DIR "C:/SDL2/SDL2-2.30.10")

#include_directories(${SDL2_DIR}/include)
#link_directories(${SDL2_DIR}/lib/x64)

add_executable(RayTracing main.cpp)

target_link_libraries(RayTracing PRIVATE SDL2)



