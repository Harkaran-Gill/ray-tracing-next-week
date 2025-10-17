//
// Created by harka on 22-08-2025.
//

#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

inline void write_color(std::ofstream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //normalize values, [0,1] to [0,255]
    static const interval intensity(0.000, 0.999);
    //std::cerr << intensity.clamp(r) << intensity.clamp(b) << '\n';
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // write out pixels
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n' << std::flush;
}

#endif //COLOR_H
