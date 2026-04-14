#ifndef BOSS_H
#define BOSS_H

#include "Common.h"
#include "Map.h"
#include "Player.h"
#include "Box.h"

typedef struct Boss1
{
	sfSprite* sprite1;
	sfSprite* sprite2;
	sfSprite* spriteCanon;
	float timerCanon;
	sfSprite* spriteTurret1;
	sfSprite* spriteTurret1Bis;
	sfSprite* spriteTurret1Canon;
	float timerTurret1Canon;
	sfSprite* spriteTurret2;
	sfSprite* spriteTurret2Bis;
	sfSprite* spriteTurret2Canon;
	float timerTurret2Canon;
	sfVector2f velocity;
}Boss1;

typedef struct Boss
{
	Boss1* boss1;

}Boss;

void LoadBoss(void);

void SwitchBoss(char _index, sfVector2f _position);

void UpdateBoss(float _dt);

void HitBoss(sfFloatRect _hitbox);

#endif //BOSS_H