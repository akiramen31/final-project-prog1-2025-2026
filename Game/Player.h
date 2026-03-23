#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

typedef enum  State
{
	IDLE = 0,
	WALK = 4
}State;

typedef struct Player
{
	State state;
	sfSprite* sprite;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void KillPlayer(void);
#endif
