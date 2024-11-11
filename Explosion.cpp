#include "Explosion.h"

Explosion::Explosion(){}

Explosion::Explosion(int horizontal, int vertical)
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-effects.png");
  _sprite.setTexture(*_texture);
  _sprite.setPosition(sf::Vector2f(horizontal,vertical));
  _sprite.setTextureRect({1,110,32,32});

}

void Explosion::smallExplosion(float &frame)
{
  /// Dependiendo del rectangulo , tengo que dibujarlo...

  frame+=0.2;
  _sprite.setTextureRect({38*(int)(frame/1.5),110,33,32});
}

/*
void Explosion::smallExplosion()
{
  /// Dependiendo del rectangulo , tengo que dibujarlo...

  _frame+=0.2f;

  if(_frame >= 8)
  {
    _frame = 0.f;
  }
   _sprite.setTextureRect({38*(int)(_frame/1.5),110,33,32});
}
*/

void Explosion::draw(sf::RenderTarget &target, sf::RenderStates states)const
{
  target.draw(_sprite, states);
}



