# Proyecto-1942 — Browser proof of concept

This is an **isolated technical experiment**, not a port of the original SFML game yet. It draws a movable placeholder square using C++17, SDL2 and Emscripten.

## Requirements

Install the Emscripten SDK (emsdk) and CMake. Activate the SDK in your shell so that `emcmake` and `em++` are available.

## Build

From the repository root:

```sh
emcmake cmake -S web -B web/build -DCMAKE_BUILD_TYPE=Release
cmake --build web/build -j
```

## Run locally

```sh
cd web/build
python3 -m http.server 8080
```

Open http://localhost:8080/proyecto1942.html. Use arrow keys or WASD to move the blue square. Click the canvas first if keyboard focus is needed.

## Acceptance criteria

- [ ] Browser loads generated HTML, JS and WASM without console errors
- [ ] Canvas displays at 600 × 800
- [ ] Arrow keys / WASD move the square
- [ ] Player stays inside the canvas
- [ ] No native SFML dependencies are needed for this proof of concept

## Next step

After verifying the browser toolchain, extract reusable gameplay logic from the SFML desktop project and add a platform-independent game state.

No original desktop source files have been modified.
