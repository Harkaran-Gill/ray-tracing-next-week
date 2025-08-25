//
// Created by harka on 24-08-2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "rt.h"

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;

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
                    pixel_color += ray_color(r, world);
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
    point3 pixel00_loc;             // Center point location of the upper most left pixel
    vec3 pixel_delta_u;             // Offset of pixel to the right
    vec3 pixel_delta_v;             // Offset of pixel below

    void initialize() {
        //calculate the height of the image, and set = 1, if less than 1
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center =  point3(0, 0, 0);

        //set the viewport height and calculate the width
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width  = viewport_height * (double(image_width) / image_height);


        //calculate vectors along height and width
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        //calculate individual pixel height and width i.e delta u & v
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //calculate the location of upper left pixel
        auto viewport_upper_left = camera_center + vec3(-viewport_width/2, viewport_height/2, -focal_length);

        //calculating the center of the upper left pixel
        pixel00_loc = viewport_upper_left + ((pixel_delta_u + pixel_delta_v)/2);


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

    // TODO: implement a non square version to experiment with non-square pixels
    vec3 sample_square() {
        // Returns the vector to a random point in the [-0.5, -0.5] - [0.5, 0.5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        // -0.5 because random_double() returns in range [0, 1]

    }

    color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, interval(0, infinity),rec)) {
            return 0.5 * color(rec.normal + color(1, 1, 1));
        }

        //calculating the background gradient
        vec3 unit_direction = unit_vector(r.direction());
        auto a = (unit_direction.y() + 1) * 0.5;
        return (1-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

};

#endif //CAMERA_H
