// Player.h

#pragma once
#include <SFML/Graphics.hpp>
#include "Collisionable.h"
#include "Avion.h"
#include <string>
#include "Sounds.h"

class Player: public sf::Drawable, public Collisionable, public Avion
{
public:

    Player() {
      _texture = new sf::Texture;
      _texture->loadFromFile("assets/sprites/1942-sprites-player.png");
      _sprite.setTexture(*_texture);
      _sprite.setTextureRect({107,17,32,18}); // tamaño x3 = 75x54
      _sprite.setScale(3,3);
      _position = {(600-96)/2+2,700};
      _sprite.setPosition(_position);
      _velocity = {5,5};
      _shoot = false;
    }

    void cmd() override
    {

      _velocity = {0, 0};

      bool teclaMovimientoPresionada = false; // Inicializamos en false al principio de cada llamada

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        teclaMovimientoPresionada = true;
        _velocity.x = 5;
        _sprite.setTextureRect({180,18,23,16});
      }

      if (!teclaMovimientoPresionada && getTeclaAnterior()) {
        _sprite.setTextureRect({107,17,32,18}); // tamaño x3 = 75x54
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        teclaMovimientoPresionada = true;
        _velocity.x = -5;
        _sprite.setTextureRect({42,18,22,16});
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      {
        _velocity.y = -5;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      {
        _velocity.y = 5;
      }

      setTeclaAnterior(teclaMovimientoPresionada);


//  /**
//  if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
//  {
//    _state = PlayerState::Move_Back;
//  }
//  */

    }

    sf::Vector2f getBulletOrigin() const
    {
      return {_sprite.getPosition().x + (96-48)/2 ,_sprite.getPosition().y};
    }

    void update() override
    {

      /*
      switch (_state)
      {
        case PlayerState::Idle:
          _sprite.setTextureRect({213,33,50,34});

          break;
        case PlayerState::Move_Right:
          _frame += 0.2;

          if(_frame >= 4){ _frame = 0;}

          _sprite.setTextureRect({270 + int(_frame)*40,33,65,34});
          break;
        case PlayerState::Move_Left:

          break;
        case PlayerState::Move_Up:

          break;
        case PlayerState::Move_Down:

          break;
        case PlayerState::Move_Back:

          break;
      }
    */

      /// Restricciones de pantalla
      if (_sprite.getPosition().x < -74/2 ){
        _sprite.setPosition(-74/2, _sprite.getPosition().y);
      }
      if (_sprite.getPosition().y < 0) {
        _sprite.setPosition(_sprite.getPosition().x, 0);
      }
      if(_sprite.getPosition().x > 600+ 74/2 - getBounds().width) {
        _sprite.setPosition(600 + 74/2 - getBounds().width, _sprite.getPosition().y);
      }
      if(_sprite.getPosition().y > 800 - getBounds().height) {
        _sprite.setPosition(_sprite.getPosition().x, 800 - getBounds().height);
      }
      _sprite.move({_velocity.x, _velocity.y});
    }

    bool Shoot()
    {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
      {
        if (!_shoot)
        {
//      _disparoPew.playDisparoPew();
          _shoot = true;
          return true;
        }
      }
      else
      {
        /// Cuando deje de soltar el espacio, el player va a poder disparar.
        if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
        {
          _shoot = false;
        }
      }

      return false;
    }

    sf::FloatRect getBounds()const override
    {
      return _sprite.getGlobalBounds();
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states)const override
    {
      target.draw(_sprite, states);
    }

    void setTeclaAnterior(bool teclaAnterior)
    {
      _teclaAnterior = teclaAnterior;
    }

    bool getTeclaAnterior() const
    {
      return _teclaAnterior;
    }

    void respawn()
    {
      _sprite.setPosition((600-96)/2+2, 700);
    }

    sf::Vector2f getPosition() const override
    {
      return _sprite.getPosition();
    }





private:
    bool _teclaAnterior = false;
    sf::Vector2f _position;
    Sounds _disparoPew;
    /// TODO: Machine states
};
