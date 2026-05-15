#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "Common.h"
#include "Entity.h"

#define BULLET_ALLY_MAX 5
#define MISTEAL_ALLY_MAX 6

#define BULLET_ENEMY_MAX 20
#define MAX_BOSS_MISSILE 4

#define BULLET_SPEED_ALLY 300
#define MISTEAL_SPEED BULLET_SPEED_ALLY * 1.2f

#define MISTEAL_ROTATION 5.f
#define BULLET_SPEED_ENEMY 150
#define BULLET_LIFETIME 2.5f

#define MISTEAL_TIMER_GENERAL 2.f
#define MISTEAL_TIMER_IN_WALL MISTEAL_TIMER_GENERAL * 10.f

#define BOSS_MISSILE_SPEED 400.f
#define BOSS_MISSILE_SPAWN_TIME 1.f
#define BOSS_MISSILE_AIR_TIME 3.f
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

#define GRENADE_LIFETIME 3.f
#define GRENADE_FALL_SPEED 200.f

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

typedef enum BossMissileState
{
	BOSS_MISSILE_SPAWNING,
	BOSS_MISSILE_ASCENDING,
	BOSS_MISSILE_STASIC,
	BOSS_MISSILE_FALLING,
	BOSS_MISSILE_OUT
}BossMissileState;

typedef struct BossMissile
{
	sfSprite* sprite;
	sfMusic* music;

	sfVector2f velocity;
	float destination;

	BossMissileState bossMissileState;
	float bossMissileTimer;

}BossMissile;

typedef struct DangerZone
{
	sfSprite* sprite;
	sfBool isShown;
	sfBool doBlink;
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

typedef struct Grenade
{
	sfSprite* sprite;
	sfVector2f velocity;
	float sizeGrenade;
	float rangeGrenade;
	float lifetime;
}Grenade;

typedef struct UnhiddingBomb
{
	sfSprite* sprite;
	sfVector2f velocity;
	float droptiming;
	float lifetime;
	sfBool isGrounded;
}UnhiddingBomb;

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

typedef struct UnhiddingExplosion
{
	sfSprite* sprite;
	Animation unhiddingExplosionAnim;
	sfBool isOn;
}UnhiddingExplosion;

void LoadProjectiles(float _groundlvl);
void LoadGrenade(void);
void LoadSecondary(void);
void LoadBossProjectiles(void);
void LoadDangerZone(void);
void LoadExplosion(void);
void LoadUnhiddingExplosion(void);

void UpdateProjectiles(sfVector2f _posAim, float _dt);
void UpdateMisteal(float _dt);
void UpdateBossMissile(float _dt);
void UpdateSecondary(sfVector2f _pos, float _dt);
void UpdateDrone(sfVector2f _playerPos, float _dt);
void UpdateColdBreath(float _dt);
void UpdateExplosion(float _dt);
void UpdateUnhiddingExplosion(float _dt);
void UpdateGrenade(float _dt);

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddMisteal(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void AddDrone(sfVector2f _pos, sfBool _isRighted);
void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);

void SpawnBossMissile(sfVector2f _posShooter, float _destination);
void AddDangerZone(sfVector2f _destination, unsigned _index);

void SpawnExplosion(sfVector2f _explosionZone, sfBool _isAlly, float _range);
void SortExplosionList(unsigned _index);

void SpawnGrenade(sfVector2f _spawnZone, float _sizeGrenade, float _rangeGrenade);
void SortGrenadeList(unsigned _index);

void DeleteBulletAlly(unsigned _index);
void DeleteBulletEnemy(unsigned _index);
void DeleteMisteal(unsigned _index);
void DeleteBossMissile(unsigned _index);
void DeleteDangerZone(unsigned _index);
void DeleteGrenade(unsigned _index);

void MoveDrone(unsigned _index, sfVector2f _playerPos, float _dt);

void CheckCollisionDronesList(void);
void TestCollisionExplosionList(unsigned _index, float _range);
void TestCollisionUnhiddingExplosion(void);

sfBool HitBossMissile(sfFloatRect _hitbox);
sfBool HitGrenade(sfFloatRect _hitbox);

unsigned GetBulletCount(void);
unsigned GetMistealCount(void);
unsigned GetExplosionCount(void);

#endif // !PROJECTILES_H