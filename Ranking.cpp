#include <iostream>
#include <stdio.h>
#include <cstring>
#include "Ranking.h"

Ranking::Ranking()
{
  _puntos = 0;
  strcpy(_nombre,"");
  _pFecha={1,1,1900};
}



void Ranking::cargar(int puntos, Fecha fecha, sf::Text nombre)
{
  setPuntos(puntos);
  setFecha(fecha);
  setNombre(nombre);
}

void Ranking::mostrar()
{
  std::cout << std::to_string(getPuntos()) << "   ";
  std::cout << std::to_string(getFecha().getDia()) << "-" << std::to_string(getFecha().getMes()) << "-" << std::to_string(getFecha().getAnio()) << "   ";
  std::string nom_str = getNombre().getString();
  std::cout << nom_str << std::endl;

}

sf::Text Ranking::getNombre()
{
  sf::Text nombre;
  std::string nombre_str (_nombre);
  nombre.setString(_nombre);
  return nombre;
}

void Ranking::setNombre(sf::Text nombre)
{
  std::string nombre_str = nombre.getString();
  std::strncpy(_nombre, nombre_str.c_str(), sizeof(_nombre) - 1);
  _nombre[sizeof(_nombre) - 1] = '\0';
}

int Ranking::getPuntos()
{
  return _puntos;
}

void Ranking::setPuntos(int puntos)
{
  _puntos = puntos;
}

Fecha Ranking::getFecha()
{
  return {_pFecha.getDia(),_pFecha.getMes(),_pFecha.getAnio()};
}

void Ranking::setFecha(Fecha fecha)
{
  _pFecha = fecha;
}

void Ranking::ordenarRanking(Ranking* registros, int cantidad)
{
  for (int i = 0; i < cantidad - 1; ++i) {
    int maxIndex = i;

    // Buscar el índice del mayor elemento en la sublista
    for (int j = i + 1; j < cantidad; ++j) {
      if (registros[j].getPuntos() > registros[maxIndex].getPuntos()) {
        maxIndex = j;
      }
    }

    // Intercambiar el elemento mayor encontrado con el primer elemento de la sublista
    if (maxIndex != i) {
      Ranking temp = registros[i];
      registros[i] = registros[maxIndex];
      registros[maxIndex] = temp;
    }
  }
}
