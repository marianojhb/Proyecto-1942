// Enemy.h

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Collisionable.h"
#include "Avion.h"

class Enemy: public sf::Drawable, public Collisionable, public Avion
{
    public:
        Enemy();
        void cmd() override; /// HEREDA DE AVION
        void update() override; /// HEREDA DE AVION
        void draw(sf::RenderTarget &target , sf::RenderStates states) const override;/// HEREDA DE DIBUJABLE
        sf::FloatRect getBounds() const override; /// HEREDA DE COLLISIONABLE
        void respawn();
        sf::Vector2f getBulletOrigin();
        sf::Vector2f getPosition() const override;

    private:
        sf::Vector2f _newPosition;

};
