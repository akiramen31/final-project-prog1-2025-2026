#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Animation.h"

enum struct Direction
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
} ;

enum struct State
{
	IDLE = 0,
	WALKING = 4
} ;

typedef struct Player
{
	Direction direction;
	State state;

	AssetEntity player;
}Player;

void PlayerLoad(void);
void PlayerUpdate(float _dt);

void PlayerMove(float _dt);
#endif
