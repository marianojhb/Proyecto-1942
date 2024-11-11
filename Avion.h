#pragma once
#include <SFML/Graphics.hpp>

class Avion /// CLASE ABSTRACTA, ["Avionabel" o "IAvion"]
{
  public:
    virtual void update() = 0; /// VIRTUAL PURO
    virtual void cmd() = 0; /// VIRTUAL PURO
    virtual sf::Vector2f getPosition() const = 0; /// VIRTUAL PURO

  protected:
    sf::Texture *_texture;
    sf::Sprite _sprite;
    sf::Vector2f _velocity;
    bool _shoot;

};

