//
// Created by harka on 20-08-2025.
//

#ifndef VEC3_H
#define VEC3_H

class vec3 {
public:
    double e[3];

    //Constructors
    // the array initialization after colon is done when you want to initialize before constructor body
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    //Below are getter functions
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const {return {-e[0], -e[1], -e[2]}; }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    bool near_zero() const {
        // return true if near zero in all axes
        // Using std::fabs() to get absolute value
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
     }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min,max));
    }
};
//an alias for vec3, useful for geometric clarity
using point3 = vec3;

//other vector functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0]<< " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0]+v.e[0], u.e[1]+v.e[1], u.e[2]+v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0]-v.e[0], u.e[1]-v.e[1], u.e[2]-v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

inline vec3 operator*(const vec3& u, double t) {
    return vec3(u.e[0]*t, u.e[1]*t, u.e[2]*t);
}

inline vec3 operator*(double t , const vec3& u) {
    return u * t;
}

inline vec3 operator/(const vec3& u, double t) {
    return u * (1/t);
}

inline double dot(const vec3& u, const vec3& v) {
    return (u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2]);
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        -(u.e[0] * v.e[2] - u.e[2]*v.e[0]),
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1, 1);
        auto length_sq = p.length_squared();
        if (length_sq < 1) {
            return p / std::sqrt(length_sq);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    auto rand_u_vec = random_unit_vector();
    if (dot(normal, rand_u_vec) > 0) {
        return rand_u_vec;
    }

    else
        return -rand_u_vec;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
        return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etaI_over_etaT) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etaI_over_etaT * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif //VEC3_H
