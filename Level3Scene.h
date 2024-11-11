// Level3Scene.h

#pragma once

#include <iostream>
#include "Scene.h" // los niveles hereda de Scene
#include "SceneManager.h" // los niveles hereda de Scene

class Level3Scene : public Scene
{
public:

    Level3Scene():
            _frameExplosion(0.f),
            _isExplosionActive(false),
            _frames(0)
    {  _bg.loadFromFile("assets/sprites/bg-maps-1942.png");
      _bgSprite.setTexture(_bg);
      _bgSprite.setPosition(0, -5051 + 800);

      _player = new Player();
      _enemy1 = new Enemy();

      _explosion = new Explosion();

    }

    void start() override
    {
//      std::cout << "Level 2 Started!" << std::endl;
      //Display de vidas y puntaje
      _font1.loadFromFile("assets/fonts/MONOCOQUE_FUENTE.ttf");
      _puntos.setFont(_font1);
      _puntos.setPosition(3, 0);
      _vidas_text.setFont(_font1);
      _vidas_text.setPosition(3, 30);
      _font2.loadFromFile("assets/fonts/monofonto_rg.otf");
      _frames_cartel.setFont(_font2);
      _frames_cartel.setPosition(500, 0);
      _frames_cartel.setCharacterSize(14);
    }

    void cmd() override
    {
    }

    void update() override
    {
      _bgSprite.move(0, 2.3f);
      _puntos.setString("PUNTOS " + std::to_string(Gameplay::getInstance()->getPuntos()));
      _vidas_text.setString("VIDA " + std::to_string(Gameplay::getInstance()->getVidas()));
      _frames_cartel.setString("Frames= " + std::to_string(_frames) + "\nTiempo= " + std::to_string(_frames / 60));

      if (_bgSprite.getPosition().y > 0) {
        respawnBackground(); // TODO en lugar de respawn termina el nivel y el booleano levelComplete se pone en true
      } // Funcion que hace que el fondo se traslade hacia abajo

      _player->cmd();   // Actualiza cmd player
      _player->update(); // Actualiza posicion player
      _enemy1->update(); // Actualiza cmd enemigo1
      _enemy1->cmd(); // Actualiza posicion enemigo1

      for (auto* bullet: _bullets) {
        bullet->update();
      } // Actualiza Player->Balas

      for (auto* enemyBullet: _enemyBullets) {
        enemyBullet->update();
      } // Actualiza Player->Balas

      _frames++;
    }


    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {

      target.draw(_bgSprite, states); // Dibuja fondo

      if (!_bullets.empty()) {
        for (const Bullet* bullet : _bullets) {
          bullet->draw(target, states);  // Dibuja cada bala
        }
      }// Dibuja balas propias

      if (!_enemyBullets.empty()) {
        for (const EnemyBullet* enemyBullet : _enemyBullets) {
          enemyBullet->draw(target, states);  // Dibuja cada bala
        }
      }// Dibuja balas enemigas



      target.draw(_frames_cartel, states);
      _player->draw(target, states);
      _enemy1->draw(target, states);
      _explosion->draw(target, states);

      target.draw(_puntos, states);
      target.draw(_vidas_text, states);



    }


    void render(sf::RenderWindow &window) override
    {
      window.clear(sf::Color::Black);
      window.draw(*this);
    }

    sf::Vector2f getCameraPosition()
    {
      return _bgSprite.getPosition();
    }


    void respawnBackground()
    {
      _bgSprite.setPosition({0, -5051 + 800});
    }

    Player* getPlayer()  override
    {
      return _player;
    }

    Enemy* getEnemy() override
    {
      return _enemy1;
    }

    std::vector<Bullet*>& getBullets() override  {
//      std::cout << _bullets.size() << std::endl;
      return _bullets;
    }

    std::vector<EnemyBullet*>& getEnemyBullets() override  {
      return _enemyBullets;
    }

    std::vector<Enemy*> getEnemies() override {
      return _enemies;  // Devolver los enemigos específicos de Level1
    }

    int getFrames() const override
    {
//        std::cout << *_ptrFrames << std::endl;
//      return *_ptrFrames;
      return 1;
    }

private:
    Player* _player;
    Enemy* _enemy1;
    std::vector<Bullet*> _bullets;
    std::vector<EnemyBullet*> _enemyBullets;


    Explosion* _explosion;
    std::vector<Enemy *> _enemies;
    sf::Text _puntos;
    sf::Font _font1;
    sf::Font _font2;
    sf::Text _vidas_text;
    sf::Text _frames_cartel;
    sf::Texture _bg;
    sf::Sprite _bgSprite;
    bool levelComplete;
    int _frames;
    int *_ptrFrames;
    float _frameExplosion;
    bool _isExplosionActive;

};
