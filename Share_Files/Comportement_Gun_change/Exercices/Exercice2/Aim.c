#include "aim.h"

void MoveAim(float _dt, sfRenderWindow* _renderWindow);
void CheckCollisionAimScreen();
Aim aim;

void LoadAim(void)
{
		
	aim.circle = sfCircleShape_create();
	sfCircleShape_setRadius(aim.circle, AIM_RADIUS);
	sfCircleShape_setOrigin(aim.circle, (sfVector2f) { AIM_RADIUS, AIM_RADIUS });
	sfCircleShape_setFillColor(aim.circle, sfTransparent);
	sfCircleShape_setOutlineColor(aim.circle, sfGreen);
	sfCircleShape_setOutlineThickness(aim.circle, 2);
	sfCircleShape_setPointCount(aim.circle, 50);

	sfVector2f aimPosition = { 0 };
	aimPosition.x = (float)GetRandom(AIM_RADIUS, SCREEN_WIDTH - AIM_RADIUS - 1);
	aimPosition.y = (float)GetRandom(AIM_RADIUS, SCREEN_HEIGHT - AIM_RADIUS - 1);
	sfCircleShape_setPosition(aim.circle, aimPosition);

	

}

void UpdateAim(sfRenderWindow* const _renderWindow, float _dt)
{
	MoveAim(_dt, _renderWindow);
	CheckCollisionAimScreen();
}

void DrawAim(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawCircleShape(_renderWindow, aim.circle, NULL);
}

void CleanupAim(void)
{
	sfCircleShape_destroy(aim.circle);
}

sfVector2f GetAimPosition(void)
{
	return sfCircleShape_getPosition(aim.circle);
}

void MoveAim(float _dt, sfRenderWindow* const _renderWindow)
{
	sfVector2i mousePosI = sfMouse_getPositionRenderWindow(_renderWindow);
	sfVector2f mousePos = { (float)mousePosI.x, (float)mousePosI.y };
	sfCircleShape_setPosition(aim.circle, mousePos);
}

void CheckCollisionAimScreen(void)
{
	sfFloatRect aimHitbox = sfCircleShape_getGlobalBounds(aim.circle);

	if (aimHitbox.left < 0)
	{
		sfCircleShape_move(aim.circle, (sfVector2f) { 0 - aimHitbox.left, 0 });
		
	}
	else if (aimHitbox.left + aimHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(aim.circle, (sfVector2f) { SCREEN_WIDTH - (aimHitbox.left + aimHitbox.width), 0 });
		
	}

	if (aimHitbox.top < 0)
	{
		sfCircleShape_move(aim.circle, (sfVector2f) { 0, 0 - aimHitbox.top });
		
	}
	else if (aimHitbox.top + aimHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(aim.circle, (sfVector2f) { 0, SCREEN_HEIGHT - (aimHitbox.top + aimHitbox.height) });
		
	}
}
