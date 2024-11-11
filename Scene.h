// Scene.h

#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyBullet.h"

// Declaración anticipada de Gameplay (forward declaration), porque si se incluye Gameplay.h (#include "Gameplay.h"
//    se genera una referencia circular.
class Gameplay;
class Player;
class Bullet;
class Enemy;

class Scene : public sf::Drawable
{
public:

    // nuevas funciones virtuales que heredan los niveles:
    virtual void start() = 0;

    virtual void cmd() = 0;

    virtual void update() = 0;

    virtual void render(sf::RenderWindow &window) = 0;

    virtual Player* getPlayer() = 0;
    virtual Enemy* getEnemy() = 0;
    virtual std::vector<Bullet*>& getBullets() = 0;  // Método virtual que devuelve balas en la escena
    virtual std::vector<EnemyBullet*>& getEnemyBullets() = 0;  // Método virtual que devuelve balas en la escena
    virtual std::vector<Enemy*> getEnemies() = 0;  // Método virtual que devuelve enemigos en la escena
    virtual int getFrames() const = 0;

protected:

private:
};
