cmake_minimum_required(VERSION 3.31)
project(RayTracingNextWeek)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RayTracing main.cpp)

# Release Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(RayTracing PRIVATE
            $<$<CONFIG:Release>:-O3>
            $<$<CONFIG:Release>:-march=native>
            $<$<CONFIG:Release>:-ffast-math>)
endif ()
target_compile_definitions(RayTracing PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
)
