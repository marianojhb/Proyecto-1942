#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class EnemyBullet: public sf::Drawable, public Collisionable
{
public:
    EnemyBullet(sf::Vector2f origen, sf::Vector2f direccion, float velocidad);
    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
    sf::FloatRect getBounds() const override;
    void setMove(sf::Vector2f direccion);
    float getVelocidadBala() const;

private:
    sf::Sprite _sprite;
    sf::Texture *_texture;
    float _velocidad_bala;
    sf::Vector2f _direccion;

};
