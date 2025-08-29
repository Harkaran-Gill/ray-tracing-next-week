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

void initialize_sdl(int width) {

    int height = static_cast<int>(width*9.0/16.0);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Unable to Intialize anything" << std::endl;
    }

    // TODO: Check width !> window size

    window = SDL_CreateWindow("Ray Tracer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);

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
        width, height);
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

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color (0.4, 0.4, 0.4));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00/1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 640;

    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    initialize_sdl(cam.image_width);
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
        SDL_Delay(50);
    }
    return 0;
}
