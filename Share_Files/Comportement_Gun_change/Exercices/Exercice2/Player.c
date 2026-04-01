#include "Player.h"

Player player = { 0 };

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color);
void MovePlayer(float _dt, sfVector2f _start, sfVector2f _target);
void UpdateCooldown(float _dt);
void UpdateFireControl(void);
void CheckCollisionPlayerScreen(void);

void LoadPlayer(void)
{
	player.circle = sfCircleShape_create();
	sfCircleShape_setRadius(player.circle, PLAYER_RADIUS);
	sfCircleShape_setOrigin(player.circle, (sfVector2f) { PLAYER_RADIUS, PLAYER_RADIUS });
	sfCircleShape_setPosition(player.circle, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfCircleShape_setFillColor(player.circle, sfCyan);

	player.rect = sfRectangleShape_create();
	sfRectangleShape_setSize(player.rect, (sfVector2f) {  1.5f * PLAYER_RADIUS ,PLAYER_RADIUS / 1.5f });
	sfFloatRect bounds = sfRectangleShape_getLocalBounds(player.rect);
	sfRectangleShape_setOrigin(player.rect, (sfVector2f) { 0, bounds.height / 2.0f });
	sfRectangleShape_setPosition(player.rect, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfRectangleShape_setFillColor(player.rect, sfBlue);

	player.speed = 500;
	player.canShoot = sfTrue;
	player.cooldown = 1.f / FIRE_RATE;
	player.lineOfSight = CreateLineOfSight((sfVector2f) { 0, 0 }, (sfVector2f) { 0, 0 }, sfRed);
}

void UpdatePlayer(float _dt, sfVector2f _start, sfVector2f _target)
{
	MovePlayer(_dt, _start, _target);
	if (!player.canShoot)
	{
		UpdateCooldown(_dt);
	}
	else
	{
		UpdateFireControl();
	}
	CheckCollisionPlayerScreen();
}

void DrawPlayer(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawCircleShape(_renderWindow, player.circle, NULL);
	sfRenderWindow_drawRectangleShape(_renderWindow, player.rect, NULL);
	sfRenderWindow_drawVertexArray(_renderWindow, player.lineOfSight, NULL);
}

void CleanupPlayer(void)
{
	sfCircleShape_destroy(player.circle);
	sfVertexArray_destroy(player.lineOfSight);
	player = (Player){ 0 };
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

void MovePlayer(float _dt, sfVector2f _start, sfVector2f _target)
{
	sfVector2f direction = { 0 };

	if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		direction.x--;
	}
	if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		direction.x++;
	}

	if (sfKeyboard_isKeyPressed(sfKeyZ))
	{
		direction.y--;
	}
	if (sfKeyboard_isKeyPressed(sfKeyS))
	{
		direction.y++;
	}

	if (direction.x != 0 || direction.y != 0)
	{
		float angle = atan2f(direction.y, direction.x);

		sfCircleShape_move(player.circle, (sfVector2f) { player.speed* cosf(angle)* _dt, player.speed* sinf(angle)* _dt });
		sfRectangleShape_move(player.rect, (sfVector2f) { player.speed* cosf(angle)* _dt, player.speed* sinf(angle)* _dt });
	}
	float dx = _target.x - _start.x;
	float dy = _target.y - _start.y;

	float angleRect = atan2f(dy, dx) * (180.0f / (float)M_PI);
	if (angleRect > 90.0f || angleRect < -90)
	{
		sfRectangleShape_setFillColor(player.rect, sfRed);
	}
	else
	{
		sfRectangleShape_setFillColor(player.rect, sfBlue);
	}
	sfRectangleShape_setRotation(player.rect, angleRect);
}

void CheckCollisionPlayerScreen(void)
{
	sfFloatRect playerHitbox = sfCircleShape_getGlobalBounds(player.circle);

	if (playerHitbox.left < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0 - playerHitbox.left, 0 });
		sfRectangleShape_move(player.rect, (sfVector2f) { 0 - playerHitbox.left, 0 });

	}
	else if (playerHitbox.left + playerHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
		sfRectangleShape_move(player.rect, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
	}

	if (playerHitbox.top < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, 0 - playerHitbox.top });
		sfRectangleShape_move(player.rect, (sfVector2f) { 0, 0 - playerHitbox.top });
	}
	else if (playerHitbox.top + playerHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
		sfRectangleShape_move(player.rect, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
	}
}

sfVector2f GetPlayerPosition(void)
{
	return sfCircleShape_getPosition(player.circle);
}

void UpdatePlayerLineOfSight(sfVector2f _aimPosition)
{
	sfVertex* firstVertex = sfVertexArray_getVertex(player.lineOfSight, 0);
	firstVertex->position = sfCircleShape_getPosition(player.circle);

	sfVertex* lastVertex = sfVertexArray_getVertex(player.lineOfSight, 1);
	lastVertex->position = _aimPosition;
}

void UpdateCooldown(float _dt)
{
	if (!player.canShoot)
	{
		player.cooldown -= _dt;
		if (player.cooldown < 0)
		{
			player.cooldown += 1.f / FIRE_RATE;
			player.canShoot = sfTrue;
		}
	}
}

void UpdateFireControl(void)
{
	if (sfKeyboard_isKeyPressed(sfKeySpace))
	{
		if (player.canShoot && GetBulletCount() < BULLET_MAX)
		{
			sfVector2f shotPosition = GetPlayerPosition();
			AddBullet(shotPosition, GetAimPosition());

			player.canShoot = sfFalse;
		}
	}
	if (sfKeyboard_isKeyPressed(sfKeyLControl))
	{
		if (player.canShoot)
		{
			AddMissile(GetPlayerPosition());
			player.canShoot = sfFalse;
		}
	}
}