#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <emscripten/emscripten.h>
#include <emscripten/em_js.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstring>

// Read preloaded MP3 bytes from Emscripten's virtual filesystem.
EM_JS(void, playOriginalSound, (int kind), {
    try {
        var paths = [
            '/assets/sounds/sfx/pew-pew-disparo.mp3',
            '/assets/sounds/sfx/impact_explosion_03.mp3'
        ];
        var index = kind === 0 ? 0 : 1;
        Module.originalSounds = Module.originalSounds || {};
        if (!Module.originalSounds[index]) {
            var bytes = FS.readFile(paths[index]);
            var blob = new Blob([bytes], {type: 'audio/mpeg'});
            Module.originalSounds[index] = URL.createObjectURL(blob);
        }
        var audio = new Audio(Module.originalSounds[index]);
        audio.volume = index === 0 ? 0.15 : 0.10;
        audio.play().catch(function() {});
    } catch (e) { console.warn('Audio playback:', e); }
});

EM_JS(void, setThemeMusic, (int enabled), {
    try {
        Module.themeMusic = Module.themeMusic || (function() {
            var bytes = FS.readFile('/assets/sounds/themes/01_main_theme_v1_stereo.mp3');
            var audio = new Audio(URL.createObjectURL(new Blob([bytes], {type: 'audio/mpeg'})));
            audio.loop = true;
            audio.volume = 0.28;
            return audio;
        })();
        if (enabled) Module.themeMusic.play().catch(function() {});
        else Module.themeMusic.pause();
    } catch (e) { console.warn('Theme music:', e); }
});


EM_JS(void, setRankingMusic, (int enabled), {
    try {
        if (!Module.rankingMusic) {
            var bytes = FS.readFile('/assets/sounds/themes/04_high_score_entry_stereo.mp3');
            var audio = new Audio(URL.createObjectURL(new Blob([bytes], {type: 'audio/mpeg'})));
            audio.loop = true;
            audio.volume = 0.28;
            Module.rankingMusic = audio;
        }
        if (enabled) {
            if (Module.themeMusic) Module.themeMusic.pause();
            Module.rankingMusic.play().catch(function() {});
        } else {
            Module.rankingMusic.pause();
            Module.rankingMusic.currentTime = 0;
        }
    } catch (e) { console.warn('Ranking music:', e); }
});


EM_JS(void, setMenuMusic, (int enabled), {
    try {
        if (!Module.menuMusic) {
            var bytes = FS.readFile('/assets/sounds/themes/02_main_theme_v2_stereo.mp3');
            var audio = new Audio(URL.createObjectURL(new Blob([bytes], {type: 'audio/mpeg'})));
            audio.loop = true;
            audio.volume = 0.20;
            Module.menuMusic = audio;
        }
        if (enabled) {
            if (Module.themeMusic) Module.themeMusic.pause();
            if (Module.rankingMusic) Module.rankingMusic.pause();
            Module.menuMusic.play().catch(function() {});
        } else Module.menuMusic.pause();
    } catch (e) { console.warn('Menu music:', e); }
});

EM_JS(int, readPreference, (const char* name, int fallback), {
    try {
        var value = localStorage.getItem(UTF8ToString(name));
        return value === null ? fallback : (value === '1' ? 1 : 0);
    } catch(e) { return fallback; }
});
EM_JS(void, writePreference, (const char* name, int enabled), {
    try { localStorage.setItem(UTF8ToString(name), enabled ? '1' : '0'); } catch(e) {}
});
EM_JS(char*, loadScores, (), {
    var data = '';
    try { data = localStorage.getItem('proyecto1942.ranking.v1') || ''; } catch(e) {}
    return stringToNewUTF8(data);
});
EM_JS(void, saveScores, (const char* value), {
    try { localStorage.setItem('proyecto1942.ranking.v1', UTF8ToString(value)); } catch(e) {}
});

namespace {
constexpr int kWidth = 600;
constexpr int kHeight = 800;
constexpr int kSpeed = 5;
constexpr int kScale = 3;
constexpr const char* kPlayerSprite = "assets/sprites/1942-sprites-player.png";
constexpr const char* kProjectileSprite = "assets/sprites/1942-sprites-projectiles.png";
constexpr const char* kEnemySprite = "assets/sprites/1942-sprites-enemy.png";
constexpr const char* kFontFile = "assets/fonts/MONOCOQUE_FUENTE.ttf";
constexpr const char* kBackgroundFile = "assets/sprites/bg-maps-1942.png";
constexpr const char* kEffectsFile = "assets/sprites/1942-sprites-effects.png";
constexpr const char* kBossFile = "assets/sprites/1942-sprites-enemy-copia.png";
constexpr SDL_Rect kBossSource{68, 468, 65, 53};
constexpr int kBossWidth = 260;
constexpr int kBossHeight = 212;
constexpr int kBossStartFrame = 60 * 20;
constexpr int kBossFireStartFrame = 60 * 28;
constexpr int kPowerUpInterval = 60 * 14;
constexpr int kFirstPowerUpFrame = 60 * 5;
constexpr int kBossHealth = 50;
constexpr int kBossDeathFrames = 90;
constexpr int kBossBulletCap = 9;
constexpr float kScrollSpeed = 2.3f;
constexpr int kExplosionFrames = 40;
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
struct ExplosionEffect { int x, y, age = 0, scale = 2; };
struct Enemy { float x = 300.f, y = -96.f, vx = 1.5f, vy = 3.2f; };
struct EnemyB {
    float x = 0, y = -96, targetX = 240, targetY = 180;
    int fireCooldown = kEnemyBFireFrames;
};
struct HostileBullet { float x, y, vx, vy; bool aimed; };
struct PowerUp { float x = 0, y = 0; int variant = 0; bool active = false; };
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
    SDL_Texture* background = nullptr;
    SDL_Texture* effects = nullptr;
    SDL_Texture* bossSheet = nullptr;
    SDL_Texture* menuLogo = nullptr;
    SDL_Texture* menuPlane = nullptr;
    SDL_Texture* capcomLogo = nullptr;
    SDL_Texture* rankingBackground = nullptr;
    TTF_Font* menuFont = nullptr;
    TTF_Font* hudReadableFont = nullptr;
    bool inMenu = true;
    bool confirmExit = false;
    bool exitKeyHeld = false;
    bool scoreRecorded = false;
    float rankingScroll = 0.f;
    float creditsScroll = 0.f;
    struct ScoreEntry { int score; std::string date; std::string name; };
    std::vector<ScoreEntry> ranking;
    bool menuKeyHeld = false;
    bool menuOptions = false;
    int menuSelection = 0;
    int subSelection = 0;
    int menuPage = 0; // 0 main, 1 settings, 2 ranking, 3 credits
    bool musicEnabled = true;
    int bestScore = 0;
    bool soundEnabled = true;
    bool mutedKey = false;
    bool powerUpTestKey = false;
    bool bossActive = false;
    bool victory = false;
    bool bossDying = false;
    int bossDeathTimer = 0;
    PowerUp powerUp;
    int bossHp = kBossHealth;
    float bossY = -kBossHeight;
    int bossCooldown = 0;
    int backgroundHeight = 0;
    float scrollY = 0.f;
    std::vector<ExplosionEffect> explosions;
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

void drawText(App& app, const std::string& message, int x, int y, int size = 24, SDL_Color color = {255, 255, 255, 255}) {
    TTF_Font* font = app.inMenu ? (app.menuFont ? app.menuFont : app.font)
                                : (app.hudReadableFont ? app.hudReadableFont : app.font);
    if (!font) return;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, message.c_str(), color);
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


void loadRanking(App& app) {
    char* raw = loadScores();
    if (!raw) return;
    std::istringstream lines(raw);
    std::string line;
    while (std::getline(lines, line) && app.ranking.size() < 10) {
        std::istringstream fields(line);
        std::string score, date, name;
        if (std::getline(fields, score, '|') && std::getline(fields, date, '|') &&
            std::getline(fields, name) && !name.empty()) {
            char* end = nullptr;
            const long points = std::strtol(score.c_str(), &end, 10);
            if (end != score.c_str() && *end == 0 && points >= 0 && points <= 2147483647L)
                app.ranking.push_back({static_cast<int>(points), date, name});
        }
    }
    free(raw);
    std::sort(app.ranking.begin(), app.ranking.end(), [](const auto& a, const auto& b) {
        return a.score > b.score;
    });
}
void recordScore(App& app) {
    if (app.scoreRecorded) return;
    app.scoreRecorded = true;
    app.bestScore = std::max(app.bestScore, app.score);
    if (app.score <= 0) return;
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char date[16] = "----/--/--";
    if (local) std::strftime(date, sizeof(date), "%Y/%m/%d", local);
    app.ranking.push_back({app.score, date, "PLAYER"});
    std::stable_sort(app.ranking.begin(), app.ranking.end(), [](const auto& a, const auto& b) {
        return a.score > b.score;
    });
    if (app.ranking.size() > 10) app.ranking.resize(10);
    std::ostringstream output;
    for (const auto& entry : app.ranking)
        output << entry.score << "|" << entry.date << "|" << entry.name << "\n";
    saveScores(output.str().c_str());
}
void drawMenuPlane(App& app, int y) {
    if (app.menuPlane) {
        SDL_Rect plane{40, y, 95, 55};
        SDL_RenderCopy(app.renderer, app.menuPlane, nullptr, &plane);
    } else drawText(app, ">", 115, y);
}
void drawSettingCircle(App& app, int y, bool enabled) {
    SDL_SetRenderDrawColor(app.renderer, enabled ? 0 : 220, enabled ? 220 : 0, 0, 255);
    SDL_Rect rect{500, y + 8, 20, 20};
    SDL_RenderFillRect(app.renderer, &rect);
}

void resetGame(App& app) {
    app.scoreRecorded = false;
    app.confirmExit = false;
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
    app.scrollY = 0.f;
    app.explosions.clear();
    app.bossActive = false;
    app.victory = false;
    app.bossHp = kBossHealth;
    app.bossY = -kBossHeight;
    app.bossCooldown = 0;
    app.bossDying = false;
    app.bossDeathTimer = 0;
    app.powerUp = PowerUp{};
    app.powerUpTestKey = false;
}

void frame(void* context) {
    auto& app = *static_cast<App*>(context);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) app.running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE &&
            !event.key.repeat && !app.inMenu && !app.gameOver && !app.victory)
            app.confirmExit = !app.confirmExit;
    }
    if (!app.running) {
        emscripten_cancel_main_loop();
        return;
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (app.inMenu) {
        const bool up = keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W];
        const bool down = keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S];
        const bool back = keys[SDL_SCANCODE_ESCAPE];
        if (!up && !down && !back && !keys[SDL_SCANCODE_RETURN] && !keys[SDL_SCANCODE_SPACE])
            app.menuKeyHeld = false;
        if (!app.menuKeyHeld) {
            if (back && app.menuPage != 0) { if (app.menuPage == 2) setRankingMusic(0); app.menuPage = 0; if (app.musicEnabled) setMenuMusic(1); app.menuKeyHeld = true; }
            else if (up || down) {
                int& selection = app.menuPage == 1 ? app.subSelection : app.menuSelection;
                int count = app.menuPage == 1 ? 3 : 5;
                if (app.menuPage <= 1) selection = (selection + (up ? count - 1 : 1)) % count;
                app.menuKeyHeld = true;
            }
        }
        const bool start = keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_SPACE];
        if (start && !app.menuKeyHeld) {
            app.menuKeyHeld = true;
            if (!app.menuOptions && app.menuPage == 0) { app.menuOptions = true; if (app.musicEnabled) setMenuMusic(1); }
            else if (app.menuPage == 0) {
                if (app.menuSelection == 0) {
                    setMenuMusic(0);
                    app.inMenu = false;
                    resetGame(app);
                    if (app.musicEnabled) setThemeMusic(1);
                } else if (app.menuSelection == 4) {
                    app.menuOptions = false;
                    app.menuSelection = 0;
                    setMenuMusic(0);
                } else {
                    app.menuPage = app.menuSelection;
                    app.subSelection = 0;
                    if (app.menuPage == 2) { app.rankingScroll = 0.f; setMenuMusic(0); if (app.musicEnabled) setRankingMusic(1); }
                    if (app.menuPage == 3) app.creditsScroll = 0.f;
                }
            } else if (app.menuPage == 1) {
                if (app.subSelection == 0) { app.soundEnabled = !app.soundEnabled; writePreference("proyecto1942.sound", app.soundEnabled); }
                if (app.subSelection == 1) {
                    app.musicEnabled = !app.musicEnabled;
                    writePreference("proyecto1942.music", app.musicEnabled);
                    setMenuMusic(app.musicEnabled ? 1 : 0);
                }
                if (app.subSelection == 2) app.menuPage = 0;
            } else { if (app.menuPage == 2) setRankingMusic(0); app.menuPage = 0; if (app.musicEnabled) setMenuMusic(1); }
        }
        if (app.inMenu) {
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
            SDL_RenderClear(app.renderer);
            if (app.menuPage == 0 && app.menuLogo) {
                int w = 0, h = 0;
                SDL_QueryTexture(app.menuLogo, nullptr, nullptr, &w, &h);
                if (w > 0 && h > 0) {
                    const float scale = std::min(2.f, std::min(440.f / w, 135.f / h));
                    SDL_Rect logo{(kWidth - static_cast<int>(w * scale)) / 2, 145,
                                  static_cast<int>(w * scale), static_cast<int>(h * scale)};
                    SDL_RenderCopy(app.renderer, app.menuLogo, nullptr, &logo);
                }
            }
            if (app.menuPage == 0) {
                if (app.capcomLogo) {
                    int cw = 0, ch = 0;
                    SDL_QueryTexture(app.capcomLogo, nullptr, nullptr, &cw, &ch);
                    if (cw > 0 && ch > 0) {
                        const float cs = std::min(150.f / cw, 65.f / ch);
                        SDL_Rect cap{(kWidth - static_cast<int>(cw * cs)) / 2, 660,
                                     static_cast<int>(cw * cs), static_cast<int>(ch * cs)};
                        SDL_RenderCopy(app.renderer, app.capcomLogo, nullptr, &cap);
                    }
                }
                drawText(app, "@1984 CAPCOM", 220, 730, 24, SDL_Color{235, 235, 235, 255});
            }
            if (app.menuPage == 0 && !app.menuOptions) {
                if ((SDL_GetTicks() / 500) % 2 == 0)
                    drawText(app, "INSERT COIN", 195, 450, 24, SDL_Color{255, 255, 0, 255});
                drawText(app, "PRESS ENTER", 190, 540);
            } else if (app.menuPage == 0) {
                const char* items[] = {"JUGAR", "CONFIGURACION", "RANKING", "CREDITOS", "SALIR"};
                for (int i = 0; i < 5; ++i) {
                    drawText(app, items[i], 175, 350 + i * 50, 24,
                             i == app.menuSelection ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 255, 255});
                }
                drawMenuPlane(app, 350 + app.menuSelection * 50);
            } else if (app.menuPage == 1) {
                const char* options[] = {"SOUNDFX", "MUSICA", "VOLVER"};
                for (int i = 0; i < 3; ++i)
                    drawText(app, options[i], i == 0 ? 170 : 195, 350 + i * 50, 24,
                             i == app.subSelection ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 255, 255});
                drawMenuPlane(app, 350 + app.subSelection * 50);
                drawSettingCircle(app, 350, app.soundEnabled);
                drawSettingCircle(app, 400, app.musicEnabled);
            } else if (app.menuPage == 2) {
                if (app.rankingBackground) {
                    SDL_Rect bg{0, 0, kWidth, kHeight};
                    SDL_RenderCopy(app.renderer, app.rankingBackground, nullptr, &bg);
                }
                const int startY = kHeight - static_cast<int>(app.rankingScroll);
                const SDL_Color magenta{255, 0, 255, 255};
                drawText(app, "POS  SCORE    DATE       NAME", 45, startY, 24, magenta);
                for (size_t i = 0; i < 10; ++i) {
                    const int y = startY + static_cast<int>(i + 1) * 48;
                    if (y < -40 || y > kHeight) continue;
                    std::string line = std::to_string(i + 1) + "  ";
                    if (i < app.ranking.size()) {
                        const auto& entry = app.ranking[i];
                        line += std::to_string(entry.score) + "  " + entry.date + "  " + entry.name;
                    } else line += "---";
                    drawText(app, line, 45, y, 24, SDL_Color{255, 255, 255, 255});
                }
                app.rankingScroll += 0.4f;
                if (startY + 11 * 48 < 0) app.rankingScroll = 0.f;
            } else {
                const char* credits[] = {"Desarrolado por:", "Carlos Zubilete Sanchez",
                                         "Mariano Belgrano", "Enzo Segovia", "Nicolas Esteban Sosa"};
                for (int i = 0; i < 5; ++i) {
                    const int y = -250 + i * 50 + static_cast<int>(app.creditsScroll);
                    if (y > -40 && y < kHeight)
                        drawText(app, credits[i], 10, y);
                }
                app.creditsScroll += 0.6f;
                if (app.creditsScroll > 1100.f) app.creditsScroll = 0.f;
            }
            SDL_RenderPresent(app.renderer);
            return;
        }
    }
    if (!app.inMenu && app.confirmExit) {
        const bool yes = keys[SDL_SCANCODE_Y];
        const bool no = keys[SDL_SCANCODE_N] || keys[SDL_SCANCODE_ESCAPE];
        if (!yes && !no) app.exitKeyHeld = false;
        if (!app.exitKeyHeld && (yes || no)) {
            app.exitKeyHeld = true;
            app.confirmExit = false;
            if (yes) {
                app.inMenu = true;
                app.menuPage = 0;
                app.menuOptions = true;
                app.menuKeyHeld = true;
                setThemeMusic(0);
                if (app.musicEnabled) setMenuMusic(1);
            }
        }
        if (app.confirmExit) {
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
            SDL_RenderClear(app.renderer);
            drawText(app, "SALIR  Y / N", 170, 370);
            drawText(app, "Y: SI    N: NO", 145, 420);
            SDL_RenderPresent(app.renderer);
        }
        return;
    }
    if (app.gameOver || app.victory) {
        recordScore(app);
        if (keys[SDL_SCANCODE_ESCAPE]) {
            app.inMenu = true;
            app.menuPage = 0;
            app.menuOptions = true;
            app.menuKeyHeld = true;
            setThemeMusic(0);
            if (app.musicEnabled) setMenuMusic(1);
            return;
        }
        if (keys[SDL_SCANCODE_R]) {
            resetGame(app);
            if (app.musicEnabled) setThemeMusic(1);
        }
        else {
            SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
            SDL_RenderClear(app.renderer);
            drawText(app, app.victory ? "MISSION COMPLETE" : "GAME OVER", 145, 310);
            drawText(app, "SCORE " + std::to_string(app.score), 190, 355);
            drawText(app, "PRESS R TO RESTART", 115, 415);
            SDL_RenderPresent(app.renderer);
            return;
        }
    }
    const bool mute = keys[SDL_SCANCODE_M];
    if (mute && !app.mutedKey) {
        app.soundEnabled = !app.soundEnabled;
        setThemeMusic(app.musicEnabled ? 1 : 0);
    }
    app.mutedKey = mute;
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
        if (app.soundEnabled) playOriginalSound(0);
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
            if (app.soundEnabled) playOriginalSound(1);
            app.explosions.push_back({static_cast<int>(enemy.x), static_cast<int>(enemy.y), 0, 2});
            respawnEnemy(enemy);
            enemyRect = {static_cast<int>(enemy.x), static_cast<int>(enemy.y),
                         kEnemyWidth, kEnemyHeight};
            std::cout << "Enemy hit! Score: " << app.score << std::endl;
        } else {
            ++it;
        }
    }

    ++app.frames;
    // First pickup appears before the boss encounter; subsequent pickups every 14 seconds.
    const bool testPowerUp = keys[SDL_SCANCODE_P];
    const bool spawnPowerUp = app.frames == kFirstPowerUpFrame ||
        (app.frames > kFirstPowerUpFrame &&
         (app.frames - kFirstPowerUpFrame) % kPowerUpInterval == 0) ||
        (testPowerUp && !app.powerUpTestKey);
    app.powerUpTestKey = testPowerUp;
    if (spawnPowerUp) {
        // Spawn inside the visible playfield to make the collectible easy to spot.
        app.powerUp = {randomRange(70, 470), 75.f, std::rand() % 8, true};
        std::cout << "Power-up spawned at frame " << app.frames << std::endl;
    }
    if (app.powerUp.active) {
        app.powerUp.y += kScrollSpeed;
        if (app.powerUp.y > kHeight + 45) app.powerUp.active = false;
    }
    if (!app.bossActive && !app.bossDying && app.frames % kEnemyBSpawnFrames == 0 &&
        app.enemiesB.size() < kMaxEnemyB) {
        app.enemiesB.push_back(makeEnemyB());
    }
    if (app.frames < kBossFireStartFrame && app.frames % 120 == 0) {
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
        if (app.frames < kBossFireStartFrame && --e.fireCooldown <= 0) {
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
                if (app.soundEnabled) playOriginalSound(1);
                app.explosions.push_back({static_cast<int>(e->x), static_cast<int>(e->y), 0, 2});
                hit = true;
                break;
            }
        }
        if (hit) e = app.enemiesB.erase(e);
        else ++e;
    }

    // Simplified level progression: boss encounter after earning enough points.
    if (!app.bossActive && !app.bossDying && !app.victory && app.frames >= kBossStartFrame &&
        app.frames == kBossStartFrame) {
        app.bossActive = true;
        app.enemiesB.clear();
        app.hostileBullets.clear();
        app.bossY = static_cast<float>(kHeight + 50);
        app.bossHp = kBossHealth;
        app.bossCooldown = 180;
    }
    if (app.bossActive) {
        app.bossY = std::max(0.f, app.bossY - 2.2f);
        // Original GamePlay.cpp: nine projectiles in a 30..150 degree fan.
        // Wait until the previous volley has cleared before launching another.
        if (app.frames >= kBossFireStartFrame && app.bossY <= 0.f &&
            --app.bossCooldown <= 0 && app.hostileBullets.empty()) {
            constexpr float kPi = 3.14159265358979323846f;
            for (int degrees = 30; degrees <= 150; degrees += 15) {
                const float radians = degrees * kPi / 180.f;
                app.hostileBullets.push_back({
                    170.f + 115.f, app.bossY + 170.f,
                    std::cos(radians) * 5.f, std::sin(radians) * 5.f, true
                });
            }
            app.bossCooldown = 180;
        }
        SDL_Rect bossRect{170, static_cast<int>(app.bossY), kBossWidth, kBossHeight};
        for (auto b = app.bullets.begin(); b != app.bullets.end();) {
            SDL_Rect bulletRect{b->x, b->y, kBulletSource.w * kScale,
                                kBulletSource.h * kScale};
            if (intersects(bossRect, bulletRect)) {
                b = app.bullets.erase(b);
                --app.bossHp;
                if (app.soundEnabled) playOriginalSound(1);
                if (app.bossHp <= 0) {
                    app.bossActive = false;
                    app.bossDying = true;
                    app.bossDeathTimer = kBossDeathFrames;
                    app.score += 1000;
                    app.hostileBullets.clear();
                    app.enemiesB.clear();
                    app.explosions.push_back({220, static_cast<int>(app.bossY) + 60, 0, 3});
                    if (app.soundEnabled) playOriginalSound(1);
                    break;
                }
            } else ++b;
        }
    }

    if (app.bossDying) {
        if (app.bossDeathTimer % 12 == 0) {
            app.explosions.push_back({static_cast<int>(randomRange(170, 365)),
                static_cast<int>(app.bossY + randomRange(20, 140)), 0, 3});
            if (app.soundEnabled) playOriginalSound(1);
        }
        if (--app.bossDeathTimer <= 0) {
            app.bossDying = false;
            app.victory = true;
        }
    }

    SDL_Rect playerRect{app.playerX, app.playerY, spriteWidth, spriteHeight};
    if (app.powerUp.active) {
        SDL_Rect powerRect{static_cast<int>(app.powerUp.x),
                           static_cast<int>(app.powerUp.y), 45, 45};
        if (intersects(playerRect, powerRect)) {
            ++app.lives;
            app.powerUp.active = false;
            if (app.soundEnabled) playOriginalSound(1);
        }
    }
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
        app.explosions.push_back({app.playerX, app.playerY, 0, 3});
        --app.lives;
        if (app.soundEnabled) playOriginalSound(2);
        app.playerX = (kWidth - 96) / 2 + 2;
        app.playerY = 700;
        app.invulnerableFrames = kInvulnerableFrames;
        respawnEnemy(enemy);
        if (app.lives <= 0) app.gameOver = true;
    }

    // Advance the map toward smaller source Y so the visible terrain moves downward.
    app.scrollY -= kScrollSpeed;
    if (app.backgroundHeight > 0 && app.scrollY < 0.f)
        app.scrollY += app.backgroundHeight;
    for (auto& explosion : app.explosions) ++explosion.age;
    app.explosions.erase(
        std::remove_if(app.explosions.begin(), app.explosions.end(),
                       [](const ExplosionEffect& e) { return e.age >= kExplosionFrames; }),
        app.explosions.end());

    SDL_SetRenderDrawColor(app.renderer, 8, 17, 36, 255);
    SDL_RenderClear(app.renderer);
    if (app.background && app.backgroundHeight > 0) {
        // Wrap the original tall map vertically; draw visible strips only.
        int remaining = kHeight;
        int destinationY = 0;
        int sourceY = (app.backgroundHeight - kHeight +
                       static_cast<int>(app.scrollY)) % app.backgroundHeight;
        while (remaining > 0) {
            int stripHeight = std::min(remaining, app.backgroundHeight - sourceY);
            SDL_Rect sourceBg{0, sourceY, kWidth, stripHeight};
            SDL_Rect destBg{0, destinationY, kWidth, stripHeight};
            SDL_RenderCopy(app.renderer, app.background, &sourceBg, &destBg);
            remaining -= stripHeight;
            destinationY += stripHeight;
            sourceY = 0;
        }
    }
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
    if (app.frames < kBossFireStartFrame)
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
    if (app.powerUp.active) {
        SDL_Rect src{106 + (app.powerUp.variant == 7 ? 238 - 106 :
                            app.powerUp.variant == 6 ? 220 - 106 :
                            app.powerUp.variant * 19), 11, 15, 15};
        SDL_Rect dest{static_cast<int>(app.powerUp.x),
                      static_cast<int>(app.powerUp.y), 45, 45};
        SDL_RenderCopy(app.renderer, app.projectileSheet, &src, &dest);
        drawText(app, "+1 LIFE", std::max(0, dest.x - 18), std::max(50, dest.y - 30));
    }
    if (app.bossActive || app.bossDying) {
        SDL_Rect bossDest{170, static_cast<int>(app.bossY), kBossWidth, kBossHeight};
        if (!app.bossDying || (app.bossDeathTimer / 5) % 2 == 0) {
            SDL_RenderCopy(app.renderer, app.bossSheet, &kBossSource, &bossDest);
        }
        drawText(app, "BOSS HP " + std::to_string(app.bossHp), 350, 20);
        SDL_Rect bar{340, 52, 245 * std::max(0, app.bossHp) / kBossHealth, 10};
        SDL_SetRenderDrawColor(app.renderer, 230, 65, 65, 255);
        SDL_RenderFillRect(app.renderer, &bar);
    }
    for (const auto& effect : app.explosions) {
        // Original Explosion.cpp advances 0.2 per update and switches every 1.5.
        const int frameIndex = std::min(5, effect.age / 8);
        SDL_Rect src{38 * frameIndex, 110, 33, 32};
        SDL_Rect dest{effect.x, effect.y, src.w * effect.scale, src.h * effect.scale};
        SDL_RenderCopy(app.renderer, app.effects, &src, &dest);
    }
    drawText(app, "SCORE " + std::to_string(app.score), 10, 20);
    drawText(app, "TIME " + std::to_string(app.frames / 60), 350, 55);
    drawText(app, "POWER-UP: P TO TEST", 10, 735);
    drawText(app, "LIVES " + std::to_string(app.lives), 10, 55);
    drawText(app, app.soundEnabled ? "SOUND ON (M)" : "SOUND OFF (M)", 10, 765);
    if (app.victory) drawText(app, "MISSION COMPLETE - PRESS R", 80, 370);
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
    static App app;
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
        std::cerr << "Cannot load projectile sprites: " << IMG_GetError() << std::endl;
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
    app.background = IMG_LoadTexture(app.renderer, kBackgroundFile);
    app.effects = IMG_LoadTexture(app.renderer, kEffectsFile);
    int bgWidth = 0;
    if (!app.background || !app.effects ||
        SDL_QueryTexture(app.background, nullptr, nullptr, &bgWidth,
                         &app.backgroundHeight) != 0 ||
        bgWidth < kWidth || app.backgroundHeight < kHeight) {
        std::cerr << "Cannot load valid background/effects textures: "
                  << IMG_GetError() << '\n';
        return 1;
    }
    app.bossSheet = IMG_LoadTexture(app.renderer, kBossFile);
    if (!app.bossSheet) {
        std::cerr << "Cannot load boss sprite: " << IMG_GetError() << '\n';
        return 1;
    }
    app.menuLogo = IMG_LoadTexture(app.renderer, "assets/sprites/logo.png");
    app.menuPlane = IMG_LoadTexture(app.renderer, "assets/sprites/Personaje.png");
    app.capcomLogo = IMG_LoadTexture(app.renderer, "assets/sprites/Capcom.jfif");
    app.rankingBackground = IMG_LoadTexture(app.renderer, "assets/sprites/1942_clock_3x4.png");
    app.menuFont = TTF_OpenFont("assets/fonts/fuente.ttf", 24);
    app.hudReadableFont = TTF_OpenFont("assets/fonts/fuente.ttf", 24);
    app.musicEnabled = readPreference("proyecto1942.music", 1) != 0;
    app.soundEnabled = readPreference("proyecto1942.sound", 1) != 0;
    loadRanking(app);
    if (!app.menuLogo)
        std::cerr << "Menu logo missing: " << IMG_GetError() << '\n';
    emscripten_set_main_loop_arg(frame, &app, 0, 1);
    return 0;
}
