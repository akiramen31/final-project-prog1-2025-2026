#include "aim.h"

void MoveAim(float _dt);
void CheckCollisionAimScreen();
Aim aim;
float cameraCoef = 0.2f;

void LoadAim(void)
{
	aim = (Aim){ 0 };

	sfTexture* texture = GetAsset("Assets/Sprites/crosshair.png");
	aim.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 41);
	SetSpriteOriginMiddle(aim.sprite);

	aim.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(aim.collision, (sfVector2f) { AIM_SIZE, AIM_SIZE });
	sfRectangleShape_setOrigin(aim.collision, (sfVector2f) { AIM_SIZE, AIM_SIZE });

}

void UpdateAim(float _dt)
{
	MoveAim(_dt);
	// CheckCollisionAimScreen();
}

sfVector2f GetAimPosition(void)
{
	return sfSprite_getPosition(aim.sprite);
}

void MoveAim(float _dt)
{
	cameraCoef = GetCameraZoom();
	sfVector2i mousePosI = sfMouse_getPositionRenderWindow(GetRenderWindow());
	sfVector2f viewPos = GetViewPosition();
	sfVector2f mousePos = { (float)mousePosI.x * cameraCoef + viewPos.x, (float)mousePosI.y * cameraCoef + viewPos.y };
	sfSprite_setPosition(aim.sprite, mousePos);
}

void CheckCollisionAimScreen(void)
{
	sfFloatRect aimHitbox = sfSprite_getGlobalBounds(aim.sprite);

	if (aimHitbox.left < 0)
	{
		sfSprite_move(aim.sprite, (sfVector2f) { 0 - aimHitbox.left, 0 });

	}
	else if (aimHitbox.left + aimHitbox.width > SCREEN_WIDTH)
	{
		sfSprite_move(aim.sprite, (sfVector2f) { SCREEN_WIDTH - (aimHitbox.left + aimHitbox.width), 0 });

	}

	if (aimHitbox.top < 0)
	{
		sfSprite_move(aim.sprite, (sfVector2f) { 0, 0 - aimHitbox.top });

	}
	else if (aimHitbox.top + aimHitbox.height > SCREEN_HEIGHT)
	{
		sfSprite_move(aim.sprite, (sfVector2f) { 0, SCREEN_HEIGHT - (aimHitbox.top + aimHitbox.height) });

	}
}
