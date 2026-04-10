#include "Player.h"

Player player = { 0 };

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color);
void MovePlayer(float _dt, sfVector2f _start, sfVector2f _target);
void MoveAxe(float _dt);
void UpdateCooldown(float _dt);
void UpdateEnergy(float _dt);
void UpdateFireControl(float _dt);
void CheckCollisionPlayerScreen(void);


void LoadPlayer(void)
{
	player.circle = sfCircleShape_create();
	sfCircleShape_setRadius(player.circle, PLAYER_RADIUS);
	sfCircleShape_setOrigin(player.circle, (sfVector2f) { PLAYER_RADIUS, PLAYER_RADIUS });
	sfCircleShape_setPosition(player.circle, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfCircleShape_setFillColor(player.circle, sfCyan);

	player.axe = sfRectangleShape_create();
	sfRectangleShape_setSize(player.axe, (sfVector2f) { AXE_RADIUS, PLAYER_RADIUS / 1.5f });
	sfFloatRect bounds = sfRectangleShape_getLocalBounds(player.axe);
	sfRectangleShape_setOrigin(player.axe, (sfVector2f) { 0, bounds.height / 2.0f });
	sfRectangleShape_setPosition(player.axe, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfRectangleShape_setFillColor(player.axe, sfBlue);
	sfRectangleShape_setRotation(player.axe, -180);
	player.isRighted = sfTrue;

	player.rangeBox = sfCircleShape_create();
	sfCircleShape_setRadius(player.rangeBox, RANGE_RADIUS);
	sfCircleShape_setOrigin(player.rangeBox, (sfVector2f) { RANGE_RADIUS, RANGE_RADIUS });
	sfCircleShape_setPosition(player.rangeBox, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	sfCircleShape_setFillColor(player.rangeBox, sfBlack);
	sfCircleShape_setOutlineThickness(player.rangeBox, 2.f);
	sfCircleShape_setOutlineColor(player.rangeBox, sfRed);

	player.speed = 500;
	player.canHit = sfTrue;
	player.cooldown = 1.f / FIRE_RATE;
	player.lineOfSight = CreateLineOfSight((sfVector2f) { 0, 0 }, (sfVector2f) { 0, 0 }, sfRed);
	player.isAttacking = sfFalse;
	player.pressTime = 0.f;
	player.energy = 10;
}

void UpdatePlayer(float _dt, sfVector2f _start, sfVector2f _target)
{
	MovePlayer(_dt, _start, _target);
	UpdateEnergy(_dt);
	if (!player.canHit)
	{
		UpdateCooldown(_dt);
		MoveAxe(_dt);
	}
	else
	{
		UpdateFireControl(_dt);
	}
	CheckCollisionPlayerScreen();
}

void DrawPlayer(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawCircleShape(_renderWindow, player.rangeBox, NULL);
	sfRenderWindow_drawCircleShape(_renderWindow, player.circle, NULL);
	sfRenderWindow_drawRectangleShape(_renderWindow, player.axe, NULL);
	sfRenderWindow_drawVertexArray(_renderWindow, player.lineOfSight, NULL);
}

void CleanupPlayer(void)
{
	sfCircleShape_destroy(player.circle);
	sfCircleShape_destroy(player.rangeBox);
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
		sfCircleShape_move(player.rangeBox, (sfVector2f) { player.speed* cosf(angle)* _dt, player.speed* sinf(angle)* _dt });
		sfRectangleShape_move(player.axe, (sfVector2f) { player.speed* cosf(angle)* _dt, player.speed* sinf(angle)* _dt });
	}
	float dx = _target.x - _start.x;
	float dy = _target.y - _start.y;

	float angleRect = atan2f(dy, dx) * (180.0f / (float)M_PI);
	if (player.canHit)
	{
		if (angleRect > 90.0f || angleRect < -90)
		{

			if (player.isRighted)
			{
				sfRectangleShape_setFillColor(player.axe, sfColor_fromRGB(160, 123, 255));
				player.isRighted = sfFalse;
			}
		}
		if (angleRect < 90.0f && angleRect > -90)
		{
			if (!player.isRighted)
			{

				sfRectangleShape_setFillColor(player.axe, sfBlue);
				player.isRighted = sfTrue;
			}
		}
		sfRectangleShape_setRotation(player.axe, angleRect);
	}
}

void CheckCollisionPlayerScreen(void)
{
	sfFloatRect playerHitbox = sfCircleShape_getGlobalBounds(player.circle);

	if (playerHitbox.left < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0 - playerHitbox.left, 0 });
		sfCircleShape_move(player.rangeBox, (sfVector2f) { 0 - playerHitbox.left, 0 });
		sfRectangleShape_move(player.axe, (sfVector2f) { 0 - playerHitbox.left, 0 });

	}
	else if (playerHitbox.left + playerHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
		sfCircleShape_move(player.rangeBox, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
		sfRectangleShape_move(player.axe, (sfVector2f) { SCREEN_WIDTH - (playerHitbox.left + playerHitbox.width), 0 });
	}

	if (playerHitbox.top < 0)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, 0 - playerHitbox.top });
		sfCircleShape_move(player.rangeBox, (sfVector2f) { 0, 0 - playerHitbox.top });
		sfRectangleShape_move(player.axe, (sfVector2f) { 0, 0 - playerHitbox.top });
	}
	else if (playerHitbox.top + playerHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(player.circle, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
		sfCircleShape_move(player.rangeBox, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
		sfRectangleShape_move(player.axe, (sfVector2f) { 0, SCREEN_HEIGHT - (playerHitbox.top + playerHitbox.height) });
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
	if (!player.canHit)
	{
		player.cooldown -= _dt;

		if (player.cooldown <= 0)
		{
			player.canHit = sfTrue;
			player.cooldown = MAX_COOLDOWN;
			player.isAttacking = sfFalse;
			player.attackType = NOATTACK;
			if (!player.isRighted)
			{
				sfRectangleShape_setFillColor(player.axe, sfColor_fromRGB(160, 123, 255));
			}
			if (player.isRighted)
			{
				sfRectangleShape_setFillColor(player.axe, sfBlue);
			}
		}
	}
}
void UpdateEnergy(float _dt)
{
	printf("%f.2 \t", player.energy);
	if (player.energy < 10.f)
	{
		player.energy += _dt;
	}
	if (player.energy > 10.f)
	{
		player.energy = 10.f;
	}
}
void UpdateFireControl(float _dt)
{
	if (player.canHit)
	{
		if (sfKeyboard_isKeyPressed(sfKeySpace))
		{
			player.pressTime += _dt;
		}
		if (!sfKeyboard_isKeyPressed(sfKeySpace) && player.pressTime > 0.f)
		{
			if (player.pressTime < 0.5f && player.energy < 2.5f)
			{
				player.attackType = LIGHT;
				if (player.isRighted)
					sfRectangleShape_rotate(player.axe, -(RANGE_ANGLE_LIGHT / 2.f));
				else
					sfRectangleShape_rotate(player.axe, RANGE_ANGLE_LIGHT / 2.f);
				sfRectangleShape_setFillColor(player.axe, sfGreen);
			}
			else if (player.pressTime < 0.5f && player.energy >= 2.5f)
			{
				player.attackType = MEDIUM;
				player.energy -= 2.5f;
				if (player.isRighted)
					sfRectangleShape_rotate(player.axe, -(RANGE_ANGLE_MEDIUM / 2.f));
				else
					sfRectangleShape_rotate(player.axe, RANGE_ANGLE_MEDIUM / 2.f);
				sfRectangleShape_setFillColor(player.axe, sfColor_fromRGB(255, 165, 0));

			}
			if (player.pressTime >= 0.5f && player.energy < 5)
			{
				player.attackType = MEDIUM;
				player.energy -= 2.5f;
				if (player.isRighted)
					sfRectangleShape_rotate(player.axe, -(RANGE_ANGLE_MEDIUM / 2.f));
				else
					sfRectangleShape_rotate(player.axe, RANGE_ANGLE_MEDIUM / 2.f);
				sfRectangleShape_setFillColor(player.axe, sfColor_fromRGB(255, 165, 0));


			}
			else if (player.pressTime >= 0.5f && player.energy >= 5)
			{
				player.attackType = HEAVY;
				player.energy -= 5.f;
				if (player.isRighted)
					sfRectangleShape_rotate(player.axe, -(RANGE_ANGLE_MEDIUM / 2.f));
				else
					sfRectangleShape_rotate(player.axe, RANGE_ANGLE_MEDIUM / 2.f);
				sfRectangleShape_setFillColor(player.axe, sfRed);
			

			}
			player.isAttacking = sfTrue;
			player.canHit = sfFalse;
			player.pressTime = 0;
		}
	}
}

sfBool IsPlayerAttacking(void)
{
	return player.isAttacking;
}

AttackType GetPlayerAttack(void)
{
	return player.attackType;
}

void ChangePlayerState(void)
{
	player.isAttacking = sfFalse;
}

void MoveAxe(float _dt)
{
	if (!player.canHit)
	{
		float range = 0;
		if (player.attackType == LIGHT)
		{
			range = RANGE_ANGLE_LIGHT;
		}
		else if (player.attackType == MEDIUM)
		{
			range = RANGE_ANGLE_MEDIUM;
		}
		else
		{
			range = RANGE_ANGLE_HEAVY;
		}
		float frameRotation = (range / MAX_COOLDOWN) * _dt;

		if (player.isRighted)
			sfRectangleShape_rotate(player.axe, frameRotation);
		else
			sfRectangleShape_rotate(player.axe, -frameRotation);
	}
}

sfRectangleShape* GetAxe(void)
{
	return player.axe;
}
