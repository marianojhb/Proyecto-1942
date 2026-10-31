#include "BossBullets.h"

BossBullets::~BossBullets()
{
  delete _texture;
}

/// POSDEL JEFE ->
/// TODO: tiene que tener una direccion y angulo..

/*
BossBullets::BossBullets(int horizontal , int vertical , float velocidad)
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-projectiles.png");
  _sprite.setTexture(*_texture);
  _sprite.setTextureRect({73,14,4,4});
  _sprite.setScale(5,5);
  _sprite.setPosition(horizontal,vertical);
  _velocidad_bala = velocidad;
}
*/
BossBullets::BossBullets(int eje_x, int eje_y , float angulo, float velocidad)
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-projectiles.png");
  _sprite.setTexture(*_texture);
  _sprite.setTextureRect({73,14,4,4});
  _sprite.setScale(5,5);
  _sprite.setPosition(eje_x,eje_y);
  _velocidad_bala = velocidad;
  /// CALCULAMOS LA DIRECCION
  _dx = cos(angulo * (M_PI / 180.0f))*velocidad;
  _dy = sin(angulo * (M_PI / 180.0f))*velocidad;


}

void BossBullets::update()
{
  _sprite.move(_dx,_dy);
}

void BossBullets::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(_sprite, states);
}

sf::FloatRect BossBullets::getBounds() const
{
  return _sprite.getGlobalBounds();
}

/*
float BossBullets::calcularAngulo()
{
    /// CALCULAR LA PENDIENTE, ENTRE LOS DOS PUNTOS:
    /// SI LA PENDIENTE ES POSITIVA ESTAMOS EN ANGULO AGUDO
    /// SI ES NEGATIVA ES UN ANGULO OBTUSO , LE SUMAMOS 180
    float dy = _newPosition.y - _sprite.getPosition().y;
    float dx = _newPosition.x - _sprite.getPosition().x;

    float anguloEnRadianes = atan2(dy,dx);
    /// atan() retorna el arcotangente de un n�mero en radianes
    /// atan() retorna  �ngulos entre -PI/2 y PI/2
    /// atan2() retorna cualer angulo;

    /// Hacemos la conversion ...
    _angulo = anguloEnRadianes * (180 / M_PI);

    /// TODO: averiguar mas sobre pi radianes..
    if ( _angulo < 0 )
    {
        _angulo+=360;
    }

    return _angulo;
}

float BossBullets::calcularDistancia(){

   /// APLICAMOS FOMULA DE LA DISTANCIA ...
    sf::Vector2f currentPosition = _sprite.getPosition();
    _direction = _newPosition - currentPosition;
    float distancia = std::sqrt(_direction .x * _direction .x + _direction .y * _direction .y);

    return distancia;
}

BossBullets::BossBullets(sf::Vector2f origen, sf::Vector2f direccion, float velocidad)
{
  _texture = new sf::Texture;
  _texture->loadFromFile("assets/sprites/1942-sprites-projectiles.png");
  _sprite.setTexture(*_texture);
  _sprite.setTextureRect({73,14,4,4}); // tama�o x 3 = 12*12
  _sprite.setScale(3,3);
  _sprite.setPosition(origen);
  _velocidad_bala = velocidad;
  _direccion = direccion;

  // Normaliza la direcci�n y luego escala con la velocidad
  float magnitud = sqrt(direccion.x * direccion.x + direccion.y * direccion.y);
  if (magnitud != 0) {
    _direccion = (direccion / magnitud) * _velocidad_bala;
  } else {
    _direccion = {0, 0}; // Evitar divisi�n por cero
  }
}
  */
