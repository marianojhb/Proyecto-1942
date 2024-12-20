#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>

#include "Scene.h"
#include "Menu.h"
#include "Creditos.h"
#include "RankingArchivo.h"
#include "Ranking.h"
#include "Fecha.h"
#include "MenuRanking.h"
#include "MenuConfiguracion.h"
#include "Configuracion.h"
#include "ConfiguracionArchivo.h"


int main() {

  ConfiguracionArchivo ca;
  Configuracion *_configuracion = ca.obtenerConfiguracion();

  MenuConfiguracion menuConfiguracion;
  Sounds pew;




  std::srand((unsigned) std::time(0));

  sf::RenderWindow window(sf::VideoMode(600, 800), "1942 @UTN", sf::Style::Default);

  Menu mainMenu(window.getSize().x, window.getSize().y);

  window.setFramerateLimit(60);

//   Definir la vista
  sf::View view(sf::FloatRect(0.f, 0.f, 600.f, 800.f));
  window.setView(view);



  // Main theme 2
  sf::SoundBuffer buffer_main_theme_v2;
  buffer_main_theme_v2.loadFromFile("assets/sounds/themes/02_main_theme_v2_stereo.mp3");
  sf::Sound main_theme_v2;
  main_theme_v2.setBuffer(buffer_main_theme_v2);
  main_theme_v2.setVolume(7.f);
  if( _configuracion->getMusic()) main_theme_v2.play();

  // Main theme 1
  sf::SoundBuffer buffer_main_theme_v1;
  buffer_main_theme_v1.loadFromFile("assets/sounds/themes/01_main_theme_v1_stereo.mp3");
  sf::Sound main_theme_v1;
  main_theme_v1.setBuffer(buffer_main_theme_v1);
  main_theme_v1.setVolume(7.f);

  // Nostalgia theme
  sf::SoundBuffer buffer_nostalgia;
  buffer_nostalgia.loadFromFile("assets/sounds/themes/04_high_score_entry_stereo.mp3");
  sf::Sound nostalgia;
  nostalgia.setBuffer(buffer_nostalgia);
  nostalgia.setVolume(7.f);


  // CARTEL CONTINUAR EN EL JUEGO
  sf::Font font;
  font.loadFromFile("assets/fonts/MONOCOQUE_FUENTE.ttf");
  sf::Text continuar_text;
  sf::RectangleShape continuar_text_fondo;

  bool mostrarContinuar = false;


  // GAME LOOP
  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {
//
      if ((event.type == sf::Event::Closed) ||
          (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
//
//
      if (event.type == sf::Event::KeyReleased) {
//
        if (event.key.code == sf::Keyboard::Up) {
          mainMenu.moveUp();
//
        } else if (event.key.code == sf::Keyboard::Down) {
          mainMenu.moveDown();
//
        } else if (event.key.code == sf::Keyboard::Return) {
//
          if (mainMenu.getShowInsertCoin()) {
            mainMenu.handleEnterPress();
//
          } else {
//
            int x = mainMenu.getPressedItem();
//
            if (x == 0) // JUEGO ARRANCA SCENE+GAMEPLAY
            {

              Scene scene;

//
              sf::RenderWindow Play(sf::VideoMode(600, 800), "1942");
              Play.setFramerateLimit(60);
              main_theme_v2.stop();
              if( _configuracion->getMusic()) main_theme_v1.play();
              mostrarContinuar = false;

              // GAME LOOP
              while (Play.isOpen()) {

                // EVENTS /////////////////////////////////////////////////////////
                sf::Event playEvent;

                while (Play.pollEvent(playEvent))
                {

                  if (playEvent.type == sf::Event::Closed) {
                    Play.close();
                    main_theme_v1.stop();

                  }

                  if (playEvent.type == sf::Event::KeyPressed) {
                    if (playEvent.key.code == sf::Keyboard::Escape) {
                      mostrarContinuar = true;
                      continuar_text_fondo.setSize({300, 150});
                      continuar_text_fondo.setOrigin(continuar_text_fondo.getLocalBounds().width / 2,
                                                     continuar_text_fondo.getLocalBounds().height /
                                                     2);
                      continuar_text_fondo.setPosition(600 / 2, 800 / 2);
                      continuar_text_fondo.setFillColor(sf::Color(0, 0, 0, 128));

                      continuar_text.setFillColor(sf::Color::White);
                      continuar_text.setString("SALIR  Y  N");
                      continuar_text.setCharacterSize(24);
                      continuar_text.setPosition(600 / 2, 800 / 2);
                      continuar_text.setFont(font);
                      continuar_text.setOrigin(continuar_text.getGlobalBounds().width / 2,
                                               continuar_text.getGlobalBounds().height / 2);
                    }

                    if (mostrarContinuar) {
                      if (playEvent.key.code == sf::Keyboard::N) {
                        continuar_text.setPosition(-1000,0);
                        continuar_text_fondo.setPosition(-1000,0);
                      }

                      else if (playEvent.key.code == sf::Keyboard::Y) {

                        main_theme_v1.stop();
                        if( _configuracion->getMusic())main_theme_v2.play();
                        Play.close();
                        mostrarContinuar = false;


                      }
                    }
                  }
                }/// FIN DEL WHILE INTERNO . . .

                // CMD ////////////////////////////////////////////////////////////
                if (scene.getJuegoTerminado()) {
                  ///  STOP MUSICA DEL JUEGO
                  scene.setJuegoTerminado(true);
                  main_theme_v1.stop();
                }
                // UPDATE /////////////////////////////////////////////////////////

                if (!scene.getJuegoTerminado()) {
                  scene.cmd();
                  scene.update();
                }

                // DRAW ///////////////////////////////////////////////////////////
                Play.clear();

                Play.draw(scene);

                if (mostrarContinuar) {
                  Play.draw(continuar_text_fondo);
                  Play.draw(continuar_text);
                } // DIBUJO CARTEL CONTINUAR
                // FLIP ///////////////////////////////////////////////////////////
                Play.display();
                // LIBERACION DEL JUEGO

              } /// FIN DEL GAME LOOP
//
            }
            else if (x == 1) // CONFIGURACION
            {

              sf::RenderWindow windowConfiguracion(sf::VideoMode(600,800), "Configuracion");
              windowConfiguracion.setFramerateLimit(60);
              while(windowConfiguracion.isOpen())
              {
                sf::Event configuracionEvent;

                while(windowConfiguracion.pollEvent(configuracionEvent))
                {
                  if (configuracionEvent.type == sf::Event::Closed ||
                      (configuracionEvent.type == sf::Event::KeyPressed && configuracionEvent.key.code == sf::Keyboard::Escape))
                  {
                    if( _configuracion->getMusic()) main_theme_v2.play();
                    windowConfiguracion.close();
                  }

                  if (configuracionEvent.type == sf::Event::KeyReleased) {
                    if (configuracionEvent.key.code == sf::Keyboard::Up) {
                      menuConfiguracion.moveUp();
                    } else if (configuracionEvent.key.code == sf::Keyboard::Down) {
                      menuConfiguracion.moveDown();
                    } else if (configuracionEvent.key.code == sf::Keyboard::Return) {
                      if (menuConfiguracion.getShowInsertCoin()) {
                        menuConfiguracion.handleEnterPress();
                      } else {
                        int x = menuConfiguracion.getPressedItem();
                        if (x == 0)
                        {
                          _configuracion->setSoundEffects(!_configuracion->getSoundEffects()); // pongo el contrario de la configuracion
                          _configuracion->cargarConfiguracion(_configuracion->getSoundEffects(),_configuracion->getMusic()); // preparo el reg
                          ca.grabarConfiguracion(*_configuracion); // grabo el reg
                          menuConfiguracion.setConfiguracion(_configuracion);
                          pew.playDisparoPew();




                        } else if (x == 1)
                        {
                          _configuracion->setMusic(!_configuracion->getMusic()); // pongo el contrario de la configuracion
                          _configuracion->cargarConfiguracion(_configuracion->getSoundEffects(),_configuracion->getMusic()); // preparo el reg
                          ca.grabarConfiguracion(*_configuracion); // grabo el reg



                          if (_configuracion->getMusic()) {
                            main_theme_v2.play();
                          } else {
                            main_theme_v2.stop();
                          }

                        }
                        else if (x == 2)
                        {
                          // TODO: salir
                          windowConfiguracion.close();
                        }



                      }

                    }
                  }





                }
                // CMD
                // UPDATE
                menuConfiguracion.update();
                // DRAW
                windowConfiguracion.clear();
                windowConfiguracion.draw(menuConfiguracion);
                windowConfiguracion.display();
                // FLIP
              }

            }
            else if (x == 2) // RANKING
            {
              main_theme_v2.stop();
              if( _configuracion->getMusic()) nostalgia.play();
              sf::RenderWindow windowRanking(sf::VideoMode(600, 800), "Ranking");
              MenuRanking menuRanking;
              windowRanking.setFramerateLimit(60);

              while (windowRanking.isOpen()) {

                sf::Event menuRankingEvent;

                while(windowRanking.pollEvent(menuRankingEvent)) {

                  if (menuRankingEvent.type == sf::Event::Closed ||
                      (menuRankingEvent.type == sf::Event::KeyPressed && menuRankingEvent.key.code == sf::Keyboard::Escape)) {
                    nostalgia.stop();
                    if( _configuracion->getMusic()) main_theme_v2.play();
                    windowRanking.close();
                  }
                }

                // Actualizar el estado de MenuRanking
                menuRanking.update();

                // Limpiar, dibujar y mostrar el contenido en pantalla
                windowRanking.clear();
                windowRanking.draw(menuRanking);
                windowRanking.display();
              }
            }
            else if (x == 3) // CREDITOS
            {
//
              sf::RenderWindow CREDITOS(sf::VideoMode(600, 800), "CREDITOS");
              Creditos creditosObj;

              while (CREDITOS.isOpen()) {

                sf::Event optionsEvent;

                while (CREDITOS.pollEvent(optionsEvent)) {

                  if (optionsEvent.type == sf::Event::Closed ||
                      (optionsEvent.type == sf::Event::KeyPressed && optionsEvent.key.code == sf::Keyboard::Escape)) {
                    CREDITOS.close();
                  }
                }

                creditosObj.update();

                CREDITOS.clear();
                CREDITOS.draw(creditosObj);
                CREDITOS.display();
              }

            }
            else if (x == 4) // CIERRA
            {
              window.close();

            }


          }
        }
      }
    }
//
    mainMenu.update();
    window.clear();
    window.draw(mainMenu);
    window.display();
//
  }

  delete [] _configuracion;

  return 0;
}
