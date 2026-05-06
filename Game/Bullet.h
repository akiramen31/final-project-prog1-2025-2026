#ifndef BULLET_H
#define BULLET_H

#include "Common.h"
#include "Box.h"

#define BULLET_ALLY_MAX 5
#define MISTEAL_ALLY_MAX 6

#define BULLET_ENEMY_MAX 20
#define MAX_BOSS_DRONE 4

#define BULLET_SPEED_ALLY 300
#define MISTEAL_SPEED BULLET_SPEED_ALLY * 1.2f

#define MISTEAL_ROTATION 5.f
#define BULLET_SPEED_ENEMY 250
#define BULLET_LIFETIME 2.5f

#define MISTEAL_TIMER_OUTMAP_NOTSTICKED 2.f
#define MISTEAL_TIMER_STICKED MISTEAL_TIMER_OUTMAP_NOTSTICKED * 10.f

#define BOSS_DRONE_SPEED 400.f
#define BOSS_DRONE_SPAWN_TIME 1.f
#define BOSS_DRONE_IN_SKY_TIME 3.f

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

typedef enum BossDroneState
{
	BDRONE_IS_SPAWNING,
	BDRONE_IS_ASCENDING,
	BDRONE_IS_STASIC_IN_SKY,
	BDRONE_IS_FALLING,
	BDRONE_IS_OUT
}BossDroneState;

typedef struct BossDrone
{
	sfSprite* sprite;
	sfMusic* music;

	sfVector2f velocity;
	float destination;

	BossDroneState bossDroneState;
	float bossDroneTimer;

}BossDrone;

typedef struct DangerZone
{
	sfSprite* sprite;
	sfBool isShowed;
}DangerZone;

void LoadProjectiles(void);
void LoadBossDrone(void);
void LoadDangerZone(void);

void UpdateProjectiles(float _dt);
void UpdateMisteal(float _dt);
void UpdateBossDrone(float _dt);

unsigned int GetBulletCount(void);
unsigned GetMistealCount(void);

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddMisteal(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);

void AddBossDrone(sfVector2f _posShooter, float _destination);

void DeleteBulletAlly(unsigned _index);
void DeleteBulletEnemy(unsigned _index);
void DeleteMisteal(unsigned _index);
void DeleteBossDrone(unsigned _index);

sfBool HitBossDrone(sfBool _destroy, sfFloatRect _hitbox);

#endif // !BULLET_H