#ifndef GAME_H
#define GAME_H

#include "Common.h"

typedef struct
{
	sfSprite* temp;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
