#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include "Scene.h"
#include "Player.h"
#include "Bullet.h"
#include "Menu.h"
#include "GamePlayOld.h"
#include "Creditos.h"
#include "RankingArchivo.h"
#include "Ranking.h"
#include "Game.h"

int main() {

  std::srand((unsigned)std::time(0));
  Game game;
  game.run();


    sf::RenderWindow window(sf::VideoMode(600, 800), "MENU 1942", sf::Style::Default);

    Menu mainMenu(window.getSize().x, window.getSize().y);

    window.setFramerateLimit(60);



    // Definir la vista
    sf::View view(sf::FloatRect(0.f, 0.f, 600.f, 800.f));
    window.setView(view);

//    GamePlayOld gamePlay;
    Scene scene;

//  sf::SoundBuffer buffer_main_theme_v2;
//  buffer_main_theme_v2.loadFromFile("assets/sounds/themes/02_main_theme_v2_stereo.mp3");
//  sf::Sound main_theme_v2;
//  main_theme_v2.setBuffer(buffer_main_theme_v2);
//  main_theme_v2.setVolume(7.f);
//  main_theme_v2.play();


//  sf::SoundBuffer buffer_main_theme_v1;
//  buffer_main_theme_v1.loadFromFile("assets/sounds/themes/01_main_theme_v1_stereo.mp3");
//  sf::Sound main_theme_v1;
//  main_theme_v1.setBuffer(buffer_main_theme_v1);
//  main_theme_v1.setVolume(7.f);



//    Player player;

//    sf::Font font;
//    font.loadFromFile("assets/fonts/MONOCOQUE_FUENTE.ttf");

//    sf::Text puntos;
//    puntos.setFont(font);
//    puntos.setPosition(3, 0);

//    sf::Text vidas;
//    vidas.setFont(font);
//    vidas.setPosition(3, 30);

    // GAME LOOP
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
//                main_theme_v2.stop();
//                main_theme_v1.stop();
                window.close();
            }



            if (event.type == sf::Event::KeyReleased) {

                if (event.key.code == sf::Keyboard::Up) {
                    mainMenu.moveUp();

                } else if (event.key.code == sf::Keyboard::Down) {
                    mainMenu.moveDown();

                } else if (event.key.code == sf::Keyboard::Return) {

                    if (mainMenu.showInsertCoin) {
                        mainMenu.handleEnterPress();

                    } else {

                        int x = mainMenu.getPressedItem();

                        if (x == 0) {

                            sf::RenderWindow Play(sf::VideoMode(600, 800), "1942");
                            Play.setFramerateLimit(60);
                            window.close();
//                            main_theme_v2.stop();
//                            main_theme_v1.play();


                            // GAME LOOP

                            while (Play.isOpen()) {

                                // EVENTS

                                sf::Event playEvent;

                                while (Play.pollEvent(playEvent)) {

                                    if (playEvent.type == sf::Event::Closed ||
                                        (playEvent.type == sf::Event::KeyPressed && playEvent.key.code == sf::Keyboard::Escape)) {
//                                      main_theme_v1.stop();
                                      Play.close();
                                    }

                                }

                                // CMD
                                 scene.cmd();


                                // UPDATE
//                                gamePlay.update();
                                scene.update();

//                                if (gamePlay.isCollisionWithEnemy()) {
//                                    player.changePuntos(1);
//                                }
//
//                                if (gamePlay.isCollisionWithPersonaje()) {
//                                    player.changeVidas(10);
//                                }
//
//                                puntos.setString("PUNTOS " + std::to_string(player.getPuntos()));
//                                vidas.setString("VIDA " + std::to_string(player.getVida()));


                                // DRAW
                                Play.clear();
                                Play.draw(scene);
//                                Play.draw(gamePlay);
                                //Play.draw(puntos);
                                //Play.draw(vidas);

                                // FLIP
                                Play.display();

                                // LIBERACION DEL JUEGO


                            }

                        } else if (x == 1) {

                            sf::RenderWindow CREDITOS(sf::VideoMode(600, 820), "CREDITOS");
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

                        } else if (x == 2) {
                            window.close();

                        }
                    }
                }
            }
        }
//         main_theme_v1.stop();
        mainMenu.update();
        window.clear();
        window.draw(mainMenu);
        window.display();




    }



    return 0;
}



