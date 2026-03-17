#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

typedef enum  State
{
	IDLE,
	WALKING
}State;

typedef struct Player
{
	Direction direction;
	State state;

	sfSprite* player;
}Player;

void PlayerLoad(void);
void PlayerUpdate(float _dt);

void PlayerMove(float _dt);
#endif
