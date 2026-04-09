#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Bullet.h"
#include "Missile.h"
#include "Aim.h"

#define FIRE_RATE_RAILGUN 1.1f

#define PLAYER_WALK_SPEED_MAX 290

#define PLAYER_FALL_SPEED_MAX 1
#define PLAYER_JUMP_POWER 2.2f

#define PLAYER_DASH_POWER 2.0f
#define PLAYER_DASH_COOLDOWN 0.5f
#define PLAYER_DASH_DURATION 0.08f

#define PLAYER_JUMP_FORGIVE 0.1f

#define PLAYER_COLLISION_WIDTH 14
#define PLAYER_COLLISION_HEIGHT 32

#define WEAPON_ANGLE_OFFSET 11.5f

typedef enum WeaponType
{
	RAILGUN,
	OTHER
}WeaponType;

typedef struct Weapon
{

	sfSprite* sprite;
	WeaponType type;
	sfBool isRight;

}Weapon;

typedef struct Player
{
	enum State state;
	sfSprite* sprite;
	sfRectangleShape* collision;

	Weapon weapon;

	float life;
	float energyMax;
	float energy;
	sfVector2f velocity;

	sfBool canShoot;
	float cooldown;

	// if True direction is right
	sfBool direction;

	sfBool isGrounded;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void KillPlayer(void);

sfVector2f GetPlayerPosition(void);
sfFloatRect GetPlayerRect(void);

void SetPlayerPosition(sfVector2f _pos);

#endif