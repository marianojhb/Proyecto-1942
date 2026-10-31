#include "MenuRanking.h"
#include <iostream>
#include <algorithm>

MenuRanking::MenuRanking()
{

  RankingArchivo ra("assets/config/ranking.dat");

  _cantidadRegistros = ra.CantidadRegistros();
  Ranking registros[10];
  const int cantidad = std::min(_cantidadRegistros, 10);
  Ranking *reg = ra.obtenerRegistrosRanking(_cantidadRegistros);
  if (reg != nullptr) {
    for (int i = 0; i < cantidad; ++i) {
      registros[i] = reg[i];
    }
    delete[] reg;
  }
  Ranking::ordenarRanking(registros, 10);
  _rankingleido = "";
  _rankingleido += "POS\tSCORE\tDATE\t\tNAME\n";
  for (int i = 0; i < 10; i++) {
    std::string nom_str = registros[i].getNombre().getString();
    _rankingleido +=
            std::to_string(i + 1) + "\t" + std::to_string(registros[i].getPuntos()) + "\t" + registros[i].getFecha().toString() +
            "\t" + nom_str + "\n";
  }
  _rankingMasBajo = registros[9].getPuntos();
  _rankingMasAlto = registros[0].getPuntos();


  if (!font.loadFromFile("assets/fonts/fuente.ttf")) {

    std::cout << "No se pudo cargar la fuente" << std::endl;
  }
  texto.setFillColor(sf::Color::Magenta);
  lectura_ranking = _rankingleido;
  texto.setString(lectura_ranking);
  texto.setFont(font);
  texto.setCharacterSize(15);
  texto.setLineSpacing(5);
  texto.setOrigin(texto.getLocalBounds().width / 2, texto.getLocalBounds().height / 2);
  texto.setPosition(600.f / 2, 1000.f);
  if (!fondo_texture.loadFromFile("assets/sprites/1942_clock_3x4.png")) {
    std::cout << "No se pudo cargar la fuente" << std::endl;
  }
  fondo.setTexture(fondo_texture);
  fondo.setPosition(0, 0);
  texto_front = texto;
  texto_front.setFillColor(sf::Color::White);
  texto_front.move(1,1);

}

void MenuRanking::update()
{
  texto.move(0,-0.4);
  texto_front.move(0,-0.4);
}

void MenuRanking::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(fondo, states);
  target.draw(texto, states);
  target.draw(texto_front, states);

}

void MenuRanking::setRankingLeido(std::string &rankingleido)
{
  lectura_ranking = rankingleido;
}

std::string MenuRanking::getRankingLeido() const
{
  return lectura_ranking;
}

int MenuRanking::obtenerRankingMasBajo() const
{
  return _rankingMasBajo;
}

int MenuRanking::obtenerRankingMasAlto() const
{
  return _rankingMasAlto;
}
