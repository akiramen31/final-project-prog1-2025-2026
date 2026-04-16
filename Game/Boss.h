#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"
#include "Box.h"

typedef enum bossReactionToPlayer
{
	PLAYER_AWAY,
	PLAYER_RANGE_SHOOT,
	PLAYER_UNDER,
	PLAYER_ON_TOP,
	PLAYER_TURRET,
	PLAYER_RIGHT,
	PLAYER_LEFT

}bossReactionToPlayer;

typedef struct Boss1
{
	sfSprite* track;
	sfSprite* steamTank;
	sfSprite* gunCariage;
	sfSprite* spriteCanon;
	float timerCanon;
	sfSprite* spriteTurret1Case;
	sfSprite* spriteTurret1Base;
	sfSprite* spriteTurret1Canon;
	float timerTurret1Canon;
	sfSprite* spriteTurret2Case;
	sfSprite* spriteTurret2Base;
	sfSprite* spriteTurret2Canon;
	float timerTurret2Canon;

	float runAwayTiming;


	sfVector2f velocity;
}Boss1;

typedef struct Boss
{
	Boss1* boss1;

}Boss;

void LoadBoss(void);

void SwitchBoss(char _index, sfVector2f _position);

void UpdateBoss(float _dt);
sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount, int _axis);

void HitBoss(sfFloatRect _hitbox);

#endif //BOSS_H