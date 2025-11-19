#include "rt.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"


static void wide_angle_spheres(hittable_list &world, camera &cam) {
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_dielectric = make_shared<dielectric>(1.5);
    auto material_bubble = make_shared<dielectric>(1.0 / 1.5);
    auto material_metal = make_shared<metal>(color(0.2, 0.5, 0.7), 0);
    auto material_lambertian = make_shared<lambertian>(color(0.2, 0.2, 0.8));

    auto sphere_ground = make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material);
    auto sphere_left = make_shared<sphere>(point3(-1, 0.5, -1), 0.49, material_dielectric);
    auto sphere_bubble = make_shared<sphere>(point3(-1, 0.5, -1), 0.45, material_bubble);
    auto sphere_middle = make_shared<sphere>(point3(0, 0.5, -1), 0.49, material_metal);
    auto sphere_right = make_shared<sphere>(point3(1, 0.5, -1), 0.49, material_lambertian);

    world.add(sphere_ground);
    world.add(sphere_left);
    world.add(sphere_bubble);
    world.add(sphere_middle);
    world.add(sphere_right);


    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 40;
    cam.lookfrom = point3(0, 0.5, 1);
    cam.lookat = point3(0, 0.45, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;
}

static void bouncing_spheres(hittable_list &world, camera &cam) {
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_mat = random_double(0, 1.0); // choose material based on probability

            // choose position on the plane with some randomness
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Lambertian
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    //auto center2 = center + point3(0, random_double(0, 0.2), 0);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random() * color::random();
                    auto fuzz = random_double(0, 0.4);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
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

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 30;
    cam.max_depth         = 10;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;
}

static void zoomed_spheres(hittable_list &world, camera &cam) {
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 30;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;
}

static void checkered_spheres(hittable_list &world, camera &cam) {
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

static void earth(hittable_list &world, camera &cam) {
    auto earth_texture = make_shared<image_texture>("earthmap2.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
    world.add(globe);

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

static void perlin(hittable_list &world, camera &cam) {
    auto per_text = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(per_text)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(per_text)));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 100;
    cam.max_depth         = 10;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

static void quads(hittable_list &world, camera &cam) {
    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

static void ellipses(hittable_list &world, camera &cam) {
    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // ellipses and disks
    world.add(make_shared<disk>(point3(-3,-2, 4), vec3(0, 0,-4), vec3(0, 4, 0), 1, left_red));
    world.add(make_shared<ellipse>(point3(0, 0, 0), vec3(2, 0, 0), vec3(0, 1, 0), back_green));
    world.add(make_shared<disk>(point3( 3,0, 1), vec3(0, 0, 4), vec3(0, 4, 0), 1, right_blue));
    world.add(make_shared<disk>(point3(-2, 1, 1), vec3(4, 2, 0), vec3(0, 0, 4),1, upper_orange));
    world.add(make_shared<disk>(point3(-2.8,-3, 4), vec3(4, 0, 0), vec3(0, 0,-4), 1, lower_teal));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;

    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

static void simple_light(hittable_list& world, camera& cam) {
    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    // world.add(make_shared<quad>(point3(1,1,6), vec3(2,0,0), vec3(0,2,0), difflight));

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
}

static void cornell_box(hittable_list& world, camera& cam) {

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
}

int main() {
    //set the world
    hittable_list world;
    camera cam;
    std::cout << "Please enter the scene number to render: " << std::endl;
    std::cout << "01: Scene-01, Simple scene with only 3 Spheres" << std::endl;
    std::cout << "02: Scene-02, A more complex scene with more than 50 Spheres" << std::endl;
    std::cout << "03: Scene-03, Another scene with 3 Spheres" << std::endl;
    std::cout << "04: Scene-04, Checkered Spheres" << std::endl;
    std::cout << "05: Scene-05, Earth Model" << std::endl;
    std::cout << "06: Scene-06, Perlin Spheres" << std::endl;
    std::cout << "07: Scene-07, Quadrilaterals" << std::endl;
    std::cout << "08: Scene-08, Disks and Ellipses " << std::endl;
    std::cout << "09: Scene-09, A Simple light for lighting " << std::endl;
    std::cout << "10: Scene-10, Cornell Box " << std::endl;

    int choice = 10;
    if (false)
        std::cin >> choice;
    switch (choice) {
        case 1: wide_angle_spheres(world, cam);
            break;
        case 2: bouncing_spheres(world, cam);
            break;
        case 3: zoomed_spheres(world, cam);
            break;
        case 4: checkered_spheres(world, cam);
            break;
        case 5: earth(world, cam);
            break;
        case 6: perlin(world, cam);
            break;
        case 7: quads(world, cam);
            break;
        case 8: ellipses(world, cam);
            break;
        case 9: simple_light(world, cam);
            break;
        case 10: cornell_box(world, cam);
            break;

        default:
            std::cout << "Please enter a valid choice number" << std::endl;
    }
    auto start_time = std::chrono::system_clock::now();
    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world);
    auto end_time = std::chrono::system_clock::now();
    auto time = end_time - start_time;
    std::cout << "\nTime taken to render: " <<
            double(std::chrono::duration_cast<std::chrono::milliseconds>(time).count()) / (1000.0) << std::endl;
    return 0;
}
