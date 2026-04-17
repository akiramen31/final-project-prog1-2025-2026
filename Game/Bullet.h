#ifndef BULLET_H
#define BULLET_H

#include "Common.h"
#include "Box.h"

#define BULLET_ALLY_MAX 5

#define BULLET_ENEMY_MAX 20

#define BULLET_SPEED_ALLY 500
#define BULLET_SPEED_ENEMY 500
#define BULLET_LIFETIME 10.f

typedef struct ShooterType
{
	float weaponPos;
	sfVector2f shootPosition;
	AttackType bulletType;
	sfBool isAlly;
	sfBool isRighted;
}ShooterType;

typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
	sfBool isAlly;
	AttackType bulletType;
}Bullet;

void LoadBullet(void);
void UpdateBullet(float _dt);

unsigned int GetBulletCount(void);
void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void DeleteBulletAlly(unsigned _index);
void DeleteBulletEnemy(unsigned _index);
#endif // !BULLET_H
