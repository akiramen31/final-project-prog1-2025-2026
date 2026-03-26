#include "Collision.h"

sfBool IsCollisionCircleCircle(CircleHitbox _hitbox1, CircleHitbox _hitBox2)
{
    return GetDistance(_hitbox1.position, _hitBox2.position) < _hitbox1.radius + _hitBox2.radius;
}
