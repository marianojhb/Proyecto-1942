#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cmath>
#include "Bullet.h"
//#define M_PI 3.14159265358979323846
//#include <iostream>
#include "Collisionable.h"
#include "Avion.h"
#include "Sounds.h"
#include "Explosion.h"

enum class EnemyBState{
        Vivo=0,
        Muerto
};

class EnemyB: public sf::Drawable, public Collisionable, public Avion
{
    public:
        EnemyB();
        void cmd() override; /// HEREDA DE AVION
        void update() override; /// HEREDA DE AVION
        sf::Vector2f getPosition()const override; /// HEREDA DE AVION
        void draw(sf::RenderTarget &target , sf::RenderStates states)const override;/// HEREDA DE DIBUJABLE
        sf::FloatRect getBounds() const override; /// HEREDA DE COLLISIONABLE
        bool shot();
        void respawn();
        void explosion();
        void setStateMuerto();
        void inicioEnemyB(int);

    private:
        sf::Vector2f _newPosition;
        bool _canShoot;
        float _frame;
        int _timeRespawn;
        int _timeReload;
        Sounds _sound;
        float _frameExplosion;
        Explosion *_explosion;
        EnemyBState _state;
        //sf::Vector2f _moveHacia;
        //float _angle;
        //int _horizonte;
        std::vector <EnemyB*> _vEnemiesB;
         std::vector<Bullet*> _bullets_vEnemyB;

};

