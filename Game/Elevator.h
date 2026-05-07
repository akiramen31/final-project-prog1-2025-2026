#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "Common.h"

#define SPEED_ELEVATOR 32

typedef struct Elvator
{
	sfSprite* sprite;
	sfRectangleShape* hitbox[3];
	int numberLevel;
	int actualLevel;
	float posYlevel[3];
}Elevator;

void LoadElevator(void);
void LoadElevator(void);
sfVector2f UpdateElevator(sfFloatRect _rectPlayer, sfVector2f _posPlayer, float _dt);

sfVector2f ColisionElevator(sfFloatRect _hitbox, int _axis);
sfBool ColisionElevatorButon(sfFloatRect _hitbox);
#endif