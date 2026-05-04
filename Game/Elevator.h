#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "Common.h"

#define SPEED_ELEVATOR 16

typedef struct Elvator
{
	sfSprite* sprite;
	int numberLevel;
	int actualLevel;
	float posYlevel[3];
}Elevator;

void LoadElevator(void);
void LoadElevator(void);
void UpdateElevator(float _dt);
#endif