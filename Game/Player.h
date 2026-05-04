#ifndef PLAYER_H
#define PLAYER_H

#include "Weapons.h"
#include "Secondaries.h"
#include "Common.h"

#define PLAYER_HORIZONTAL_SPEED_MAX 150
#define PLAYER_VERTICAL_SPEED_MAX 300

#define PLAYER_FALL_SPEED_MAX 1.1f
#define PLAYER_JUMP_POWER 1.65f

#define PLAYER_DASH_POWER 3.f
#define PLAYER_DASH_COOLDOWN 0.5f
#define PLAYER_DASH_DURATION 0.13f

#define PLAYER_JUMP_FORGIVE 0.1f

#define PLAYER_DAMAGE_IMUNITY_DURATION 2.f

#define PLAYER_MAX_HEALTH 10

typedef struct EnergyPlayerStruct
{
	float energy;
	float energyMax;
	float energyRegen;
	float energyRegenCooldown;

	float dashConsuption;
}EnergyPlayerStruct;

typedef struct Player
{
	sfSprite* sprite;
	sfRectangleShape* collision;

	Weapon weapon;

	int life;
	int lifeMax;

	EnergyPlayerStruct ener;

	sfVector2f velocity;

	sfBool canShoot;
	sfBool isAttacking;

	float pressTime;
	float cooldown;

	// if True direction is right
	sfBool direction;

	sfBool isGrounded;

	sfVector2f spawn;
	sfVector2f tpBoss;

	Animation running;
	Animation jumping;
	Animation idling;
	Animation dashing;

	sfSound* walkSound;
	sfSound* jumpSound;
	sfSound* hitSound;
	sfSound* cutSound;
	sfSound* shootSound;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);

void DamagePlayer(int _damage);
void KillPlayer(void);

sfVector2f GetPlayerPosition(void);
sfFloatRect GetPlayerRect(void);

sfVector2f GetPlayerVelocity(void);
void SetPlayerVelocity(sfVector2f _velocity);

float GetPlayerEnergyInfo(int _index);
void SetPlayerEnergyInfo(float _val, int _index);

int GetPlayerLife(void);
void SetPlayerLifeMax(int _life);
void AddPlayerLife(int _life);

void SetPlayerPosition(sfVector2f _pos);
void MovePlayer(sfVector2f _move);

void TpPlayerToSpawn(void);
void SetSpawnPlayer(sfVector2f _pos);

void TpPlayerBoss(void);
void SetTpPlayerBoss(sfVector2f _pos);

void HandlePlayerBossCollision(sfVector2f _push);

void ChangePlayerInvicibility(void);

sfBool ColisionWithPlayer(sfFloatRect _rect);
#endif
