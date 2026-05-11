#ifndef GAME_H
#define GAME_H

#include "Common.h"

#define PAUSE_ROOM_DURATION 0.25f

typedef struct
{
	float timer;
	float timerRoomPause;
	float timerStartLevel;
		
	float timerDurationStartLevel;

	sfSprite* temp;

	sfCircleShape* startIntroCircle;
	sfRectangleShape* startIntoRectangle;
	sfBool startIntroIsFinished;

	sfCircleShape* cameraCenter;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
