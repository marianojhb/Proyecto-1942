# Proyecto-1942 — Phase 3: Player shooting

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

## Next

Add enemies and bullet/enemy collisions after verifying projectile movement.
