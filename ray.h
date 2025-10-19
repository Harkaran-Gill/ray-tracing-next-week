//
// Created by harka on 22-08-2025.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {};

    ray (const point3& origin, const vec3& direction, const double time)
     : orig(origin), dir(direction),tm(time){}

    ray (const point3& origin, const vec3& direction)
     : ray(origin, direction, 0) {}

    [[nodiscard]] const point3& origin() const {return orig; }
    [[nodiscard]] const vec3& direction() const {return dir; }
    [[nodiscard]] double time() const{ return tm ;}

    [[nodiscard]] point3 at(double t) const {
        return orig + t*dir;
    }

private:
    point3 orig;
    vec3 dir;
    double tm;
};
#endif //RAY_H
