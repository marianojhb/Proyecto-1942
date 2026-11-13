# Proyecto-1942 — Phase 6: Enemy B and hostile projectiles

The browser prototype now renders the original player sprite sheet from `assets/sprites/1942-sprites-player.png` and original bullet sprite sheet using SDL2_image and Emscripten. The original SFML desktop code remains untouched.

## Build

From the repository root, with Emscripten activated:

```sh
emcmake cmake -S web -B web/build -DCMAKE_BUILD_TYPE=Release
cmake --build web/build -j
python3 -m http.server 8080 --directory web/build
```

Open http://localhost:8080/proyecto1942.html.

## Controls

- Arrow keys / WASD: move player
- Left/right: display corresponding original turning sprites
- Space: fire one bullet per key press (maximum five active bullets, 15-frame reload)
- R: restart after game over
- Escape: stop game loop

## Verification

- [ ] Build emits HTML, JS, WASM and a `.data` asset package
- [ ] Browser renders the original plane sprite rather than a blue square
- [ ] Plane moves and changes appearance on left/right turns
- [ ] Space fires one sprite-based projectile per key press
- [ ] Bullets travel upward, disappear offscreen, and never exceed five active shots
- [ ] Shooting respects the 15-frame reload timer
- [ ] Desktop SFML files remain unchanged

## Phase 4 verification

- Enemy sprite appears and descends while moving sideways
- Enemy bounces off the sides and respawns after leaving the screen
- Player bullets remove the enemy on contact and respawn it
- Each successful hit adds 100 points (logged to browser console)
- Desktop SFML sources remain unchanged

## Phase 5 verification

- Score and three initial lives are displayed using the original font
- Player-enemy contact removes one life and respawns the player
- Player briefly blinks while invulnerable after damage
- At zero lives, the game stops and displays GAME OVER
- R restarts the game with zero score and three lives

## Phase 6 verification

- [ ] Enemy B aircraft appear periodically (up to three active)
- [ ] Enemy B moves toward changing targets and fires downward
- [ ] Original Enemy fires aimed shots toward the player
- [ ] Enemy projectiles reduce lives on contact
- [ ] Invulnerability prevents repeated immediate damage
- [ ] Player bullets can destroy Enemy B and increase score
- [ ] Game over and R restart reset enemy projectiles and aircraft

## Next

Scrolling background, explosions, and the rest of the original gameplay systems.
