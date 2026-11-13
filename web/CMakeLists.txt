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

set(HUD_FONT "${CMAKE_CURRENT_SOURCE_DIR}/../assets/fonts/MONOCOQUE_FUENTE.ttf")
if(NOT EXISTS "${HUD_FONT}")
  message(FATAL_ERROR "HUD font not found: ${HUD_FONT}")
endif()

set(BACKGROUND_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/bg-maps-1942.png")
set(EFFECTS_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/1942-sprites-effects.png")
foreach(ASSET IN ITEMS BACKGROUND_SPRITE EFFECTS_SPRITE)
  if(NOT EXISTS "${${ASSET}}")
    message(FATAL_ERROR "Missing asset: ${${ASSET}}")
  endif()
endforeach()

set(BOSS_SPRITE "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sprites/1942-sprites-enemy-copia.png")
if(NOT EXISTS "${BOSS_SPRITE}")
  message(FATAL_ERROR "Boss sprite not found: ${BOSS_SPRITE}")
endif()

set(SFX_SHOT "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/sfx/pew-pew-disparo.mp3")
set(SFX_EXPLOSION "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/sfx/impact_explosion_03.mp3")
foreach(ASSET IN ITEMS SFX_SHOT SFX_EXPLOSION)
  if(NOT EXISTS "${${ASSET}}")
    message(FATAL_ERROR "Missing original audio: ${${ASSET}}")
  endif()
endforeach()

add_executable(proyecto1942 main.cpp)
target_compile_features(proyecto1942 PRIVATE cxx_std_17)
set_target_properties(proyecto1942 PROPERTIES SUFFIX ".html")
target_compile_options(proyecto1942 PRIVATE -Wall -Wextra -Wpedantic -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2)
target_link_options(proyecto1942 PRIVATE
  -sUSE_SDL=2
  -sUSE_SDL_IMAGE=2
  -sUSE_SDL_TTF=2
  -sSDL2_IMAGE_FORMATS=['png']
  -sALLOW_MEMORY_GROWTH=1
  -sASSERTIONS=1
  -sEXIT_RUNTIME=0
  "SHELL:--preload-file ${PLAYER_SPRITE}@assets/sprites/1942-sprites-player.png"
  "SHELL:--preload-file ${PROJECTILE_SPRITE}@assets/sprites/1942-sprites-projectiles.png"
  "SHELL:--preload-file ${ENEMY_SPRITE}@assets/sprites/1942-sprites-enemy.png"
  "SHELL:--preload-file ${HUD_FONT}@assets/fonts/MONOCOQUE_FUENTE.ttf"
  "SHELL:--preload-file ${BACKGROUND_SPRITE}@assets/sprites/bg-maps-1942.png"
  "SHELL:--preload-file ${EFFECTS_SPRITE}@assets/sprites/1942-sprites-effects.png"
  "SHELL:--preload-file ${BOSS_SPRITE}@assets/sprites/1942-sprites-enemy-copia.png"
  "SHELL:--preload-file ${SFX_SHOT}@assets/sounds/sfx/pew-pew-disparo.mp3"
  "SHELL:--preload-file ${SFX_EXPLOSION}@assets/sounds/sfx/impact_explosion_03.mp3"
)
