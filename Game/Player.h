#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

#define PLAYER_WALK_SPEED_MAX 100

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
#endif
