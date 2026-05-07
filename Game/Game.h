#ifndef GAME_H
#define GAME_H

#include "Common.h"

#define START_GAME_CAM_DURATION 2.5f
#define PAUSE_ROOM_DURATION 0.25f

typedef struct
{
	float timer;
	float timerRoomPause;
	float timerstartLevel;
	sfSprite* temp;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
