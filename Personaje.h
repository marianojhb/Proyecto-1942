#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
#include "Avion.h"
#include <string>
#include "Sounds.h"

class Personaje: public sf::Drawable, public Collisionable, public Avion
{
  public:
    Personaje();
    void cmd() override; /// HEREDA DE AVION
    void update() override; /// HEREDA DE AVION
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;/// HEREDA DE COLLISIONABLE
    sf::FloatRect getBounds() const override; /// HEREDA DE COLLISIONABLE
    bool Shoot(); /// SI APRETAMOS LA TECLA DISPARO
    bool getTeclaAnterior()const;
    void setTeclaAnterior(bool);
    sf::Vector2f getBulletOrigin() const;
    sf::Vector2f getPosition() const override;
    void respawn();
    void hide();
    bool getInmundidad() const;
    void setInmundidad(bool);



  private:
    bool _teclaAnterior = false;
    sf::Vector2f _position;
    Sounds _disparoPew;
    bool _inmunidad;
  /// TODO: Machine states
};
