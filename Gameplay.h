// Gameplay

#pragma once
#include <iostream>
#include "Scene.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyB.h"
#include "EnemyBullet.h"
#include "PowerUp.h"

class Gameplay {
public:

    Gameplay() {}  // Constructor privado para evitar creación de instancias fuera de getInstance()

    static Gameplay* getInstance() {
      if (!_instance) {
        _instance = new Gameplay();
      }
      return _instance;
    }

    void update(Scene* currentScene) {


        // Obtengo el player y el enemigo para luego usarlos (para chequear colisiones por ej).
      Player *player = currentScene->getPlayer();
      Enemy *enemy1 = currentScene->getEnemy();
      std::vector<Bullet*>& bullets = currentScene->getBullets();
      std::vector<EnemyBullet*>& enemyBullets = currentScene->getEnemyBullets();


      if (player->Shoot()) {
        if (bullets.size() < 15 && _timerReload <= 0) {
            _timerReload = 15;
          bullets.push_back(new Bullet(player->getBulletOrigin()));
        }
      } // PLAYER dispara

      for (int i = bullets.size() - 1; i >= 0; i--) {
        if (bullets[i]->getBounds().top + bullets[i]->getBounds().height < 0) {
          delete bullets[i];  // Eliminar la bala
          bullets.erase(bullets.begin() + i);  // Eliminar de la lista
        }
      } // BULLETS elimina balas propias fuera de pantalla

      if (_frames%120 == 0)
      {
        // Calcula dirección normalizada de EnemyBullet hacia el jugador
        sf::Vector2f direccion = player->getPosition() - enemy1->getPosition();
        float magnitud = sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
        if (magnitud != 0) {
          direccion /= magnitud;  // Normaliza el vector
        }


        enemyBullets.push_back(new EnemyBullet(enemy1->getBulletOrigin(), direccion, 3.5f));
      } // Dispara EnemyBullet hacia el jugador

      for (int i = 0; i < enemyBullets.size(); i++) {
        if (enemyBullets[i]->getBounds().top + enemyBullets[i]->getBounds().height > 800) {
          delete enemyBullets[i];
          enemyBullets.erase(enemyBullets.begin() + i);
        }
      } //  ENEMY BULLETS elimina fuera de pantalla

//        if (currentScene->getFrames() % 360 == 0) {
//          //  _powerUp = new PowerUp();
//          //  _powerUp->respawn();
//        }
//

      // Chequeo de colisiones entre Player y Enemigo1
      if (checkCollision(*player, *enemy1)) {
        Gameplay::getInstance()->changeVidas();
        player->respawn() ;
        enemy1->respawn();
      } // COLISION Player y Enemigo1

      for (int i = bullets.size() - 1; i >= 0; i--) {
        if (checkCollision2(*bullets[i], *enemy1)) {
          Gameplay::getInstance()->changePuntos(100);
          enemy1->respawn();
          delete bullets[i]; // Libera la memoria de la bala y la elimina del vector
          bullets.erase(bullets.begin() + i);
        }
      } // COLISION Bullets y Enemigo1

      for (int i = enemyBullets.size() - 1; i >= 0; i--) {
        if (checkCollision3(*player, *enemyBullets[i])) {
          player->respawn();
          Gameplay::getInstance()->changeVidas();
          delete enemyBullets[i]; // Libera la memoria de la bala y la elimina del vector
          enemyBullets.erase(enemyBullets.begin() + i);
        }
      } // COLISION Player y Enemy Bullets


      // TIMER RELOAD PARA VOLVER A DISPARAR
      if (_timerReload > 0) {
        _timerReload--;
      }

      _frames++;
    }



    int getVidas() const { return _vidas; }  // funcion obtiene vidas
    int getPuntos() const { return _puntos; } // funcion obtiene puntos
    void changeVidas() { _vidas--; } // funcion quita vidas
    void changePuntos(int puntos) { _puntos += puntos; } // funcion suma puntos



    //////////////////////// PRIVATE ///////////////////////////
private:
    bool checkCollision(const Player& player, const Enemy& enemy) const
    {
      return player.getBounds().intersects(enemy.getBounds());
    } // funcion booleana chequea colision Player-Enemigo
    bool checkCollision2(const Bullet& bullet, const Enemy& enemy) const
    {
      return bullet.getBounds().intersects(enemy.getBounds());
    } // funcion booleana chequea colision Bullets-Enemigo
    bool checkCollision3(const Player& player, const EnemyBullet& enemyBullet) const
    {
      return player.getBounds().intersects(enemyBullet.getBounds());
    } // funcion booleana chequea colision Player-Bullets

    void inicioEnemyB();
    Sounds _sound;

    std::vector<Explosion*> _vExplosiones; /// SI HAY EXPLISION
    std::vector<bool*> _vec_isExplosiveActive; /// VERIFICAR SI ESTA EN VERDADERO
    std::vector<float*> _vec_frameExplosive; /// LOS FRAMES PARA MANDERSELO A LA FUNCION

    PowerUp *_powerUp;

    sf::Clock clock;

    int _timerReload = 15;
    int _frames = 0;
    int _puntos = 0;
    int _vidas = 3;

    static Gameplay* _instance;
};

// Inicialización del puntero estático
Gameplay* Gameplay::_instance = nullptr;