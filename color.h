//
// Created by harka on 22-08-2025.
//

#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color = vec3;

inline void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //normalize values, [0,1] to [0,255]
    static const interval intensity(0.000, 0.999);
    //std::cerr << intensity.clamp(r) << intensity.clamp(b) << '\n';
    int rbyte = int(255.999 * r);
    int gbyte = int(255 * intensity.clamp(g));
    int bbyte = int(255 * intensity.clamp(b));

    // write oue pixels
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //COLOR_H
