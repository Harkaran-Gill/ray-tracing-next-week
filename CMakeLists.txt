cmake_minimum_required(VERSION 3.31)
project(RayTracingNextWeek)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Change this path to your installed location of SDL2
if (CMAKE_CXX_PLATFORM_ID MATCHES "MINGW")
    set(CMAKE_PREFIX_PATH "C:/SDL2/SDL2-2.30.10-mingw")
else ()
    set(CMAKE_PREFIX_PATH "C:/SDL2/SDL2-2.30.10-VC")
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "-mconsole")
endif()
find_package(SDL2 REQUIRED CONFIG)

add_executable(RayTracing main.cpp)

# Release Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(RayTracing PRIVATE
            $<$<CONFIG:Release>:-O3>
            $<$<CONFIG:Release>:-march=native>
            $<$<CONFIG:Release>:-ffast-math> )
endif ()
target_compile_definitions(RayTracing PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
)

target_link_libraries(RayTracing PRIVATE SDL2::SDL2)

