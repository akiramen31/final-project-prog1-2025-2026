#ifndef BULLET_H
#define BULLET_H

#include "Common.h"
#include "Box.h"

#define BULLET_ALLY_MAX 5
#define MISTEAL_ALLY_MAX 6

#define BULLET_ENEMY_MAX 20

#define BULLET_SPEED_ALLY 300
#define MISTEAL_SPEED BULLET_SPEED_ALLY * 1.2f

#define MISTEAL_ROTATION 5.f
#define BULLET_SPEED_ENEMY 250
#define BULLET_LIFETIME 2.5f

#define MISTEAL_TIMER_OUTMAP_NOTSTICKED 2.f
#define MISTEAL_TIMER_STICKED MISTEAL_TIMER_OUTMAP_NOTSTICKED * 10.f

typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
	sfBool isAlly;
	AttackType bulletType;
}Bullet;

typedef struct Misteal
{
	sfSprite* sprite;
	sfVector2f velocity;
	sfBool isSticked;
	sfBool isAlreadyHit;

	float timer;
}Misteal;

void LoadBullet(void);
void UpdateBullet(float _dt);
void UpdateMisteal(float _dt);

unsigned int GetBulletCount(void);
unsigned GetMistealCount(void);
void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddMisteal(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void DeleteBulletAlly(unsigned _index);
void DeleteBulletEnemy(unsigned _index);
void DeleteMisteal(unsigned _index);
#endif // !BULLET_H