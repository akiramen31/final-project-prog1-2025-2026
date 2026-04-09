#ifndef BULLET_H
#define BULLET_H

#include "Common.h"

#define BULLET_MAX 5
#define BULLET_SPEED 700

typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
	sfBool isAlive;
}Bullet;

void LoadBullet(void);
void UpdateBullet(float _dt);

unsigned int GetBulletCount(void);
void AddBullet(sfVector2f _pos);

#endif // !BULLET_H
