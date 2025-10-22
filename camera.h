//
// Created by harka on 24-08-2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "rt.h"
#include <SDL.h>

class camera {
public:
    double aspect_ratio     = 1.0;      // Ratio of image width over height
    int render_width        = 100;      // Width of Rendered image in pixels
    int samples_per_pixel   = 10;       // Number random samples for each pixel
    int max_depth           = 10;       // Maximum number of ray bounces in row
    std::vector<uint8_t> pixels;        // Pixel array for image
    double vfov       = 90;                 // Vertical field of view of camera
    point3 lookfrom   = point3(0, 0, 0);    // The position to look from/ camera center
    point3 lookat     = point3(0 ,1 , 0);   // The position camera is pointing towards
    vec3 vup          = vec3(0, 1, 0);      // Camera relative 'up' direction

    double defocus_angle    = 0;     // Variation angle of rays through each pixel
    double focus_dist       = 10;    // Distance between lookfrom and the plane of perfect focus

    bool render(const hittable& world, SDL_Renderer *renderer, SDL_Texture *texture){
        Uint32 time_start = SDL_GetTicks();
        initialize();
        //Render
        std::cout << "P3\n" << render_width << " " << render_height << "\n255\n";

        for (int j = 0; j < render_height; ++j) {
            std::clog << "\rScanLines remaining: " << render_height - j << ' '<<std::flush;
            for (int i = 0; i < render_width; ++i) {
                color pixel_color = color (0, 0, 0);
                for (int samples = 0; samples < samples_per_pixel; ++samples) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(pixel_samples_scale * pixel_color, pixels, j, i, render_width);
            }


            SDL_UpdateTexture(texture, nullptr, pixels.data(), render_width*3 );
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);

            // Process events
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return true;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return true;
                }
            }
        }
        Uint32 time_end = SDL_GetTicks();
        std::clog << "\rDone!  "<< (time_end - time_start)/1000.0<< "               " << render_height << '\n';
        return false;
    }

private:
    int render_height;                // Rendered image height
    double pixel_samples_scale;     // Color scale factor for sum of pixel samples
    point3 camera_center;           // Camera center
    point3 pixel00_loc;             // Center point location of the uppermost left pixel
    vec3 pixel_delta_u;             // Offset of pixel to the right
    vec3 pixel_delta_v;             // Offset of pixel below
    SDL_Event event;
    vec3 u, v, w;                   // Camera frame basis vectors
    vec3 defocus_disk_u;            // Defocus disk horizontal radius
    vec3 defocus_disk_v;            // Defocus disk vertical radius

    void initialize() {
        //calculate the height of the image, and set = 1, if less than 1
        render_height = int(render_width / aspect_ratio);
        render_height = (render_height < 1) ? 1 : render_height;
        pixels.resize(render_width * render_height * 3);

        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center =  lookfrom;

        // calculate the viewport height and calculate the width

        // Commenting focal_length for now, using focus_dist to calculate viewport_height,
        // viewport_upper_left
        //auto focal_length = (lookfrom - lookat).length();

        auto theta =  degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2.0 * h * focus_dist;        // 2 is arbitrary height
        auto viewport_width  = viewport_height * (double(render_width) / render_height);

        // Calculate u, v, w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        //calculate vectors along height and width
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        //calculate individual pixel height and width i.e delta u & v
        pixel_delta_u = viewport_u / render_width;
        pixel_delta_v = viewport_v / render_height;

        //calculate the location of upper left pixel
        auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u/2 - viewport_v/2;

        // + vec3(-viewport_width/2, viewport_height/2, -focal_length);
        // The above can be used when looking in the same plane as objects

        //calculating the center of the upper left pixel
        pixel00_loc = viewport_upper_left + ((pixel_delta_u + pixel_delta_v) * 0.5);

        //Calculate camera defocus disk vectors
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle)/2);
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;


    }

    ray get_ray (int i, int j) {
        // Construct a ray from defocus disk and directed at randomly sampled point
        // around the pixel location i, j

        auto offset       = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.x()) * pixel_delta_v);

        auto ray_origin   = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time     = random_double();

        return ray(ray_origin, ray_direction, ray_time);


    }

    // TODO: implement a non-square version to experiment with non-square pixels
    static vec3 sample_square() {
        // Returns the vector to a random point in the [-0.5, -0.5] - [0.5, 0.5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        // -0.5 because random_double() returns in range [0, 1]
    }

    point3 defocus_disk_sample() const{
        // Returns a random point in camera defocus disk
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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
