#include <SDL2/SDL.h>
#undef main
#include <Scene/AntiAliasing.h>
#include <Scene/ColorShowdown.h>
#include <Scene/ExampleScene1.h>
#include <Scene/ExampleScene2.h>
#include <Scene/Raytracing.h>
#include <Scene/Raytracing2.h>
#include <Scene/Reflection.h>
#include <Scene/Scene.h>
#include <Scene/Texture.h>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define WIDTH 400
#define HEIGHT 400
#define LATENCY_MIN 16.6f// 60 FPS

#define COLOR_SHOWDOWN 1
#define RAY_TRACING 2
#define RAY_TRACING2 3
#define REFLECTION 4
#define TEXTURE 5
#define ANTI_ALIASING 6
#define SCENE1 7
#define SCENE2 8

#define ACTIVE 4

#if ACTIVE == 6
#undef WIDTH
#undef HEIGHT
#define WIDTH 800
#define HEIGHT 800
#endif



class Screen {
private:
    int running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Scene *scene;

public:
    Screen() : running(0) {
        int textureWidth;
        int textureHeight;
        int channels;
        unsigned char *img;
        switch (ACTIVE) {
            case COLOR_SHOWDOWN:
                scene = new ColorShowdown();
                return;
            case RAY_TRACING:
                scene = new Raytracing(WIDTH, HEIGHT);
                return;
            case RAY_TRACING2:
                scene = new Raytracing2(WIDTH, HEIGHT);
                return;
            case REFLECTION:
                scene = new Reflection(WIDTH, HEIGHT);
                return;
            case TEXTURE:
                img = stbi_load("../lava.bmp", &textureHeight, &textureWidth, &channels, 0);
                scene = new Texture(WIDTH, HEIGHT, img, textureWidth, textureHeight,channels);

                return;
            case ANTI_ALIASING:
                scene = new AntiAliasing(WIDTH, HEIGHT);
                return;
            case SCENE1:
                scene = new ExampleScene1(WIDTH, HEIGHT);
                return;
            case SCENE2:
                scene = new ExampleScene2(WIDTH, HEIGHT);
                return;
        }
    }

    ~Screen() { this->stop(); }

    void start() {
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            return;
        }
        if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window,
                                        &renderer)) {
            return;
        }
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                                    SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
        //std::thread drawThread(&Screen::drawLoop, this);
        std::thread tickThread(&Screen::tickLoop, this);
        SDL_Event event;
        this->running = 1;
        drawLoop();
        tickThread.detach();
    }

    void tickLoop() {
        while (running) {
            tick();
            // Reduce CPU usage
            SDL_Delay(10);
        }
    }

    void drawLoop() {
        auto lastTicks = SDL_GetTicks();
        while (running) {
            draw();
            auto newTicks = SDL_GetTicks();
            double latency = (double) newTicks - (double) lastTicks;
            if (latency < LATENCY_MIN) {
                // Reduce GPU usage
                SDL_Delay(LATENCY_MIN - latency);
            }
            lastTicks = SDL_GetTicks();
        }
    }

    void stop() {
        if (nullptr != renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (nullptr != window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        SDL_Quit();
    }

    void tick() { scene->GetPixels(); }

    void draw() {
        if (scene->pixels == nullptr) return;
        SDL_RenderClear(renderer);
        SDL_UpdateTexture(texture, nullptr, scene->pixels, WIDTH * sizeof(int));
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
};

int main() {
    Screen screen;
    screen.start();
    return 0;
}
