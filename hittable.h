//
// Created by harka on 23-08-2025.
//

#ifndef HITTABLE_H
#define HITTABLE_H

class material;

class hit_record {
public:
    point3 p;                   // The point where the ray hits
    vec3 normal;                // Surface normal of the point where the ray hit
    shared_ptr<material> mat;
    double t;                   // The root of the function of ray, since ray is just a line
    bool front_face;            // Storing if the ray is facing inwards or outwards

    //
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        //sets the hit record normal vector
        //outward normal is assumed to be of unit length

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;

    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif //HITTABLE_H
