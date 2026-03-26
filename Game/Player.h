#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

#define PLAYER_WALK_SPEED_MAX 400

#define PLAYER_JUMP_POWER 4

#define PLAYER_DASH_POWER 4
#define PLAYER_DASH_COOLDOWN 1.f
#define PLAYER_DASH_DURATION 0.2f

typedef struct Player
{
	enum State state;
	sfSprite* sprite;
	float life;
	float energyMax;
	float energy;
	sfVector2f velocity;

	// if True direction is right
	sfBool direction;

	sfBool isGrounded;
	sfBool isDashing;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void KillPlayer(void);

sfVector2f GetPlayerPosition(void);
#endif
