#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"



typedef struct Player
{
	State state;
	sfSprite* sprite;
	float life;
	float energy;
	sfVector2f move;
}Player;

void LoadPlayer(void);
void UpdatePlayer(float _dt);
void KillPlayer(void);
#endif
