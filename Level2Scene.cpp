#include "Level2Scene.h"

Level2Scene::Level2Scene()
{

}


void Level2Scene::start()
{
// Configurar enemigos, jugador, y objetos específicos de este nivel
//  enemies.push_back(new Enemy(/* parametros */));
// Otros objetos como power-ups, plataformas, etc.
}

void Level2Scene::cmd()
{

// CMD
}

void Level2Scene::update()
{
//  for (auto *enemy: enemies) {
//    enemy->update();
//  }
// Otras lógicas del nivel, como colisiones y progreso
}


void Level2Scene::render(sf::RenderWindow &window)
{
//  for (auto *enemy: enemies) {
//    window.draw(*enemy);
//  }
  window.draw(*this);
// Dibujar otros elementos del nivel
}

sf::Vector2f Level2Scene::getCameraPosition() {}

void Level2Scene::draw(sf::RenderTarget &target, sf::RenderStates states) {
    target.draw(_bgSprite,states);
    target.draw(_gamePlay, states);
}

