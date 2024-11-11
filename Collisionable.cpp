#include "Collisionable.h"
bool Collisionable::isCollision(Collisionable &col) const
{
  return getBounds().intersects(col.getBounds());
}
