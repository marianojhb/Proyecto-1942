#pragma once
#include <SFML/Graphics.hpp>
#include "Fecha.h"
#include "Player.h"



class Ranking
{

public:
    Ranking();
    void cargar(sf::Text puntos, sf::Text dia, sf::Text mes, sf::Text anio, sf::Text nombre);
    Ranking buscaPosicion(int);
    void mostrar();
    sf::Text getNombre();
    Fecha getFecha();
    void setNombre(sf::Text nombre);
    void setFecha(sf::Text dia, sf::Text mes, sf::Text anio);
    void setPuntos(sf::Text puntos);
    int getPuntos();

private:
    int _puntos;
    char _nombre[30];
    Fecha _pFecha;


};


