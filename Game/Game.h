#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h" 
#include "Box.h"
#include "Bomb.h"

typedef enum
{
	VOID,
	WALL,
	BOX,
	FIRE_UP,
	FULL_FIRE,
	FIRE_DOWN,
	SPEED_UP,
	SPEED_DOWN,
	BOMBE_UP,
	BOMB_DOWN,
	EXIT
}CaseContainer;

typedef struct
{
	CaseContainer caseState[NB_GRID_ROW][NB_GRID_COLUMN];
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
