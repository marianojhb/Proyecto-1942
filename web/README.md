# Proyecto-1942 — Phase 2: Player sprite

The browser prototype now renders the original player sprite sheet from `assets/sprites/1942-sprites-player.png` using SDL2_image and Emscripten. The original SFML desktop code remains untouched.

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
- Space: one-shot input is logged in browser console (no projectiles yet)
- Escape: stop game loop

## Verification

- [ ] Build emits HTML, JS, WASM and a `.data` asset package
- [ ] Browser renders the original plane sprite rather than a blue square
- [ ] Plane moves and changes appearance on left/right turns
- [ ] Space logs only once per press (check browser console)
- [ ] Desktop SFML files remain unchanged

## Next

Port the bullet and projectile logic after verifying player rendering and input.
