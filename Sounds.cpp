#include "Sounds.h"

Sounds::Sounds()
{
  _buffer.loadFromFile("assets/sounds/sfx/impact_explosion_03.mp3");
  _sound.setBuffer(_buffer);
  _sound.setVolume(10.f);

  _bufferPew.loadFromFile("assets/sounds/sfx/pew-pew-disparo.mp3");
  _soundPew.setBuffer(_bufferPew);
  _soundPew.setVolume(15.f);

}

void Sounds::playExplosionSmall()
{
  _sound.play();
}


void Sounds::playDisparoPew()
{
  _soundPew.play();
}

void Sounds::stopAllSounds()
{
  _sound.stop();
  _soundPew.stop();
}

Sounds::~Sounds()
{

}

