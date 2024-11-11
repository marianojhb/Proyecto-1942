// Enemy.cpp

#include <iostream>
#include "Enemy.h"

Enemy::Enemy()
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-enemy.png");
  _sprite.setTexture(*_texture);
  _sprite.setTextureRect({59,57,15,14});
  _sprite.setScale(3,3);
  _sprite.setPosition( std::rand()%600+1,0-48-48);
  _velocity.x = std::rand()%2 ? (float)(std::rand()%50)/10+1.0 :  - (float)(std::rand()%50)/10+1.0;
  _velocity.y = 3.2f;

//  textura[] = {4,19,14,14};
//  textura[] = {24,19,13,13};
//  textura[] = {42,19,15,13};
//  textura[] = {61,19,15,14};
//  textura[] = {81,19,14,13};
//  textura[] = {101,19,13,13};
//  textura[] = {120,19,13,14};
//  textura[] = {119,39,14,13};
//  textura[] = {5,39,14,13};

//  textura[1] = {101,58,13,13};
//  textura[2] = {81,57,15,14};
//  textura[3] = {61,57,15,14};
//  textura[4] = {43,57,13,15};
//  textura[5] = {24,58,13,13};
//  textura[6] = {5,58,14,13};

}

void Enemy::cmd() {

  std::vector <sf::IntRect> textura(16);
  float angle = atan2(_velocity.y, _velocity.x) * 180 / M_PI;
  int direction =(int)((angle + 360) / 22.5) % 16;

  textura[1] = {100,58,13,13}; // 6 con 1 (inclinacion 2)
  textura[2] = {80,57,15,14}; // 2 con 5 (inclinacion 1)
  textura[3] = {59,57,15,14}; // 3 con 4 (derecho)
  textura[4] = {59,57,15,15}; // 3 con 4 (derecho)
  textura[5] = {22,58,13,13}; // 5 con 2 (inclinacion 1)
  textura[6] = {4,58,14,13}; // 6 con 1 (inclinacion 2)

  switch(direction)
  {
    case 1:

      _sprite.setTextureRect(textura[1]);
      break;
    case 2:
      _sprite.setTextureRect(textura[2]);
      break;
    case 3:
      _sprite.setTextureRect(textura[3]);
      break;
    case 4:
      _sprite.setTextureRect(textura[4]);
      break;
    case 5:
      _sprite.setTextureRect(textura[5]);
      break;
    case 6:
      _sprite.setTextureRect(textura[6]);
      break;
  }



  // rebote en los costados:
  if (_sprite.getPosition().x + getBounds().width < 0 + getBounds().width  ) {
    _velocity.x = -_velocity.x;


  }
  if (_sprite.getPosition().x + getBounds().width > 600 + getBounds().width) {
    _velocity.x = -_velocity.x;

  }
  if (_sprite.getPosition().y - getBounds().height > 800){
    respawn();
  }


//  if (_velocity.x < 0 )
//  {
//    _sprite.setTextureRect({22,58,13,13});
//  } else
//  {
//    _sprite.setTextureRect({99,58,15,14});
//  }

}

void Enemy::update()
{

  _sprite.move(_velocity.x,_velocity.y);
}

void Enemy::respawn(){

  _sprite.setPosition(std::rand()%600,0-48-48);
  _velocity.x = std::rand()%2 ? (float)(std::rand()%30)/10+1.0 :  - (float)(std::rand()%30)/10+1.0;
}

void Enemy::draw(sf::RenderTarget &target , sf::RenderStates states)const {
  target.draw(_sprite, states);
}

sf::FloatRect Enemy::getBounds()const {
  return _sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getBulletOrigin()
{
  return {_sprite.getPosition().x + 20 ,_sprite.getPosition().y+54};
}

sf::Vector2f Enemy::getPosition() const
{
  return _sprite.getPosition();
}
