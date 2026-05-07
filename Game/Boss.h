#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#define BOSS_SPEED PLAYER_HORIZONTAL_SPEED_MAX * 0.8f
#define BOSS_SPEED_RUNAWAY BOSS_SPEED * 2.5f

#define BOSS_PART_NUMBER 10

#define RUNAWAY_TIMER 4.f

#define BOSS_FIRERATE_BULLET 1.f  //in hz aka in bullet per second
#define BOSS_FIRERATE_DRONE 1.f   //in hz aka in drone per second
#define MAX_BOSS_0_LIFE 250.f

#define TURRET_ROTATION_SPEED 150.f

#define SHOOT_DISTANCE_MAX 300.f
#define SHOOT_DISTANCE_MIN 80.f

#define ARENA_CENTER 8671.f
#define ARENA_ENTRY 8290.f

#define ARENA_LIMITE_LEFT 8351.f
#define ARENA_LIMITE_RIGHT 8991.f

typedef enum PlayerRangeToBoss
{
	NOT_IN_ARENA,
	AWAY_RIGHT,
	AWAY_LEFT,
	SHOT_RANGE_RIGHT,
	SHOT_RANGE_LEFT,
	UNDER,
	TOP,
	TURRET_RIGHT,
	TURRET_LEFT

}PlayerRangeToBoss;

typedef enum BossReaction
{
	NONE,
	SLOW_LEFT,
	SLOW_RIGHT,
	FAST_LEFT,
	FAST_RIGHT
}BossReaction;

typedef enum BossParts
{
	CARIAGE,
	MISSILE_LAUNCHER,
	TRACK,
	STEAM_TANK,
	L_CHAMBER,
	L_CANNON,
	R_CHAMBER,
	R_CANNON,
	PART_COUNT
};

typedef struct Boss1
{
	sfSprite* sprites[PART_COUNT];
	sfFloatRect hitboxes[4];
	float timerCanon;
	float cooldownBullet;
	float cooldownBallistic;
	PlayerRangeToBoss playerPositionToBoss;
	BossReaction bossReactionToPlayer;
	sfBool bossReacting;
	float runAwayTiming;
	sfVector2f velocity;
}Boss1;

typedef struct Boss
{
	Boss1* boss1;
	int currentBoss;
	float timerFrozen;
	float life;
}Boss;

void LoadBoss(int _index, sfVector2f _position);

void CheckBossPlayerState(sfVector2f _posPlayer, float _dt);

void UpdateBoss(sfVector2f _posPlayer, float _dt);
void UpdateTurret(sfVector2f _posPlayer, float _dt);

sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount, int _axis);

sfBool HitBoss(float _degat, sfFloatRect _hitbox, AttackType _attackType);
sfBool DamageBoss(float _damage);

void BossShoot(sfVector2f _posPlayer, float _dt);

void DestroyBoss(void);

float GetBossLife(void);
float* GetBossLifeAdress(void);
sfVector2f GetBossPosition(void);

sfBool IsBossActive(void);
#endif // !BOSS_H