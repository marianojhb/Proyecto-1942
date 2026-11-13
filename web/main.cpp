#include <SDL.h>
#include <SDL_image.h>
#include <emscripten/emscripten.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace {
constexpr int kWidth = 600;
constexpr int kHeight = 800;
constexpr int kSpeed = 5;
constexpr int kScale = 3;
constexpr const char* kPlayerSprite = "assets/sprites/1942-sprites-player.png";
constexpr const char* kProjectileSprite = "assets/sprites/1942-sprites-projectiles.png";
constexpr SDL_Rect kBulletSource{23, 16, 11, 10};
constexpr int kBulletSpeed = 7;
constexpr int kMaxBullets = 5;
constexpr int kReloadFrames = 15;
struct Bullet { int x; int y; };

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* spriteSheet = nullptr;
    SDL_Texture* projectileSheet = nullptr;
    std::vector<Bullet> bullets;
    int reloadFrames = 0;
    int playerX = (kWidth - 96) / 2 + 2;
    int playerY = 700;
    bool running = true;
    bool shooting = false;
    SDL_Rect source{107, 17, 32, 18};
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
    int dx = 0, dy = 0;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) dx += kSpeed;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) dx -= kSpeed;
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) dy -= kSpeed;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) dy += kSpeed;

    // Match the original Personaje.cpp sprite-sheet coordinates.
    app.source = {107, 17, 32, 18};
    if (dx > 0) app.source = {180, 18, 23, 16};
    else if (dx < 0) app.source = {42, 18, 22, 16};

    app.playerX += dx;
    app.playerY += dy;
    const int spriteWidth = app.source.w * kScale;
    const int spriteHeight = app.source.h * kScale;
    app.playerX = std::clamp(app.playerX, -37, kWidth + 37 - spriteWidth);
    app.playerY = std::clamp(app.playerY, 0, kHeight - spriteHeight);

    // Original Personaje::Shoot() fires once per key press, not continuously.
    const bool space = keys[SDL_SCANCODE_SPACE];
    if (space && !app.shooting &&
        app.bullets.size() < kMaxBullets && app.reloadFrames <= 0) {
        // Match Personaje::getBulletOrigin() (96 - 48) / 2 = 24.
        app.bullets.push_back({app.playerX + 24, app.playerY});
        app.reloadFrames = kReloadFrames;
    }
    app.shooting = space;
    for (auto& bullet : app.bullets) bullet.y -= kBulletSpeed;
    app.bullets.erase(
        std::remove_if(app.bullets.begin(), app.bullets.end(),
                       [](const Bullet& bullet) {
                           return bullet.y + kBulletSource.h * kScale < 0;
                       }),
        app.bullets.end());
    if (app.reloadFrames > 0) --app.reloadFrames;

    SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
    SDL_RenderClear(app.renderer);
    SDL_Rect destination{app.playerX, app.playerY, spriteWidth, spriteHeight};
    SDL_RenderCopy(app.renderer, app.spriteSheet, &app.source, &destination);
    for (const auto& bullet : app.bullets) {
        SDL_Rect destinationBullet{bullet.x, bullet.y,
                                  kBulletSource.w * kScale,
                                  kBulletSource.h * kScale};
        SDL_RenderCopy(app.renderer, app.projectileSheet,
                       &kBulletSource, &destinationBullet);
    }
    SDL_RenderPresent(app.renderer);
}
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }
    App app;
    app.window = SDL_CreateWindow("Proyecto 1942 - Player Port",
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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    app.spriteSheet = IMG_LoadTexture(app.renderer, kPlayerSprite);
    if (!app.spriteSheet) {
        std::cerr << "Cannot load player sprites: " << IMG_GetError() << '\n';
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }
    app.projectileSheet = IMG_LoadTexture(app.renderer, kProjectileSprite);
    if (!app.projectileSheet) {
        std::cerr << "Cannot load projectile sprites: " << IMG_GetError() << '\\n';
        SDL_DestroyTexture(app.spriteSheet);
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }
    emscripten_set_main_loop_arg(frame, &app, 0, 1);
    return 0;
}
