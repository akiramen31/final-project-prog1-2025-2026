#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

typedef enum  State
{
	IDLE = 0,
	WALKING = 4
}State;

typedef struct Player
{
	Direction direction;
	State state;

	sfSprite* player;
	sfVector2i posGrid;
	Animation animation
}Player;

void PlayerLoad(void);
void PlayerUpdate(float _dt);

void PlayerMove(float _dt);
#endif
