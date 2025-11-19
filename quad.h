//
// Created by harka on 03-11-2025.
//

#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "hittable_list.h"

class quad : public hittable {
public:
    quad(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
        set_bounding_box();
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
    }

    void set_bounding_box() {
        // Make this virtual?
        // Compute bounding box of all four vertices,
        // since a quad can be a parallelogram as well
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit, if ray is parallel to the plane
        if (std::fabs(denom) <= 1e-8)
            return false;

        // Return false if hit point parameter t is outside the ray interval
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Determine if the hit point lies within planar shape using its plane coordinates
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double alpha, double beta, hit_record &rec) const {
        interval unit_interval = interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
            return false;

        rec.u = alpha;
        rec.v = beta;
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

class disk : public hittable {
public:
    // For this class u and v vectors are purely for orientation and not for the scale of
    // the disk. The disk size is determined by the radius
    disk(const point3 &center, const vec3 &u_, const vec3 &v_, double radius, shared_ptr<material> mat)
        : center(center), u(unit_vector(u_)), v(unit_vector(v_)), radius(radius),
          mat(mat) {
        auto n = cross(u, v);
        normal = unit_vector(n);
        set_bounding_box();
        D = dot(normal, center);
        w = n / dot(n, n);
    }

    void set_bounding_box() {
        bbox = aabb(center - radius * (u + v), center + radius * (u + v));
    }

    [[nodiscard]] aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) <= 1e-8)
            return false;

        // Return false if t is outside ray interval
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - center;

        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        rec.t = t;
        rec.p = intersection;
        rec.set_face_normal(r, normal);
        rec.mat = mat;
        return true;
    }

    virtual bool is_interior(const double alpha, const double beta, hit_record &rec) const {
        auto unit_interval = interval(0, radius);
        auto disk = sqrt(alpha * alpha + beta * beta);
        if (!unit_interval.contains(disk))
            return false;

        rec.u = alpha;
        rec.v = beta;
        return true;
    }

private:
    point3 center;
    vec3 u, v;
    vec3 w;
    vec3 normal;
    shared_ptr<material> mat;
    aabb bbox;
    double D;
    double radius;
};

class ellipse : public hittable {
public:
    // A very similar object to disk
    ellipse(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
        set_bounding_box();
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
    }

    void set_bounding_box() {
        // Make this virtual?
        // Compute bounding box of all four vertices,
        // since a quad can be a parallelogram as well
        auto bbox_diagonal1 = aabb(Q - u - v, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(Q - 2 * (u + v), Q + 2 * (u + v));
    }

    aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit, if ray is parallel to the plane
        if (std::fabs(denom) <= 1e-8)
            return false;

        // Return false if hit point parameter t is outside the ray interval
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Determine if the hit point lies within planar shape using its plane coordinates
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double alpha, double beta, hit_record &rec) const {
        interval unit_interval = interval(0, 2);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.
        auto sqrtt = sqrt(alpha * alpha + beta * beta);
        if (!unit_interval.contains(sqrtt))
            return false;

        rec.u = alpha;
        rec.v = beta;
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

inline shared_ptr<hittable_list> box (const point3& a, const point3& b, shared_ptr<material> mat) {
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.
    auto sides = make_shared<hittable_list>();
    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}
#endif //QUAD_H
