#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"
#include "GameState.h"

typedef struct GameOver
{
	sfSprite* temp;
}GameOver;

void LoadGameOver(void);
void PollEventGameOver(sfEvent* _event);
void UpdateGameOver(float _dt);


#endif // !GAME_OVER_H
