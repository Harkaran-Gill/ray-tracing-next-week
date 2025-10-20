//
// Created by harka on 23-08-2025.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
    // Stationary sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
     : center(static_center, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(static_center - rvec, static_center + rvec);   // construct a box around the sphere

        radius_squared = radius * radius;
    }

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius,
        shared_ptr<material> mat)
            : center(center1, (center2-center1)), radius(std::fmax(0,radius)), mat(std::move(mat)) {

        auto rvec = vec3(radius, radius, radius);
        aabb box1 = aabb(center1 - rvec, center1 + rvec);
        aabb box2 = aabb(center2 -rvec, center2 + rvec);
        bbox = aabb(box1, box2);

        radius_squared = radius * radius;
    }

    //This function is called by the hit function of the "hittable_list" class
    bool hit (const ray& r, interval ray_t, hit_record& rec) const override{
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();                      // Ray origin to Sphere center
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius_squared;
        auto discriminant = h*h - a*c;

        if (discriminant < 0) {
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
        vec3 outward_normal = (rec.p - current_center) / radius; //dividing by the radius to turn into a unit vector
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

private:
    ray center;
    double radius;
    double radius_squared;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif //SPHERE_H
