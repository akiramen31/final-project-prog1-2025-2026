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
	Direction direction;
	State state;

	sfSprite* sprite;
	Animation animation;
	sfVector2i posGrid;
	sfVector2f moveLength;

	int playerMoveSpeedGrid;
	sfBool isWalking;
}Player;

void LoadPlayer(void);
void UpdatePlayer(CasePosibility GetMovePosibility, float _dt);

sfVector2i GetPlayerPositionGrid(void);
#endif
