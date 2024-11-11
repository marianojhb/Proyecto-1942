cmake_minimum_required(VERSION 3.30)
project(Proyecto_1942 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_MakeAvailable(SFML)

add_executable(main main.cpp
#        Player.cpp
         Avion.cpp
         Avion.h
        Bullet.cpp
        Bullet.h
#        Collisionable.cpp
#        Collisionable.h
#        CompatibilidadMacos.cpp
#        CompatibilidadMacos.h
#        Creditos.cpp
#        Creditos.h
        Enemy.cpp
        Enemy.h
        EnemyB.cpp
        EnemyB.h
        EnemyBullet.cpp
        EnemyBullet.h
        Explosion.cpp
        Explosion.h
#        Fecha.cpp
#        Fecha.h
#        Game.cpp
        Game.h
#        Gameplay.cpp
        Gameplay.h
#        GamePlayOld.cpp
#        GamePlayOld.h
#        Level1Scene.cpp
#        Level1Scene.h
#        Level2Scene.cpp
        Level2Scene.h
#        Level3Scene.cpp
        Level3Scene.h
#        Menu.cpp
#        Menu.h
#        Personaje.cpp
#        Personaje.h
        Player.h
        PowerUp.cpp
        PowerUp.h
#        Ranking.cpp
#        Ranking.h
#        RankingArchivo.cpp
#        RankingArchivo.h
#        Scene.cpp
        Scene.h
#        SceneManager.cpp
        SceneManager.h
        Sounds.cpp
        Sounds.h
)

target_link_libraries(main PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
target_compile_features(main PRIVATE cxx_std_17)

# Carpeta de recursos (sprites)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()


# Copia la carpeta de recursos (sprites) al directorio de compilación
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_DIR}"              # Carpeta de origen
        "$<TARGET_FILE_DIR:main>/assets"  # Carpeta de destino en el directorio de compilación
)
