#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"

#define CREA_POS (sfVector2f) { (SCREEN_WIDTH / 2.f) + 200.f, SCREEN_HEIGHT / 2.f}
#define ROSE_POS (sfVector2f) { (SCREEN_WIDTH / 2.f) - 200.f, SCREEN_HEIGHT / 2.f}

typedef struct GameOver
{
	sfSprite* sprite;
}GameOver;

void LoadGameOver(void);
void PollEventGameOver(sfEvent* _event);
void UpdateGameOver(float _dt);


#endif // !GAME_OVER_H
