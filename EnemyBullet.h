#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//#include <iostream>

class EnemyBullet: public sf::Drawable, public Collisionable
{
public:

    EnemyBullet(sf::Vector2f origen, sf::Vector2f direccion, float velocidad);
    EnemyBullet(float horizontal,float vertical, float velocidadX, float velocidadY);
    ~EnemyBullet();
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
    sf::FloatRect getBounds() const override;
    void setMove(sf::Vector2f direccion);
    float getVelocidadBala() const;

private:

    sf::Sprite _sprite;
    sf::Texture *_texture = nullptr;
    float _velocidad_bala;
    float _velocidadX;
    float _velocidadY;
    sf::Vector2f _direccion;

};
