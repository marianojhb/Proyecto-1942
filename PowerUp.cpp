#include "PowerUp.h"
#include <iostream>


PowerUp::PowerUp(){
  _texture.loadFromFile("assets/sprites/1942-sprites-projectiles.png");
  _sprite.setTexture(_texture);
  int lista_powerUpRandom[] = {105,124,143,162,181,200,219,238};
  int powerUpRandom = std::rand()%8;
  std::cout << lista_powerUpRandom[powerUpRandom] << std::endl;
  _sprite.setTextureRect({lista_powerUpRandom[powerUpRandom],11,16,15}); // X,Y,widh,heigh
  _sprite.scale(3,3);
//  _sprite.setPosition({-50,-50});
//  _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height/2);
}

void PowerUp::update()
{
  _sprite.move(0,2.3f);
}

void PowerUp::respawn(){
  _sprite.setPosition(std::rand() % 500 + _sprite.getGlobalBounds().width,
                      std::rand() % 400 + _sprite.getGlobalBounds().height);
}

sf::FloatRect PowerUp::getBounds()const{
  return _sprite.getGlobalBounds();
}

void PowerUp::draw(sf::RenderTarget &target, sf::RenderStates states)const{
  target.draw(_sprite, states);
}


