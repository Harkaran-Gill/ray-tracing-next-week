//
// Created by harka on 24-08-2025.
//

#ifndef RT_H
#define RT_H

#include <cmath>
#include <random>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>


// C++ std usings
using std::make_shared;
using std::shared_ptr;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions
inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

//returns a random, real number in range [0, 1)
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

//returns a random, real number in range [min, max)
inline double random_double(double min, double max) {
    return min + (max-min) * random_double();
}

//common headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RT_H
