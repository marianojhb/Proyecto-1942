#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Avion.h"
#include "Player.h"
#include "Personaje.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Explosion.h"
#include "EnemyB.h"
#include "Sounds.h"
#include "PowerUp.h"
#include "Boss.h"
#include "BossBullets.h"
#include "Configuracion.h"
#include "ConfiguracionArchivo.h"

class GamePlay : public sf::Drawable
{
public:
    GamePlay();
    ~GamePlay();

    void cmd();

    void update();

    bool isCollisionWithPersonaje();

    bool isCollisionWithEnemy();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    int getPuntos() const;

    int getVidas() const;

    bool checkCollision(const Enemy &enemigo1) const;

    int getFrames() const;

    bool isCollision_vBullestEnemyB_whitPersonaje();

    bool isCollision_bullets_whitEnemyB();

    bool isCollision_withPowerUp();

    bool isCollision_vEnemyB_withPersonaje();

    bool isCollision_WithBoss();
    bool isCollision_bulletsBoss_withPersonaje();
    //bool isCollisionWithBoss();
    void restartVidas();
    bool getNivelTermiando()const;


private:

    bool _nivelTerminado;
    void iniciarBalasVector();

    Personaje _player;
    std::vector<Bullet *> _bullets;
    std::vector<EnemyBullet *> _enemyBullets;
    Enemy enemigo1;
    ///JEFE:
        Boss _boss;
        bool _banderaApareceBoss;
        bool _banderaAtacanAlBoss;
        float _frameAtackanAlBoss;
        float _frameExplosionBoss;
        bool _bandera_startBossBullest;
        int _ctoCollisiones_withBoss;
        float _frame_MuerteBoss;
        bool _bandera_BossMuerto;
        std::vector<BossBullets *> _bullets_Boss;// BALASJEFE
    ///

    std::vector<Enemy *> _enemies;
    int _timerReload;
    Player _juego;

    Explosion _explosion;
    float _frameExplosion;
    bool _isExplosionActive;
    int _frames;
    std::vector<EnemyB *> _vEnemiesB;
    std::vector<Bullet *> _bullets_vEnemyB;
    Sounds _sound;

    std::vector<Explosion *> _vExplosiones; /// SI HAY EXPLISION
    std::vector<bool *> _vec_isExplosiveActive; /// VERIFICAR SI ESTA EN VERDADERO
    std::vector<float *> _vec_frameExplosive; /// LOS FRAMES PARA MANDERSELO A LA FUNCION

    PowerUp *_powerUp = nullptr;
    sf::Clock respawn_delay;
    bool _player_down;
    bool _isExplosionActiveBoss;
    bool _soundfx;
    Configuracion _configuracion;

};
