#pragma once
#include <SFML/Graphics.hpp>


class Creditos: public sf::Drawable{
    public:
        Creditos();
        void draw(sf::RenderTarget &target , sf::RenderStates states) const override;
        void update();


    private:
        sf::Font font;
        sf::Text texts[5];
        float velocidad;
};
