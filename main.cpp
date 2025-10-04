#include "rt.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include <SDL.h>
#include "sphere.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

int window_width;

void initialize_sdl(int window_width, int render_width) {

    int window_height = static_cast<int>(window_width*9.0/16.0);
    int render_height = static_cast<int>(render_width*9.0/16.0);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Unable to Intialize anything" << std::endl;
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
}
int main() {
    //set the world
    hittable_list world;

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
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
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

    window_width = 900;

    camera cam;
    cam.aspect_ratio    = 16.0 / 9.0;
    cam.render_width     = 600;

    cam.samples_per_pixel = 40;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat   = point3(0 ,0 ,0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    initialize_sdl(window_width, cam.render_width);
    bool exit = cam.render(world, renderer, texture);
    if (exit) {
        destroy();
        return 0;
    }
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
        SDL_Delay(500);
    }
    return 0;
}
