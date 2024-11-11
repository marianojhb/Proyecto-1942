//SceneManager.h

#pragma once
#include "Gameplay.h"
#include "Level2Scene.h"

//class Level2Scene;

// Declaración anticipada de Gameplay (forward declaration), porque si se incluye Gameplay.h (#include "Gameplay.h"
//    se genera una referencia circular.

class Scene;
class Gameplay;
class Player;
class Enemy;

class SceneManager
{
public:
    SceneManager()
    {
//   Iniciar en el menú principal
      _gameplay = new Gameplay();
      currentScene = new Level2Scene();
      currentScene->start();
    }


    void changeScene(Scene *newScene)
    {
      delete currentScene;  // Libera la escena anterior
      currentScene = newScene;
      if (currentScene) {
        currentScene->start();
      }
    }

    void update()
    {
      if (currentScene) {
       currentScene->update();
      }
      _gameplay->update(currentScene);
    }

    void render(sf::RenderWindow &window)
    {
      if (currentScene) {
        currentScene->render(window);
      }
    }

    ~SceneManager()
    {
      delete currentScene;
      delete _gameplay;  // Libera `_gameplay` al destruir `SceneManager`
    }

private:
    Scene *currentScene = nullptr;
    Gameplay *_gameplay = nullptr;  // Declara `_gameplay` como puntero privado

};