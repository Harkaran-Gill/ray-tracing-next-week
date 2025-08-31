//
// Created by harka on 24-08-2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "rt.h"

class camera {
public:
    double aspect_ratio     = 1.0;      // Ratio of image width over height
    int image_width         = 100;      // Width of Rendered image in pixels
    int samples_per_pixel   = 10;       // Number random samples for each pixel
    int max_depth           = 10;       // Maximum number of ray bounces in row

    double vfov             = 90;
    point3 lookfrom         = point3(0, 0, 0);
    point3 lookat           = point3(0 ,1 , 0);
    vec3 vup                = vec3(0, 1, 0);

    void render(const hittable& world){
        initialize();

        //Render
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanLines remaining: " << image_height - j << ' '<<std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color = color (0, 0, 0);
                for (int samples = 0; samples < samples_per_pixel; ++samples) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDone!                           " << image_height << '\n';
    }

private:
    int image_height;               // Rendered image height
    double pixel_samples_scale;     // Color scale factor for sum of pixel samples
    point3 camera_center;           // Camera center
    point3 pixel00_loc;             // Center point location of the uppermost left pixel
    vec3 pixel_delta_u;             // Offset of pixel to the right
    vec3 pixel_delta_v;             // Offset of pixel below
    vec3 u, v, w;                   // Camera frame basis vectors

    void initialize() {
        //calculate the height of the image, and set = 1, if less than 1
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center =  lookfrom;

        // calculate the viewport height and calculate the width
        auto focal_length = (lookfrom - lookat).length();
        auto theta =  degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2.0 * h * focal_length;        // 2 is arbitrary height
        auto viewport_width  = viewport_height * (double(image_width) / image_height);

        // Calculate u, v, w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        //calculate vectors along height and width
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        //calculate individual pixel height and width i.e delta u & v
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //calculate the location of upper left pixel
        auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        // + vec3(-viewport_width/2, viewport_height/2, -focal_length);
        // The above can be used when looking

        //calculating the center of the upper left pixel
        pixel00_loc = viewport_upper_left + ((pixel_delta_u + pixel_delta_v) * 0.5);


    }

    ray get_ray (int i, int j) {
        // Construct from camera origin and directed at randomly sampled point
        // around the pixel location i, j

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.x()) * pixel_delta_v);

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);


    }

    // TODO: implement a non-square version to experiment with non-square pixels
    vec3 sample_square() {
        // Returns the vector to a random point in the [-0.5, -0.5] - [0.5, 0.5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        // -0.5 because random_double() returns in range [0, 1]

    }

    color ray_color(const ray& r, int depth, const hittable& world ) {
        // If ray depth is exceeded no more light is gathered
        if (depth <= 0) {
            return color(0,0,0);
        }

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity),rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return color(0, 0, 0);
        }

        //calculating the background gradient
        vec3 unit_direction = unit_vector(r.direction());
        auto a = (unit_direction.y() + 1) * 0.5;
        return (1-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

};

#endif //CAMERA_H
