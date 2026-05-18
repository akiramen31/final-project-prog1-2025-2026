#ifndef GAME_H
#define GAME_H

#include "Common.h"

#define PAUSE_ROOM_DURATION 0.25f
#define PAUSE_BUTTON_COUNT 4

typedef struct
{
	sfText* button[PAUSE_BUTTON_COUNT];
	sfSprite* backgound;
	char actif;
}GamePause;

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

	GamePause pause;
}Game;

void LoadGame(void);
void PollEventGame(sfEvent* _event);
void UpdateGame(float _dt);
#endif // !GAME_H
