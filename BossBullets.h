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

class BossBullets: public sf::Drawable, public Collisionable
{
public:

    // BossBullets(int horizontal , int vertical , float velocidad);
    // BossBullets(sf::Vector2f origen, sf::Vector2f direccion, float velocidad);
    BossBullets(int eje_x, int eje_y , float angulo , float velocidad);
    ~BossBullets();
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
    sf::FloatRect getBounds() const override;
    //void setMove(sf::Vector2f direccion);
    float getVelocidadBala() const;

private:
    sf::Sprite _sprite;
    sf::Texture *_texture = nullptr;
    float _velocidad_bala;
    float _dx;
    float _dy;
    sf::Vector2f _direccion;
};




