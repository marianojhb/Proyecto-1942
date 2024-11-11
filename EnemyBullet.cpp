#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(sf::Vector2f origen, sf::Vector2f direccion, float velocidad)
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-projectiles.png");
  _sprite.setTexture(*_texture);
  _sprite.setTextureRect({73,14,4,4}); // tamaño x 3 = 12*12
  _sprite.setScale(3,3);
  _sprite.setPosition(origen);
  _velocidad_bala = velocidad;
  _direccion = direccion;

  // Normaliza la dirección y luego escala con la velocidad
  float magnitud = sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
  if (magnitud != 0) {
    _direccion = (direccion / magnitud) * _velocidad_bala;
  } else {
    _direccion = {0, 0}; // Evitar división por cero
  }
}

void EnemyBullet::update()
{
  _sprite.move(_direccion);
}

void EnemyBullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(_sprite, states);
}

sf::FloatRect EnemyBullet::getBounds() const
{
  return _sprite.getGlobalBounds();
}

