#ifndef COLLISION_H
#define COLLISION_H

#include "Common.h"

typedef struct CircleHitbox
{
	sfVector2f position;
	float radius;
}CircleHitbox;

sfBool IsCollisionCircleCircle(CircleHitbox _hitbox1, CircleHitbox _hitBox2);

#endif // !COLLISION_H
