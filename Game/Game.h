#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h"
#include "Map.h"

typedef struct
{
	sfSprite* temp;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
