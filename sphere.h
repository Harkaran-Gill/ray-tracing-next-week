//
// Created by harka on 23-08-2025.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius))
        , mat(mat) {
        radius_squared = radius * radius;
    }

    //This function is called by the hit function of the "hittable_list" class
    bool hit (const ray& r, interval ray_t, hit_record& rec) const override{
        vec3 oc = center - r.orig;                      // Ray origin to Sphere center
        auto a = r.dir.length_squared();
        auto h = dot(r.dir, oc);
        auto c = oc.length_squared() - radius_squared;
        auto discriminant = h*h - a*c;

        if (discriminant < 0.0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        //find the nearest root that lies in the acceptable range
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius; //dividing by the radius to turn into a unit vector
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
    double radius_squared;
};

#endif //SPHERE_H
