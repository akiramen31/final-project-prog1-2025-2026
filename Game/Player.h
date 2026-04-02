#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Aim.h"

#define PLAYER_WALK_SPEED_MAX 400

#define PLAYER_FALL_SPEED_MAX 1
#define PLAYER_JUMP_POWER 1.9f

#define PLAYER_DASH_POWER 3.f
#define PLAYER_DASH_COOLDOWN 0.5f
#define PLAYER_DASH_DURATION 0.06f

#define PLAYER_JUMP_FORGIVE 0.05f

#define PLAYER_COLLISION_WIDTH 14
#define PLAYER_COLLISION_HEIGHT 30

typedef struct Player
{
	enum State state;
	sfSprite* sprite;
	sfRectangleShape* collision;

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
