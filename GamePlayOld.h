#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Avion.h"
#include "Player.h"
#include "Personaje.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Explosion.h"
#include "EnemyB.h"
#include "Sounds.h"
#include "PowerUp.h"

class GamePlayOld: public sf::Drawable
{
    public:
        GamePlayOld();
        void cmd();
        void update();
        bool isCollisionWithPersonaje();
        bool isCollisionWithEnemy();
        void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
        int getPuntos() const;
        int getVidas() const;
        bool checkCollision(const Enemy& enemigo1) const;
        int getFrames() const;
        bool isCollision_vBullestEnemyB_whitPersonaje();
        bool isCollision_bullets_whitEnemyB();
        ~GamePlayOld() override;

private:
        void inicioEnemyB();

        Personaje _player;
        std::vector<Bullet*> _bullets;
        std::vector<EnemyBullet*> _enemyBullets;
        Enemy enemigo1;
        std::vector<Enemy*> _enemies;
        int _timerReload;
        Player _juego;

        Explosion _explosion;
        float _frameExplosion;
        bool _isExplosionActive;
        int _frames;

        Sounds _sound;

        std::vector<Explosion*> _vExplosiones; /// SI HAY EXPLISION
        std::vector<bool*> _vec_isExplosiveActive; /// VERIFICAR SI ESTA EN VERDADERO
        std::vector<float*> _vec_frameExplosive; /// LOS FRAMES PARA MANDERSELO A LA FUNCION

        PowerUp *_powerUp;

        sf::Clock clock;

};

