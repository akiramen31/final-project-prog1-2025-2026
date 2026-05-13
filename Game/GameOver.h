#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"

typedef struct GameOver
{
	sfText* button[2];
	sfText* text;
	sfText* score[4];
}GameOver;

void LoadGameOver(void);
void PollEventGameOver(sfEvent* _event);
void UpdateGameOver(float _dt);
#endif // !GAME_OVER_H
