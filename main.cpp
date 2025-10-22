#include "rt.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

int window_width = 1000;

void initialize_sdl(int window_width, int render_width) {

    int window_height = static_cast<int>(window_width*9.0/16.0);
    int render_height = static_cast<int>(render_width*9.0/16.0);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Unable to Initialize anything" << std::endl;
    }

    // TODO: Check width !> window size

    window = SDL_CreateWindow("Ray Tracer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Unable to initialize window" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Unable to initialize renderer" << std::endl;
    }

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        render_width, render_height);
    if (!texture) {
        std::cerr << "Unable to initialize texture" << std::endl;
    }
}

void destroy(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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

    window_width = 800;

    cam.aspect_ratio    = 16.0 / 9.0;
    cam.render_width     = 500;

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
                    //auto center2 = center + point3(0,random_double(0,0.5), 0);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
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

    window_width = 1000;

    world = hittable_list(make_shared<bvh_node>(world));

    cam.aspect_ratio    = 16.0 / 9.0;
    cam.render_width     = 1000;

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
    initialize_sdl(window_width, cam.render_width);
    auto start_time =  SDL_GetTicks();
    if (window) {
        SDL_FlashWindow(window, SDL_FLASH_UNTIL_FOCUSED);
        SDL_RaiseWindow(window);
    }
    bool exit = cam.render(world, renderer, texture);
    auto end_time = SDL_GetTicks();
    if (exit) {
        destroy();
        return 0;
    }
    auto time = end_time - start_time;
    std::cout << "Time taken to render: " << time/1000.0f << std::endl;
    while (!exit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                destroy();
                exit = true;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                destroy();
                exit = true;
            }
        }
        // Updating to make sure there are no visual defects after the program has rendered everything
        SDL_UpdateTexture(texture, nullptr, cam.pixels.data(), cam.render_width*3 );
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    }
    destroy();

    return 0;
}
