#include "Creditos.h"
#include <iostream>

using namespace std;

Creditos::Creditos(){

      if (!font.loadFromFile("assets/fonts/fuente.ttf")) {

        cout << "No se pudo cargar la fuente" <<endl;
    }


    // Crea varios textos y los agrega al arreglo
    texts[0].setString("Desarrolado por:");
    texts[0].setFont(font);
    texts[0].setCharacterSize(20);
    texts[0].setPosition(10.f, -250.f);  // Empieza fuera de la pantalla arriba

    texts[1].setString("Carlos Zubilete Sanchez");
    texts[1].setFont(font);
    texts[1].setCharacterSize(20);
    texts[1].setPosition(10.f, -200.f);  // Empieza fuera de la pantalla arriba

    texts[2].setString("Mariano Belgrano");
    texts[2].setFont(font);
    texts[2].setCharacterSize(20);
    texts[2].setPosition(10.f, -150.f);  // Empieza fuera de la pantalla arriba

    texts[3].setString("Enzo Segovia");
    texts[3].setFont(font);
    texts[3].setCharacterSize(20);
    texts[3].setPosition(10.f, -100.f);  // Empieza fuera de la pantalla arriba

    texts[4].setString("Nicolas Esteban Sosa");
    texts[4].setFont(font);
    texts[4].setCharacterSize(20);
    texts[4].setPosition(10.f, -50.f);  // Empieza fuera de la pantalla arriba


   velocidad = 02.0f;
}


void Creditos::update() {
    // Mueve todos los textos en el arreglo
    for (int i = 0; i < 5; ++i) {  // 3 es el número de textos en el arreglo
        texts[i].move(0, velocidad * (1.0f / 60.0f));  // Mueve el texto hacia abajo

        if (texts[i].getPosition().y > 820) {  // Si el texto sale de la pantalla

            //texts[i].setPosition(100.0f, -texts[i].getGlobalBounds().height);  // Reinicia la posición arriba
        }
    }
}

void Creditos::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Dibuja todos los textos del arreglo
    for (int i = 0; i < 5; ++i) {  // 3 es el número de textos en el arreglo
        target.draw(texts[i], states);
    }
}


