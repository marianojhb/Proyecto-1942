#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class Sounds
{
  public:
    Sounds();
    void playExplosionSmall();
    void playDisparoPew();
    void stopAllSounds();
    ~Sounds();

  private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
    sf::SoundBuffer _bufferPew;
    sf::Sound _soundPew;
};


