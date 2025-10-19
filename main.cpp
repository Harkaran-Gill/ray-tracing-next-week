#include <chrono>
#include "rt.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


static void scene1(hittable_list& world, camera& cam) {
    auto ground_material     = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_dielectric = make_shared<dielectric>(1.5);
    auto material_bubble     = make_shared<dielectric>(1.0/1.5);
    auto material_metal      = make_shared<metal>(color(0.2,0.5,0.7), 0);
    auto material_lambertian = make_shared<lambertian>(color(0.2, 0.2, 0.8));

    auto sphere_ground  = make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material);
    auto sphere_left    = make_shared<sphere>(point3(-1,0.5,-1), 0.49, material_dielectric);
    auto sphere_bubble  = make_shared<sphere>(point3(-1,0.5,-1), 0.45, material_bubble);
    auto sphere_middle  = make_shared<sphere>(point3(0,0.5,-1), 0.49, material_metal);
    auto sphere_right   = make_shared<sphere>(point3(1,0.5,-1), 0.49, material_lambertian);

    world.add(sphere_ground);
    world.add(sphere_left);
    // world.add(sphere_bubble);
    world.add(sphere_middle);
    world.add(sphere_right);


    cam.aspect_ratio    = 16.0 / 9.0;
    cam.image_width     = 500;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;

    cam.vfov     = 60;
    cam.lookfrom = point3(0, 0.5, 1);
    cam.lookat   = point3(0 ,0.45 ,-1);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.0;
    cam.focus_dist    = 10.0;

}

static void scene2(hittable_list& world, camera& cam) {
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_mat = random_double(0, 1.0);      // choose material based on probability

            // choose position on the plane with some randomness
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Lambertian
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + point3(0,random_double(0,0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }

                else if (choose_mat < 0.95) {
                    auto albedo = color::random() * color::random();
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }

                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0);

    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    cam.aspect_ratio    = 16.0 / 9.0;
    cam.image_width     = 400;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat   = point3(0 ,0 ,0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.0;
    cam.focus_dist    = 10.0;
}

int main() {
    //set the world
    hittable_list world;
    camera cam;
    while (true) {
        std::cout << "Please enter the scene number to render: " << std::endl;
        std::cout << "1: Scene-1, Simple scene with only 3-Spheres" << std::endl;
        std::cout << "2: Scene-2, A more complex scene with more than 50 spheres" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            scene1(world, cam);
            break;
        }
        if (choice == 2) {
            scene2(world, cam);
            break;
        }
        else {
            std::cout << "Please enter a valid choice number" << std::endl;
        }
    }
    auto start_time =  std::chrono::system_clock::now();
    cam.render(world);
    auto end_time = std::chrono::system_clock::now();
    auto time = end_time - start_time;
    std::cout << "\nTime taken to render: " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count()/(1000.0) << std::endl;
    return 0;
}
