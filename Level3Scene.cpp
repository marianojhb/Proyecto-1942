#include "Level3Scene.h"

void Level3Scene::start()
{
// Configurar enemigos, jugador, y objetos específicos de este nivel
  enemies.push_back(new Enemy(/* parametros */));
// Otros objetos como power-ups, plataformas, etc.
}


void Level3Scene::cmd()
{

// CMD
}

void Level3Scene::update()
{
  for (auto *enemy: enemies) {
    enemy->update();
  }
// Otras lógicas del nivel, como colisiones y progreso
}

void Level3Scene::render(sf::RenderWindow &window)
{
  for (auto *enemy: enemies) {
    window.draw(*enemy);
  }
// Dibujar otros elementos del nivel
}


sf::Vector2f getCameraPosition();

void draw(sf::RenderTarget &target, sf::RenderStates states) const

override;

void cmd();

void respawnBackground();