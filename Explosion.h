#pragma once
#include <SFML/Graphics.hpp>

class Explosion: public sf::Drawable
{
  public:
    Explosion();
    Explosion(int horizontal , int vertical);
    ~Explosion();
    void reset(int horizontal, int vertical);
    //void smallExplosion();
    void smallExplosion(float &frame);
    void bigExplosion(float &frame);
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
  private:
    sf::Texture *_texture = nullptr;
    sf::Sprite _sprite;
    float _frame;

};
