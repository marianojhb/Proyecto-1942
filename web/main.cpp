#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <emscripten/emscripten.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

namespace {
constexpr int kWidth = 600;
constexpr int kHeight = 800;
constexpr int kSpeed = 5;
constexpr int kScale = 3;
constexpr const char* kPlayerSprite = "assets/sprites/1942-sprites-player.png";
constexpr const char* kProjectileSprite = "assets/sprites/1942-sprites-projectiles.png";
constexpr const char* kEnemySprite = "assets/sprites/1942-sprites-enemy.png";
constexpr const char* kFontFile = "assets/fonts/MONOCOQUE_FUENTE.ttf";
constexpr int kInvulnerableFrames = 90;
constexpr SDL_Rect kEnemySource{59, 57, 15, 14};
constexpr int kEnemyWidth = 45;
constexpr int kEnemyHeight = 42;
constexpr SDL_Rect kEnemyBSource{425, 250, 34, 34};
constexpr SDL_Rect kEnemyBulletSource{73, 14, 4, 4};
constexpr int kEnemyBSize = 68;
constexpr int kMaxEnemyB = 3;
constexpr int kEnemyBSpawnFrames = 120;
constexpr int kEnemyBFireFrames = 120;
constexpr SDL_Rect kBulletSource{23, 16, 11, 10};
constexpr int kBulletSpeed = 7;
constexpr int kMaxBullets = 5;
constexpr int kReloadFrames = 15;
struct Bullet { int x; int y; };
struct Enemy { float x = 300.f, y = -96.f, vx = 1.5f, vy = 3.2f; };
struct EnemyB {
    float x = 0, y = -96, targetX = 240, targetY = 180;
    int fireCooldown = kEnemyBFireFrames;
};
struct HostileBullet { float x, y, vx, vy; bool aimed; };
float randomRange(int min, int max) {
    return static_cast<float>(min + std::rand() % (max - min + 1));
}
void chooseTarget(EnemyB& e) {
    e.targetX = randomRange(34, 500);
    e.targetY = randomRange(68, 468);
}
EnemyB makeEnemyB() {
    EnemyB e;
    e.x = randomRange(34, 550);
    chooseTarget(e);
    return e;
}
void spawnAimedBullet(std::vector<HostileBullet>& shots, float x, float y,
                      float targetX, float targetY) {
    float dx = targetX - x, dy = targetY - y;
    float magnitude = std::sqrt(dx * dx + dy * dy);
    if (magnitude > 0.001f)
        shots.push_back({x, y, dx * 3.5f / magnitude, dy * 3.5f / magnitude, true});
}

void respawnEnemy(Enemy& enemy) { enemy.x = static_cast<float>(std::rand() % kWidth); enemy.y = -96.f; }
bool intersects(const SDL_Rect& a, const SDL_Rect& b) {
    return a.x < b.x + b.w && a.x + a.w > b.x &&
           a.y < b.y + b.h && a.y + a.h > b.y;
}

struct App {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* spriteSheet = nullptr;
    SDL_Texture* projectileSheet = nullptr;
    SDL_Texture* enemySheet = nullptr;
    Enemy enemy;
    std::vector<EnemyB> enemiesB;
    std::vector<HostileBullet> hostileBullets;
    int frames = 0;
    TTF_Font* font = nullptr;
    int score = 0;
    int lives = 3;
    int invulnerableFrames = 0;
    bool gameOver = false;
    std::vector<Bullet> bullets;
    int reloadFrames = 0;
    int playerX = (kWidth - 96) / 2 + 2;
    int playerY = 700;
    bool running = true;
    bool shooting = false;
    SDL_Rect source{107, 17, 32, 18};
};

void drawText(App& app, const std::string& message, int x, int y, int size = 24) {
    if (!app.font) return;
    SDL_Color color{255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(app.font, message.c_str(), color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    if (texture) {
        SDL_Rect dest{x, y, surface->w, surface->h};
        SDL_RenderCopy(app.renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
    (void)size;
}

void resetGame(App& app) {
    app.score = 0;
    app.lives = 3;
    app.invulnerableFrames = 0;
    app.gameOver = false;
    app.playerX = (kWidth - 96) / 2 + 2;
    app.playerY = 700;
    app.bullets.clear();
    app.reloadFrames = 0;
    app.shooting = false;
    app.enemy = Enemy{};
    app.enemiesB.clear();
    app.hostileBullets.clear();
    app.frames = 0;
}

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
    if (app.gameOver) {
        if (keys[SDL_SCANCODE_R]) resetGame(app);
        else {
            SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
            SDL_RenderClear(app.renderer);
            drawText(app, "GAME OVER", 190, 310);
            drawText(app, "SCORE " + std::to_string(app.score), 190, 355);
            drawText(app, "PRESS R TO RESTART", 115, 415);
            SDL_RenderPresent(app.renderer);
            return;
        }
    }
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

    // Original Enemy.cpp: side-to-side bounce and 3.2 px/frame descent.
    auto& enemy = app.enemy;
    if (enemy.x + kEnemyWidth < kEnemyWidth / 2.f ||
        enemy.x + kEnemyWidth > kWidth + kEnemyWidth / 2.f) {
        enemy.vx = -enemy.vx;
    }
    if (enemy.y - kEnemyHeight > kHeight) respawnEnemy(enemy);
    enemy.x += enemy.vx;
    enemy.y += enemy.vy;

    SDL_Rect enemyRect{static_cast<int>(enemy.x), static_cast<int>(enemy.y),
                       kEnemyWidth, kEnemyHeight};
    for (auto it = app.bullets.begin(); it != app.bullets.end(); ) {
        SDL_Rect bulletRect{it->x, it->y, kBulletSource.w * kScale,
                            kBulletSource.h * kScale};
        if (intersects(bulletRect, enemyRect)) {
            it = app.bullets.erase(it);
            app.score += 100;
            respawnEnemy(enemy);
            enemyRect = {static_cast<int>(enemy.x), static_cast<int>(enemy.y),
                         kEnemyWidth, kEnemyHeight};
            std::cout << "Enemy hit! Score: " << app.score << std::endl;
        } else {
            ++it;
        }
    }

    ++app.frames;
    if (app.frames % kEnemyBSpawnFrames == 0 &&
        app.enemiesB.size() < kMaxEnemyB) {
        app.enemiesB.push_back(makeEnemyB());
    }
    if (app.frames % 120 == 0) {
        spawnAimedBullet(app.hostileBullets, enemy.x + 20, enemy.y + 54,
                         app.playerX + spriteWidth / 2.f,
                         app.playerY + spriteHeight / 2.f);
    }
    for (auto& e : app.enemiesB) {
        auto approach = [](float& value, float target) {
            if (value < target) value = std::min(value + 2.f, target);
            else if (value > target) value = std::max(value - 2.f, target);
        };
        approach(e.x, e.targetX);
        approach(e.y, e.targetY);
        if (std::abs(e.x - e.targetX) <= 15.f ||
            std::abs(e.y - e.targetY) <= 15.f) chooseTarget(e);
        if (--e.fireCooldown <= 0) {
            // EnemyB uses a downward projectile at 7 pixels per frame.
            app.hostileBullets.push_back({e.x - 50.f, e.y - 30.f, 0.f, 7.f, false});
            e.fireCooldown = kEnemyBFireFrames;
        }
    }
    for (auto& shot : app.hostileBullets) {
        shot.x += shot.vx;
        shot.y += shot.vy;
    }
    app.hostileBullets.erase(
        std::remove_if(app.hostileBullets.begin(), app.hostileBullets.end(),
            [](const HostileBullet& shot) {
                return shot.x < -40 || shot.x > kWidth + 40 ||
                       shot.y < -120 || shot.y > kHeight + 40;
            }), app.hostileBullets.end());

    // Player projectiles can also destroy Enemy B aircraft.
    for (auto e = app.enemiesB.begin(); e != app.enemiesB.end(); ) {
        SDL_Rect rect{static_cast<int>(e->x), static_cast<int>(e->y),
                      kEnemyBSize, kEnemyBSize};
        bool hit = false;
        for (auto b = app.bullets.begin(); b != app.bullets.end(); ++b) {
            SDL_Rect bulletRect{b->x, b->y, kBulletSource.w * kScale,
                                kBulletSource.h * kScale};
            if (intersects(rect, bulletRect)) {
                app.bullets.erase(b);
                app.score += 100;
                hit = true;
                break;
            }
        }
        if (hit) e = app.enemiesB.erase(e);
        else ++e;
    }

    SDL_Rect playerRect{app.playerX, app.playerY, spriteWidth, spriteHeight};
    if (app.invulnerableFrames > 0) --app.invulnerableFrames;
    bool playerHit = intersects(playerRect, enemyRect);
    for (const auto& e : app.enemiesB) {
        SDL_Rect rect{static_cast<int>(e.x), static_cast<int>(e.y),
                      kEnemyBSize, kEnemyBSize};
        if (intersects(playerRect, rect)) playerHit = true;
    }
    for (auto it = app.hostileBullets.begin(); it != app.hostileBullets.end(); ) {
        const int size = it->aimed ? 12 : kBulletSource.w * kScale;
        const int height = it->aimed ? 12 : kBulletSource.h * kScale;
        SDL_Rect rect{static_cast<int>(it->x), static_cast<int>(it->y), size, height};
        if (intersects(playerRect, rect)) {
            playerHit = true;
            it = app.hostileBullets.erase(it);
        } else ++it;
    }
    if (app.invulnerableFrames == 0 && playerHit) {
        --app.lives;
        app.playerX = (kWidth - 96) / 2 + 2;
        app.playerY = 700;
        app.invulnerableFrames = kInvulnerableFrames;
        respawnEnemy(enemy);
        if (app.lives <= 0) app.gameOver = true;
    }

    SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
    SDL_RenderClear(app.renderer);
    SDL_Rect destination{app.playerX, app.playerY, spriteWidth, spriteHeight};
    if (app.invulnerableFrames == 0 || (app.invulnerableFrames / 6) % 2 == 0)
        SDL_RenderCopy(app.renderer, app.spriteSheet, &app.source, &destination);
    for (const auto& bullet : app.bullets) {
        SDL_Rect destinationBullet{bullet.x, bullet.y,
                                  kBulletSource.w * kScale,
                                  kBulletSource.h * kScale};
        SDL_RenderCopy(app.renderer, app.projectileSheet,
                       &kBulletSource, &destinationBullet);
    }
    SDL_Rect enemyDestination{static_cast<int>(enemy.x), static_cast<int>(enemy.y),
                              kEnemyWidth, kEnemyHeight};
    SDL_RenderCopy(app.renderer, app.enemySheet, &kEnemySource, &enemyDestination);
    for (const auto& e : app.enemiesB) {
        SDL_Rect dest{static_cast<int>(e.x), static_cast<int>(e.y),
                      kEnemyBSize, kEnemyBSize};
        SDL_RenderCopy(app.renderer, app.enemySheet, &kEnemyBSource, &dest);
    }
    for (const auto& shot : app.hostileBullets) {
        SDL_Rect src = shot.aimed ? kEnemyBulletSource : kBulletSource;
        SDL_Rect dest{static_cast<int>(shot.x), static_cast<int>(shot.y),
                      src.w * kScale, src.h * kScale};
        SDL_RenderCopy(app.renderer, app.projectileSheet, &src, &dest);
    }
    drawText(app, "SCORE " + std::to_string(app.score), 10, 20);
    drawText(app, "LIVES " + std::to_string(app.lives), 190, 20);
    if (app.gameOver) drawText(app, "GAME OVER - PRESS R", 105, 370);
    SDL_RenderPresent(app.renderer);
}
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init: " << TTF_GetError() << '\n';
        SDL_Quit();
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
    app.enemySheet = IMG_LoadTexture(app.renderer, kEnemySprite);
    if (!app.enemySheet) {
        std::cerr << "Cannot load enemy sprites: " << IMG_GetError() << '\n';
        SDL_DestroyTexture(app.projectileSheet);
        SDL_DestroyTexture(app.spriteSheet);
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }
    app.font = TTF_OpenFont(kFontFile, 24);
    if (!app.font) {
        std::cerr << "Cannot load HUD font: " << TTF_GetError() << '\n';
        SDL_DestroyTexture(app.enemySheet);
        SDL_DestroyTexture(app.projectileSheet);
        SDL_DestroyTexture(app.spriteSheet);
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    emscripten_set_main_loop_arg(frame, &app, 0, 1);
    return 0;
}
