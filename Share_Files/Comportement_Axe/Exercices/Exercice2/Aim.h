#ifndef AIM_H
#define AIM_H

#include "Common.h"

#define AIM_RADIUS 20

typedef struct Aim
{
	sfCircleShape* circle;
}Aim;

void LoadAim(void);
void UpdateAim(sfRenderWindow* const _renderWindow, float _dt);
void DrawAim(sfRenderWindow* const _renderWindow);
void CleanupAim(void);

sfVector2f GetAimPosition(void);

#endif // !AIM_H
