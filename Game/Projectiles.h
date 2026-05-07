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
#define BOSS_EXPLOSION_RANGE 3.f
#define BOSS_SELF_DAMAGE 50.f

#define PLAYER_DRONE_WEIGHT 8
#define PLAYER_DRONE_DAMAGE 10.f
#define PLAYER_DRONE_MAX 3

#define SECONDARY_PROJECTILE_DURATION 3

#define SPEED_PLAYER_DRONE 180.0f
#define SPEED_COLDBREATH 100.f
#define DEGRE_ROTATION 300.0f

#define EXPLOSION_FRAME_DURATION 0.1f
#define EXPLOSION_BASIC_RANGE 12.f

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
	sfBool isBlink;
}DangerZone;

typedef struct Drone
{
	sfSprite* sprite;
	sfMusic* ambientSound;
	float rotation;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Drone;

typedef struct ColdBreath
{
	sfSprite* sprite;
	sfMusic* music;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
}ColdBreath;

typedef struct Explosion
{
	sfSprite* sprite;
	Animation explosionAnim;
	sfBool isAlly;
	float range;
}Explosion;

void LoadProjectiles(float _groundlvl);
void LoadSecondary(void);
void LoadBossDrone(void);
void LoadDangerZone(void);
void LoadExplosion(void);

void UpdateProjectiles(sfVector2f _posAim, float _dt);
void UpdateMisteal(float _dt);
void UpdateBossDrone(float _dt);
void UpdateSecondary(sfVector2f _pos, float _dt);
void UpdateDrone(sfVector2f _playerPos, float _dt);
void UpdateColdBreath(float _dt);
void UpdateExplosion(float _dt);

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddMisteal(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddDrone(sfVector2f _pos, sfBool _isRighted);
void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);

void AddBossDrone(sfVector2f _posShooter, float _destination);
void AddDangerZone(sfVector2f _destination, unsigned _index);

void SpawnExplosion(sfVector2f _explosionZone, sfBool _isAlly, float _range);
void SortExplosionList(unsigned _index);

void DeleteBulletAlly(unsigned _index);
void DeleteBulletEnemy(unsigned _index);
void DeleteMisteal(unsigned _index);
void DeleteBossDrone(unsigned _index);
void DeleteDangerZone(unsigned _index);

void MoveDrone(unsigned _index, sfVector2f _playerPos, float _dt);

void CheckCollisionMissilesList(void);
void TestCollisionExplosionList(unsigned _index, float _range);

sfBool HitBossDrone(sfBool _destroy, sfFloatRect _hitbox);

unsigned int GetBulletCount(void);
unsigned GetMistealCount(void);
unsigned GetExplosionCount(void);

#endif // !BULLET_H