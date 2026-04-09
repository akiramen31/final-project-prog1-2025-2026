#ifndef BULLET_H
#define BULLET_H

#include "Common.h"

#define BULLET_MAX 5
#define BULLET_SPEED 500
#define BULLET_LIFETIME 2.f


typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
}Bullet;

void LoadBullet(void);
void UpdateBullet(float _dt);

unsigned int GetBulletCount(void);
void AddBullet(sfVector2f _posPlayer, sfVector2f _posAim);

#endif // !BULLET_H
