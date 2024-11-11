// Game.h

#pragma once
#include <SFML/Graphics.hpp>
#include "SceneManager.h"


class Game
{
public:
    Game() : window(sf::VideoMode(600, 800), "1942")

    {
      window.setFramerateLimit(60);
      sceneManager = new SceneManager();
    }



    void run()
    {
      while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event))
        {
          if(event.type == sf::Event::Closed ||  (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)  ) {
            window.close();
          }
        }
        sceneManager->update();
        window.clear();
        sceneManager->render(window);
        window.display();

      }
    }

    ~Game()
    {
      delete sceneManager;
    }

private:
    sf::RenderWindow window;
    SceneManager *sceneManager;
};
