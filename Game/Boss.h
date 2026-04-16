#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"
#include "Box.h"

#define RELOAD_TIME_TURRET 2.f
#define TURRET_ROTATION_SPEED 60.f

typedef enum bossReactionToPlayer
{
	PLAYER_AWAY,
	PLAYER_RANGE_SHOOT,
	PLAYER_UNDER,
	PLAYER_ON_TOP,
	PLAYER_TURRET

}bossReactionToPlayer;

typedef struct Boss1
{
	sfSprite* track;
	sfSprite* steamTank;
	sfSprite* gunCariage;
	sfSprite* spriteCanon;
	float timerCanon;
	sfSprite* spriteTurretLCase;
	sfSprite* spriteTurretLBase;
	sfSprite* spriteTurretLCanon;
	float timerReloadTurretLCanon;
	sfSprite* spriteTurretRCase;
	sfSprite* spriteTurretRBase;
	sfSprite* spriteTurretRCanon;
	float timerReloadTurretRCanon;

	int turretLMunitions;
	int turretRMunitions;

	float bossLife;

	float runAwayTiming;

	sfVector2f velocity;

	bossReactionToPlayer bossReactionToPlayer;
}Boss1;

typedef struct Boss
{
	Boss1* boss1;

}Boss;

void LoadBoss(void);

void SwitchBoss(char _index, sfVector2f _position);

void UpdateBoss(float _dt);
void UpdateTurret(float _dt);

sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount, int _axis);

void HitBoss(sfFloatRect _hitbox);

#endif //BOSS_H