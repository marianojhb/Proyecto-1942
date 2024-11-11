#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
//#include "Scene.h"

class PowerUp : public sf::Drawable, public Collisionable
{
  public:
    PowerUp();
    void respawn();
    void update();
    void draw(sf::RenderTarget &, sf::RenderStates)const override;
    sf::FloatRect getBounds() const override;
  private:
    sf::Texture _texture;
    sf::Sprite _sprite;
};
