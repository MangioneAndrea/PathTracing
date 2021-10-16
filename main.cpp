#include <SDL2/SDL.h>
#undef main
#include <Scene/ColorShowdown.h>
#include <Scene/Raytracing.h>
#include <Scene/Raytracing2.h>
#include <Scene/Reflection.h>
#include <Scene/Scene.h>
#include <thread>

#define WIDTH 400
#define HEIGHT 400
#define LATENCY_MIN 16.6f// 60 FPS

#define COLOR_SHOWDOWN 1
#define RAY_TRACING 2
#define RAY_TRACING2 3
#define REFLECTION 4

#define ACTIVE 2

class Screen {
private:
    int running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Scene *scene;

public:
    Screen() : running(0) {
        switch (ACTIVE) {
            case COLOR_SHOWDOWN:
                scene = new ColorShowdown();
                return;
            case RAY_TRACING:
                scene = new Raytracing();
                return;
            case RAY_TRACING2:
                scene = new Raytracing2(WIDTH, HEIGHT);
                return;
            case REFLECTION:
                scene = new Reflection(WIDTH, HEIGHT);
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
        std::thread drawThread(&Screen::drawLoop, this);
        std::thread tickThread(&Screen::tickLoop, this);
        SDL_Event event;
        this->running = 1;
        while (running) {
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = 0;
            }
        }
        drawThread.detach();
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
