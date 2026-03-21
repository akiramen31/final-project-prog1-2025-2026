#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h" 
#include "Box.h"
#include "Bomb.h"
#include "PowerUp.h"

typedef struct
{
	sfSound* kill;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
