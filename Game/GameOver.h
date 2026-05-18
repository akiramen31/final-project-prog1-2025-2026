#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "Common.h"

#define ANIM_FRAME_COUNT 20

typedef struct
{
	int didPlayerLoose;
	sfText* button[2];
	sfText* text;
	sfText* score[4];
	sfSprite* background;
	float frameTime[ANIM_FRAME_COUNT];
	sfIntRect frameRect[ANIM_FRAME_COUNT];
	int currentFrame;
	float timerAnim;
}GameOver;

void LoadGameOver(void);
void PollEventGameOver(sfEvent* _event);
void UpdateGameOver(float _dt);
#endif // !GAME_OVER_H
