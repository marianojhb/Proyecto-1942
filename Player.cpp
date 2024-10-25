#include "Player.h"


/// sf::Vector2u textureSize = texture.getSize();
// Obtemos el ancho y el alto de la textura: {textureSize.x , textureSize.y}

Player::Player()
{
  _velocity = {5,5};
  _shoot = false;
  _texture.loadFromFile("img/1942-sprites-player.png");
  _sprite.setTexture(_texture);
  _sprite.setTextureRect({1,62,25,18});
  _sprite.setPosition({295,550});
  _sprite.setScale(3,3);
  _sprite.setOrigin({_sprite.getGlobalBounds().width/2,
                    _sprite.getGlobalBounds().height/2});
}

void Player::Update()
{
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    _sprite.move(_velocity.x,0);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    _sprite.move(-_velocity.x,0);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    _sprite.move(0,-_velocity.y);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    _sprite.move(0,_velocity.y);
  }
  /// Restricciones de pantalla
  if (_sprite.getGlobalBounds().left < 0 )
  {
    _sprite.setPosition((_sprite.getOrigin().x + _sprite.getGlobalBounds().width) ,
                        _sprite.getPosition().y);
  }
  if (_sprite.getGlobalBounds().top < 0){
    _sprite.setPosition(_sprite.getPosition().x,
                        (_sprite.getOrigin().y + _sprite.getGlobalBounds().height));
  }
  if(_sprite.getGlobalBounds().left + _sprite.getGlobalBounds().width > 600){
    _sprite.setPosition(600 - ( _sprite.getOrigin().x - _sprite.getGlobalBounds().width),
                        _sprite.getPosition().y);
  }
  if(_sprite.getGlobalBounds().top + _sprite.getGlobalBounds().height > 800){
    _sprite.setPosition(_sprite.getPosition().x,
                        800 + ( _sprite.getGlobalBounds().height - _sprite.getOrigin().y));
  }
}

bool Player::Shoot()
{
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
  {
    /**
    **/
    if (!_shoot)
    {
      _shoot = true;
      return true;
    }
  }
  else
  {
    /**
    **/
    /// Cuando deje de soltar el espacio, el player va a poder disparar.
    if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
    {
      _shoot = false;
    }
  }

  return false;
}

sf::Vector2f Player::getPosicion()
{
  return _sprite.getPosition();
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states)const
{
  target.draw(_sprite, states);
}

/**
Player::Player(int x,int y, sf::Texture &texture)
{
  _sprite.setTexture(texture);
  _sprite.setTextureRect(sf::IntRect(1,62,25,18));
  _sprite.setPosition(x,y);
  _sprite.setScale(3,3);
  _sprite.setOrigin({_sprite.getGlobalBounds().width/2,
                    _sprite.getGlobalBounds().height/2});

}
**/
/**
int getPosicionX()
{
  return _sprite.getPosition().x;
}

int getPosicionY()
{
  return _sprite.getPosition().y;
}
**/
