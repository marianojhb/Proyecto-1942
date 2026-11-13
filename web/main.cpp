#include <SDL.h>
#include <emscripten/emscripten.h>
#include <algorithm>
#include <iostream>

namespace {
constexpr int kWidth = 600;
constexpr int kHeight = 800;
constexpr int kPlayerSize = 36;
constexpr int kSpeed = 5;

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int playerX = kWidth / 2;
    int playerY = kHeight - 90;
    bool running = true;
};

void frame(void* context) {
    auto& app = *static_cast<App*>(context);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) app.running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            app.running = false;
    }
    if (!app.running) {
        emscripten_cancel_main_loop();
        return;
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) app.playerX -= kSpeed;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) app.playerX += kSpeed;
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) app.playerY -= kSpeed;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) app.playerY += kSpeed;
    app.playerX = std::clamp(app.playerX, 0, kWidth - kPlayerSize);
    app.playerY = std::clamp(app.playerY, 0, kHeight - kPlayerSize);

    SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
    SDL_RenderClear(app.renderer);
    SDL_Rect player{app.playerX, app.playerY, kPlayerSize, kPlayerSize};
    SDL_SetRenderDrawColor(app.renderer, 90, 210, 245, 255);
    SDL_RenderFillRect(app.renderer, &player);
    SDL_RenderPresent(app.renderer);
}
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }
    App app;
    app.window = SDL_CreateWindow("Proyecto 1942 - Web POC",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  kWidth, kHeight, SDL_WINDOW_SHOWN);
    if (!app.window) {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer) {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }
    emscripten_set_main_loop_arg(frame, &app, 0, 1);
    return 0;
}
