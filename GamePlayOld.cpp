#include "GamePlayOld.h"
#include <iostream>
#include <cmath>


GamePlayOld::GamePlayOld() {
    _timerReload = 0;
    _frameExplosion  = 0.f;
    _isExplosionActive = false;
    _frames = 0;
}

void GamePlayOld::cmd() {
  _player.cmd();
  enemigo1.cmd();
//  for (EnemyB *enemyb: _vEnemiesB) _vEnemiesB->cmd();
}

void GamePlayOld::update()
{
  float deltaTime = clock.restart().asSeconds();

  // Oleada aviones enemigo1

  if (_frames / 60 < 30) {
    enemigo1.update();
  }

  // Oleadas enemy B

  if (_frames % 60 == 0.f) {
    sf::Vector2f direccion_a_normalizar = enemigo1.getPosition() - _player.getPosition();
    float hipotenusa = sqrt(direccion_a_normalizar.x * direccion_a_normalizar.x + direccion_a_normalizar.y + direccion_a_normalizar.y);
    sf::Vector2f direccion_final = {direccion_a_normalizar.x / hipotenusa, direccion_a_normalizar.y / hipotenusa};
    _enemyBullets.push_back(new EnemyBullet(enemigo1.getBulletOrigin(), direccion_final, 3.5f));
  }




  for (Bullet *bullet: _bullets) bullet->update(); // UPDATE BALAS PROPIAS
  _timerReload--; // RELOAD DE TIMER PARA DISPARAR
  for (EnemyBullet *enemyBullet: _enemyBullets) enemyBullet->update(); // UPDATE BALAS ENEMY
  for (int i = 0; i < _bullets.size(); i++)
  { if (_bullets[i]->getBounds().top + _bullets[i]->getBounds().height < 0) {
      delete _bullets[i];
      _bullets.erase(_bullets.begin() + i, _bullets.begin() + i + 1);
    }
  } // DELETE BALAS PROPIAS FUERA DE PANTALLA
  for (int i = 0; i < _enemyBullets.size(); i++) {
    if (_enemyBullets[i]->getBounds().top + _enemyBullets[i]->getBounds().height > 800) {
      delete _enemyBullets[i];
      _enemyBullets.erase(_enemyBullets.begin() + i);
    }
  } // DELETE BALAS ENEMIGAS FUERA DE PANTALLA


  if (_isExplosionActive) {
    _explosion.smallExplosion(_frameExplosion);

    if (_frameExplosion >= 8.f) {
      _isExplosionActive = false;
      _frameExplosion = 0.0f;
    }
  } // EXPLOSIONES

  for (int i = 0; i < _vec_isExplosiveActive.size(); i++) {
    if (*_vec_isExplosiveActive[i] == true) {

      _vExplosiones[i]->smallExplosion(*_vec_frameExplosive[i]);

      if (*_vec_frameExplosive[i] >= 8.f) {

        delete _vec_isExplosiveActive[i];
        _vec_isExplosiveActive.erase(_vec_isExplosiveActive.begin() + i);

        delete _vec_frameExplosive[i];
        _vec_frameExplosive.erase(_vec_frameExplosive.begin() + i);

        delete _vExplosiones[i];
        _vExplosiones.erase(_vExplosiones.begin() + i);
      }
    }
  } /// RECORREMOS LAS EXPLOSIONES DINAMICAS


//  if (isCollisionWithPersonaje() || (isCollision_vBullestEnemyB_whitPersonaje())) {
//    _juego.changeVidas();
//    _player.respawn();
//  } // COLISION BULLETS ENEMYB CON PLAYER
//
//  if (isCollisionWithEnemy() || (isCollision_bullets_whitEnemyB())){
//    _juego.changePuntos(100);
//  } // COLISION

//  _powerUp->update();
  _player.update();


  if (checkCollision(enemigo1)) {
    _juego.changeVidas();
    _player.respawn();
    enemigo1.respawn();
  } // COLISION ENEMIGO 1 CON PLAYER

  _frames++;



}



bool GamePlayOld::checkCollision(const Enemy& col) const
{
    return _player.getBounds().intersects(col.getBounds());
}


/// SI LA BALA DEL VECTOR ENEMIGOS B COLISIONA CON EL PERSONAJE.
//bool GamePlayOld::isCollision_vBullestEnemyB_whitPersonaje()
//{
//    bool result = false;
//
//    for ( int i = 0 ; i < _bullets_vEnemyB.size() ; i++)
//    {
//        if (_bullets_vEnemyB[i]->isCollision(_player))
//        {
//            std::cout << "COLIION , VECTOR BALAS B CON PERSONAJE " << std::endl;
//            delete _bullets_vEnemyB[i];
//            _bullets_vEnemyB.erase(_bullets_vEnemyB.begin()+i);
//            result = true;
//        }
//    }
//
//    return result;
//}


bool GamePlayOld::isCollisionWithPersonaje() // cuando te disparan
{
    bool result = false;
    for(int i=0; i<_enemyBullets.size(); i++)
    {
        if (_enemyBullets[i]->isCollision(_player))
        {
          delete _enemyBullets[i];
          _enemyBullets.erase(_enemyBullets.begin()+i);
            result = true;
        }
    }
    return result;
}

bool GamePlayOld::isCollisionWithEnemy() // cuando destruis aviones enemigos
{
    bool result = false;
//    for ( int i = 0 ; i < _bullets.size() ; i++ )
//    {
//        if(_bullets[i]->isCollision(enemigo1))
//        {
////            _sound.playExplosionSmall();
//            _juego.changePuntos(100);
//            delete _bullets[i];
//            _bullets.erase(_bullets.begin()+i);
//
//            _explosion =  Explosion(enemigo1.getPosition().x,enemigo1.getPosition().y);
//            _isExplosionActive = true;  // ACTIVA LA ANIMACION
//            _frameExplosion = 0.0f;     // ESTAMOS EN EL PRIMER FRAME
//
//            enemigo1.respawn();
//            result = true;
//
//        }
//    }
    return result;
}

//bool GamePlayOld::isCollision_bullets_whitEnemyB()
//{
//
//    /// RECORRO LAS BALAS QUE DISPARO EL PERSONAJE
//    for ( int i = 0 ; i < _bullets.size() ; i++ )
//    {
//        /// RECORREMOS TODOS LOS ENEMIGOS B EN PANTALLA
//        for ( int j = 0 ; j < _vEnemiesB.size() ; j++ )
//        {
//            if (_bullets[i]->isCollision(*_vEnemiesB[j]))
//            {
//                std::cout << "COLLISION -> BULLEST PERSONAJE CON ENEMIGO B" << std::endl;
//                delete _bullets[i];
//                _bullets.erase(_bullets.begin()+i);
//
//                _vExplosiones.push_back(new Explosion(_vEnemiesB[j]->getPosition().x -50, _vEnemiesB[j]->getPosition().y-50));
//                _vec_isExplosiveActive.push_back(new bool(true)) ;
//                _vec_frameExplosive.push_back( new float(0.0f));
//
//                _vEnemiesB[j]->respawn();/// TODO: SE TIENE QUE ELIMINAR.
//
//                return true;
//
//            }
//        }
//    }
//
//    return false;
//
//}


void GamePlayOld::draw(sf::RenderTarget &target, sf::RenderStates states)const
{

    for(int i=0; i < _bullets.size(); i++)
    {
        target.draw(*_bullets[i], states);
//        target.draw(*bala_cartel[i], states);
    }

    for(int i=0; i<_enemyBullets.size(); i++)
    {
        target.draw(*_enemyBullets[i], states);
    }


//    for ( int i = 0 ; i<_vEnemiesB.size() ; i++)
//    {
//        target.draw(*_vEnemiesB[i],states);
//    }

//    for (int i = 0; i < _bullets_vEnemyB.size(); i++)
//    {
//        target.draw(*_bullets_vEnemyB[i],states);
//    }


//    for ( int i = 0 ; i < _vExplosiones.size() ; i++)
//    {
//        target.draw(*_vExplosiones[i],states);
//    }

//    target.draw(*_powerUp,states);
    target.draw(_player,states);
    target.draw(_explosion,states);
    target.draw(enemigo1,states);
//    target.draw(enemigo1_cartel,states);
//    target.draw(player_cartel,states);

//    for(Enemy *enemy: _enemies)
//    {
//      target.draw(_enemy);
//    }

}

int GamePlayOld::getPuntos() const
{
  return _juego.getPuntos();
}

int GamePlayOld::getVidas() const
{
  return _juego.getVida();
}

int GamePlayOld::getFrames() const
{
  return _frames;
}

GamePlayOld::~GamePlayOld()
{
  _sound.stopAllSounds();


}


//int GamePlayOld::getPuntos(Player _juego) const
//{
//  return _juego.getPuntos();
//}
//
//int GamePlayOld::getVidas(Player _juego) const
//{
//  return _juego.getVida();;
//}



