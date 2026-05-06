#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#define BOSS_SPEED PLAYER_HORIZONTAL_SPEED_MAX * 0.8f
#define BOSS_SPEED_RUNAWAY BOSS_SPEED * 2.5f

#define OFFSET_BOSS (sfVector2f){56, -46}

#define BOSS_PART_NUMBER 10

#define RUNAWAY_TIMER 4.f

#define BOSS_FIRERATE_BULLET 1.f
#define BOSS_FIRERATE_DRONE 1.f
#define MAX_BOSS_LIFE 250.f

#define TURRET_ROTATION_SPEED 150.f

#define SHOOT_DISTANCE_MAX 300.f
#define SHOOT_DISTANCE_MIN 80.f

#define ARENA_CENTER 8671.f
#define ARENA_ENTRY 8290.f

#define ARENA_LIMITE_LEFT 8451.f
#define ARENA_LIMITE_RIGHT 8891.f

typedef enum PlayerPositionToBoss
{
	PLAYER_NOT_IN_ARENA,
	PLAYER_AWAY_RIGHT,
	PLAYER_AWAY_LEFT,
	PLAYER_RANGE_SHOOT_RIGHT,
	PLAYER_RANGE_SHOOT_LEFT,
	PLAYER_UNDER,
	PLAYER_ON_TOP,
	PLAYER_TURRET_RIGHT,
	PLAYER_TURRET_LEFT

}PlayerPositionToBoss;

typedef enum BossReactionToPlayer
{
	NONE,
	GO_LEFT,
	GO_RIGHT,
	PUSH_LEFT,
	PUSH_RIGHT
}BossReactionToPlayer;

typedef struct Boss1
{
	sfSprite* track;
	sfSprite* steamTank;
	sfSprite* gunCariage;
	float timerCanon;
	sfSprite* spriteTurretLCase;
	sfSprite* spriteTurretLBase;
	sfSprite* spriteTurretLCanon;
	sfSprite* spriteTurretRCase;
	sfSprite* spriteTurretRBase;
	sfSprite* spriteTurretRCanon;
	sfSprite* droneFactory;

	float cooldownShootBullet;
	float cooldownShootDrone;

	sfBool bossReacting;
	sfBool isAlive;
	sfBool isFreezed;
	float timerFreezed;

	float bossLife;

	float runAwayTiming;

	sfVector2f velocity;

	PlayerPositionToBoss playerPositionToBoss;
	BossReactionToPlayer bossReactionToPlayer;
}Boss1;

typedef struct Boss
{
	Boss1* boss1;

}Boss;

void LoadBoss(void);

void SwitchBoss(char _index, sfVector2f _position);

void CheckBossPlayerState(sfVector2f _posPlayer, float _dt);

void UpdateBoss(sfVector2f _posPlayer, float _dt);
void UpdateTurret(sfVector2f _posPlayer, float _dt);

sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount, int _axis);

sfBool HitBoss(float _degat, sfFloatRect _hitbox, AttackType _attackType);
sfBool DamageBoss(float _damage);

void BossShoot(sfVector2f _posPlayer, float _dt);

void DestroyBoss1(void);

float* GetBossHpAdr(void);
float GetBossPositionY(void);

sfBool IsBossActive(void);
#endif // !BOSS_H