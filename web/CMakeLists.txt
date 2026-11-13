cmake_minimum_required(VERSION 3.20)
project(Proyecto1942WebPOC LANGUAGES CXX)

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "This build requires Emscripten. Configure with emcmake cmake.")
endif()

set(PLAYER_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/1942-sprites-player.png")
if(NOT EXISTS "${PLAYER_SPRITE}")
  message(FATAL_ERROR "Player sprite not found: ${PLAYER_SPRITE}")
endif()

set(PROJECTILE_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/1942-sprites-projectiles.png")
if(NOT EXISTS "${PROJECTILE_SPRITE}")
  message(FATAL_ERROR "Projectile sprite not found: ${PROJECTILE_SPRITE}")
endif()

set(ENEMY_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/1942-sprites-enemy.png")
if(NOT EXISTS "${ENEMY_SPRITE}")
  message(FATAL_ERROR "Enemy sprite not found: ${ENEMY_SPRITE}")
endif()

add_executable(proyecto1942 main.cpp)
target_compile_features(proyecto1942 PRIVATE cxx_std_17)
set_target_properties(proyecto1942 PROPERTIES SUFFIX ".html")
target_compile_options(proyecto1942 PRIVATE -Wall -Wextra -Wpedantic -sUSE_SDL=2 -sUSE_SDL_IMAGE=2)
target_link_options(proyecto1942 PRIVATE
  -sUSE_SDL=2
  -sUSE_SDL_IMAGE=2
  -sSDL2_IMAGE_FORMATS=['png']
  -sALLOW_MEMORY_GROWTH=1
  -sASSERTIONS=1
  -sEXIT_RUNTIME=0
  "SHELL:--preload-file ${PLAYER_SPRITE}@assets/sprites/1942-sprites-player.png"
  "SHELL:--preload-file ${PROJECTILE_SPRITE}@assets/sprites/1942-sprites-projectiles.png"
  "SHELL:--preload-file ${ENEMY_SPRITE}@assets/sprites/1942-sprites-enemy.png"
)
