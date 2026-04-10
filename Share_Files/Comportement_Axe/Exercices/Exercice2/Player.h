#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Missile.h"
#include "Aim.h"

#define PLAYER_RADIUS 20

#define RANGE_RADIUS PLAYER_RADIUS * 2.5f
#define RANGE_ANGLE_LIGHT 120.0f
#define RANGE_ANGLE_MEDIUM 180.0f
#define RANGE_ANGLE_HEAVY 360.0f

#define AXE_RADIUS PLAYER_RADIUS * 2.0f

#define FIRE_RATE 4.f

#define MAX_COOLDOWN (1.f /FIRE_RATE )

typedef enum AttackType {
	LIGHT,
	MEDIUM,
	HEAVY,
	NOATTACK
}AttackType;

typedef struct Player
{
	sfCircleShape* circle;
	sfRectangleShape* axe;
	sfCircleShape* rangeBox;
	float speed;
	sfVertexArray* lineOfSight;
	sfBool canHit;
	float cooldown;
	sfBool isAttacking;
	sfBool isRighted;
	float pressTime;
	AttackType attackType;
	float energy;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt, sfVector2f _start, sfVector2f _target);
void DrawPlayer(sfRenderWindow* const _renderWindow);
void CleanupPlayer(void);

sfRectangleShape* GetAxe(void);

void ChangePlayerState(void);

sfVector2f GetPlayerPosition(void);
AttackType GetPlayerAttack(void);
void UpdatePlayerLineOfSight(sfVector2f _targetPosition);

sfBool IsPlayerAttacking(void);

#endif // !PLAYER_H
