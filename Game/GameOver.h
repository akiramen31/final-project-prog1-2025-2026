#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"

typedef struct GameOver
{
	sfSprite* sprite;
}GameOver;

void LoadGameOver(void);
void PollEventGameOver(sfEvent* _event);
void UpdateGameOver(float _dt);


#endif // !GAME_OVER_H
