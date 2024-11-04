#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
#include "Avion.h"

/*
enum class PersonajeState {
	Idle = 0,
	Move_Left,
	Move_Right,
	Move_Down,
	Move_Up,
	Move_Back
};
*/

class Personaje: public sf::Drawable, public Collisionable, public Avion, public sf::Transformable
{
  public:
    Personaje();
    void cmd()override; /// HEREDA DE AVION
    void update() override; /// HEREDA DE AVION
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;/// HEREDA DE COLLISIONABLE
    sf::FloatRect getBounds() const override; /// HEREDA DE COLLISIONABLE
    bool Shoot(); /// SI APRETAMOS LA TECLA DISPARO
    sf::Vector2f getPosition() override;/// OBTENMOS LA POSICION DEL PERSONAJE ACTUALIZADO
    bool getTeclaAnterior()const;
    void setTeclaAnterior(bool);
    sf::Vector2f getBulletOrigin() const;
  private:
    bool _teclaAnterior = false;
    //float _frame;
   // PersonajeState _state;
  /// TODO: Machine states
};
