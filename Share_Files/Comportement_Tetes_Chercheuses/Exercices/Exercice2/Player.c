#include "Player.h"

Player player = { 0 };

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color);
void MovePlayer(float _dt, sfRenderWindow* _sfRenderWindow);
void CheckCollisionPlayerScreen(void);

void LoadPlayer(void)
{
	player.circle = sfCircleShape_create();
	sfCircleShape_setRadius(player.circle, PLAYER_RADIUS);
	sfCircleShape_setOrigin(player.circle, (sfVector2f) { PLAYER_RADIUS, PLAYER_RADIUS });
	sfCircleShape_setPosition(player.circle, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfCircleShape_setFillColor(player.circle, sfBlue);

	player.speed = 500;
	player.lineOfSight = CreateLineOfSight((sfVector2f) { 0, 0 }, (sfVector2f) { 0, 0 }, sfRed);
}

void UpdatePlayer(sfRenderWindow* const _renderWindow, float _dt)
{
	MovePlayer(_dt, _renderWindow);
	CheckCollisionPlayerScreen();
}

void DrawPlayer(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawCircleShape(_renderWindow, player.circle, NULL);
	sfRenderWindow_drawVertexArray(_renderWindow, player.lineOfSight, NULL);
}

void CleanupPlayer(void)
{
	sfCircleShape_destroy(player.circle);
	sfVertexArray_destroy(player.lineOfSight);
	player = (Player){ 0 };
}

void MovePlayer(float _dt, sfRenderWindow* _renderWindow)
{
	
	sfVector2i mousePosI = sfMouse_getPositionRenderWindow(_renderWindow);
	sfVector2f mousePos = { (float)mousePosI.x, (float)mousePosI.y };
	sfCircleShape_setPosition(player.circle, mousePos);
}

void CheckCollisionPlayerScreen(void)
{
	sfFloatRect playerHitbox = sfCircleShape_getGlobalBounds(player.circle);

	if (playerHitbox.left < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0 - playerHitbox.left, 0 });
	}
	else if (playerHitbox.left + playerHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
	}

	if (playerHitbox.top < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, 0 - playerHitbox.top });
	}
	else if (playerHitbox.top + playerHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
	}
}

sfVector2f GetPlayerPosition(void)
{
	return sfCircleShape_getPosition(player.circle);
}

void UpdatePlayerLineOfSight(sfVector2f _targetPosition)
{
	sfVertex* firstVertex = sfVertexArray_getVertex(player.lineOfSight, 0);
	firstVertex->position = sfCircleShape_getPosition(player.circle);

	sfVertex* lastVertex = sfVertexArray_getVertex(player.lineOfSight, 1);
	lastVertex->position = _targetPosition;
}

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color)

{

	sfVertexArray* newLine = sfVertexArray_create();

	sfVertexArray_setPrimitiveType(newLine, sfLines);



	sfVertex vertexA = { _pointA, _color };

	sfVertexArray_append(newLine, vertexA);



	sfVertex vertexB = { _pointB, _color };

	sfVertexArray_append(newLine, vertexB);



	return newLine;

}