# Proyecto-1942 — Phase 4: Enemy and collisions

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

## Next

Add HUD, player damage, and additional enemy types.
