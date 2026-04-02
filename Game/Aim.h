#ifndef AIM_H
#define AIM_H

#include "Common.h"

#define AIM_SIZE 16 

typedef struct Aim
{
	sfSprite* sprite;
	sfRectangleShape* collision;
}Aim;

void LoadAim(void);
void UpdateAim( float _dt);

sfVector2f GetAimPosition(void);

#endif // !AIM_H
