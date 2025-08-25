# C++ Ray Tracer

A simple, CPU-based ray tracer built in C++ by following Peter Shirley's "Ray Tracing in One Weekend" book series. This project serves as a personal exploration into the fundamentals of computer graphics and rendering algorithms.

<!-- =================================================================== -->
<!--                  ADD YOUR BEST RENDER IMAGE HERE                  -->
<!-- =================================================================== -->
<!-- First, add a great-looking PNG image to your repository. Then,     -->
<!-- uncomment the line below and replace the placeholder.              -->
<!-- =================================================================== -->
<!-- ![My Ray Tracer Render](images/final_render.png) -->

## About This Project

This program generates photorealistic images by simulating the path of light rays as they interact with objects in a 3D scene. The implementation is based on the concepts from the "Ray Tracing in One Weekend" series, which covers the essentials of writing a path tracer from scratch.

## Features Implemented


- [x] Vector and Ray utility classes
- [x] Basic object rendering (spheres)
- [x] Surface normals and shading
- [x] Hittable objects and lists
- [x] **Anti-aliasing** using multisampling
- [ ] Diffuse (Lambertian) materials
- [ ] Metal materials with reflection
- [ ] Dielectric materials with refraction (glass, water)
- [ ] Positionable camera with depth of field
- [ ] Bounding Volume Hierarchy (BVH) for performance optimization

## Getting Started

### Prerequisites

To build and run this project, you will need:
- A modern C++ compiler (C++17 or newer)
- [CMake](https://cmake.org/download/) (version 3.10 or higher)
- [Git](https://git-scm.com/downloads)
- Your preferred Code-Editor

<!-- =================================================================== -->
<!--    OPTIONAL: If you used other libraries, add them here.           -->
<!-- =================================================================== -->
<!-- For example:                                                     -->
<!-- - [SDL2](https://www.libsdl.org/) for real-time window display    -->
<!-- =================================================================== -->

### Building the Project


#### For Visual Studio:

Follow these steps to clone and build the ray tracer from source:
1. Clone the repo to your machine by running the following command: 

   git clone https://github.com/Harkaran-Gill/RayTracer.git


2. Create a build directory  
      mkdir build  
      cd build


3. Configure the project with CMake
   On Windows, you might need to specify the generator:  
   cmake .. -G "Visual Studio 17 2022"  
   cmake ..


4. Build the project
   This will create an executable in the build directory  
   cmake --build . --config Release


#### For CLion:
1. Clone the repo to your machine by running the following command:  
   git clone https://github.com/Harkaran-Gill/RayTracer.git
    

2. Open the directory with the cloned repo in CLion, build the project using the build 
button.  


## Running the executable
As of now running the executable requires you to redirect the output the program
to a file using '>' operator. Run the following commands on the terminal.

#### For CLion:
.\cmake-build-debug\RayTracing.exe > image.ppm

#### For Visual Studio 
Run this in the same directory as your .exe file.  
.\RayTracing.exe > image.ppm
