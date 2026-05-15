#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#define BOSS1_SPEED PLAYER_HORIZONTAL_SPEED_MAX * 0.8f
#define BOSS1_SPEED_RUNAWAY BOSS1_SPEED * 2.5f

#define BOSS1_RUNAWAY_TIMER 4.f

#define BOSS1_FIRERATE_BULLET 1.f  //in hz aka in bullet per second
#define BOSS1_FIRERATE_DRONE 1.f   //in hz aka in drone per second
#define MAX_BOSS1_LIFE 250.f

#define BOSS1_TURRET_ROTATION_SPEED 150.f

#define BOSS1_SHOOT_DISTANCE_MAX 300.f
#define BOSS1_SHOOT_DISTANCE_MIN 80.f

//
#define MAX_BOSS2_LIFE 150.f
#define BOSS2_SWITCH_MODE_TIMER 2.f

#define SPEED_BOSS2_ASCENDING 140.f
#define BOSS2_SPEED_DESCENDING 100.f
#define SPEED_BOSS2_STEAMTANK 14.f

#define BOSS2_UNHIDDING_HEIGHT 285.f
#define BOSS2_SHOOT_HEIGHT 180.f
#define BOSS2_SPEED_SHOOTING_HORIZONTALE 70.f
#define BOSS2_BOMB_HEIGHT 100.f
#define BOSS2_SPEED_BOMBING_HORIZONTALE 110.f

#define BOSS2_BOMBING_RATE 0.5f

#define BOSS2_GO_UNHIDDING 2.5f

#define BOSS2_FIRERATE_SHOOTING 1.f
#define BOSS2_FIRERATE_BOMBING BOSS2_FIRERATE_SHOOTING * 1.5f
#define BOSS2_FIRERATE_UNHIDDING BOSS2_FIRERATE_SHOOTING * 2.f

#define ARENA1_CENTER 9952.f
#define ARENA1_ENTRY ARENA1_CENTER -381.f

#define ARENA1_LIMITE_LEFT ARENA1_CENTER - 320.f
#define ARENA1_LIMITE_RIGHT ARENA1_CENTER + 320.f

#define ARENA2_CENTER 5189.f
#define ARENA2_GROUND 430.f
#define ARENA2_ENTRY ARENA2_CENTER -381.f

#define ARENA2_LIMITE_LEFT ARENA2_CENTER - 300.f
#define ARENA2_LIMITE_RIGHT ARENA2_CENTER + 300.f
#define ARENA2_LIMITE_BOMBING_LEFT ARENA2_CENTER - 150.f
#define ARENA2_LIMITE_BOMBING_RIGHT ARENA2_CENTER + 150.f


typedef enum PlayerPositionToBoss1
{
	NOT_IN_ARENA1,
	AWAY_RIGHT,
	AWAY_LEFT,
	SHOT_RANGE_RIGHT,
	SHOT_RANGE_LEFT,
	UNDER,
	TOP,
	TURRET_RIGHT,
	TURRET_LEFT

}PlayerPositionToBoss1;


typedef enum Boss1Reaction
{
	NONE1,
	SLOW_LEFT,
	SLOW_RIGHT,
	FAST_LEFT,
	FAST_RIGHT
}Boss1Reaction;

typedef enum PlayerPositionToBoss2
{
	NOT_IN_ARENA2,
	HIDDEN,
	ON_PLATFORM,
	EXPOSED,
	ON_BOSS
}PlayerPositionToBoss2;

typedef enum Boss2Reaction
{
	BOSS2_NONE,
	BOSS2_STARTING,
	BOSS2_SHOOTING,
	BOSS2_BOMBING,
	BOSS2_UNHIDDING
}Boss2Reaction;

typedef enum Boss1Parts
{
	CARIAGE,
	MISSILE_LAUNCHER,
	TRACK,
	STEAM_TANK_BOSS1,
	L_CHAMBER,
	L_CANNON,
	R_CHAMBER,
	R_CANNON,
	PART_COUNT_BOSS1
}Boss1Parts;

typedef enum Boss2Parts
{
	TURRET_CANNON,
	TURRET_BASE,
	BODY,
	STEAM_TANK_BOSS2,
	STEAM_TANK_COVERING,
	STEAM_TANK_CHIMNEY, //
	BOMB_BAY,
	LEFT_ROTOR,
	LEFT_KICKSTAND,
	TOP_LEFT_CHIMNEY, //
	RIGHT_ROTOR, //
	MIDDLE_KICKSTAND,
	RIGHT_KICKSTAND, //
	TOP_RIGHT_CHIMNEY,
	BOTTOM_LEFT_CHIMNEY,
	BOTTOM_RIGHT_CHIMNEY,
	PART_COUNT_BOSS2
}Boss2Parts;

typedef struct Boss1
{
	sfSprite* sprites[PART_COUNT_BOSS1];
	sfFloatRect hitboxes[4];
	float timerCanon;
	float cooldownBullet;
	float cooldownBallistic;
	PlayerPositionToBoss1 playerPositionToBoss1;
	Boss1Reaction boss1ReactionToPlayer;
	sfBool boss1Reacting;
	float runAwayTiming;
	sfVector2f velocity;
}Boss1;

typedef struct Boss2
{
	sfSprite* sprites[PART_COUNT_BOSS2];
	sfFloatRect hitboxes[14];
	PlayerPositionToBoss2 playerPositionToBoss2;
	Boss2Reaction boss2Reaction;
	float unhiddingReactionTimer;
	float reactionTimer;
	float powerMultiplier;
	float cooldownBullet;
	float cooldownBomb;
	sfVector2f velocity;
	sfVector2f aimDestination;
	sfBool bombOut;

	int chimneyST;
	int chimneyTL;
	int chimneyTR;
	int chimneyBL;
	int chimneyBR;
}Boss2;

typedef struct Boss
{
	void* entity;
	Boss1* boss1;
	Boss2* boss2;
	int currentBoss;
	float timerFrozen;
	float life;
}Boss;

void LoadBoss(int _index, sfVector2f _position);

void CheckBossPlayerState(sfVector2f _posPlayer, float _dt);

void UpdateBoss(sfVector2f _posPlayer, float _dt);
void UpdateTurret(sfVector2f _posPlayer, float _dt);

sfVector2f ColisionBossplayer(sfFloatRect _playerHitbox, CollisionAxis _axis);

sfBool HitBoss(float _degat, sfFloatRect _hitbox, AttackType _attackType);
sfBool DamageBoss(float _damage);

void BossShoot(sfVector2f _posPlayer, float _dt);

void DestroyBoss(int _boss);
void DestroyChimney(int _index);

float GetBossLife(void);
float* GetBossLifeAdress(void);
float GetBossMaxLife(void);
sfVector2f GetBossPosition(void);

void MoveSteamTankBoss2(float _dt);

sfBool IsBossActive(void);
#endif // !BOSS_H