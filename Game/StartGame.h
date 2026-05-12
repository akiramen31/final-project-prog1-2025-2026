#ifndef START_GAME_H
#define START_GAME_H

#include "Common.h"

#define CREA_POS (sfVector2f) { (SCREEN_WIDTH / 2.f) + 200.f, SCREEN_HEIGHT / 2.f}
#define ROSE_POS (sfVector2f) { (SCREEN_WIDTH / 2.f) - 200.f, SCREEN_HEIGHT / 2.f}

typedef struct StartGame
{
	sfSprite* crea1;
	sfSprite* crea2;
	sfTexture* textureCrea[2];

	sfSprite* rose1;
	sfSprite* rose2;

	sfBool update;
	float timer;
}StartGame;

void LoadStartGame(void);
void PollEventStartGame(sfEvent* _event);
void UpdateStartGame(float _dt);


#endif // !START_GAME_H
