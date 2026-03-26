#include "Enemy.h"

Enemy enemyList[ENEMY_COUNT] = { 0 };

void MoveEnemy(unsigned _index, float _dt, sfRenderWindow* const _renderWindow);
void CheckCollisionEnemyScreen(unsigned _index);

void LoadEnemy(void)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		enemyList[i].circle = sfCircleShape_create();
		sfCircleShape_setRadius(enemyList[i].circle, ENEMY_RADIUS);
		sfCircleShape_setOrigin(enemyList[i].circle, (sfVector2f) { ENEMY_RADIUS, ENEMY_RADIUS });
		sfCircleShape_setFillColor(enemyList[i].circle, sfTransparent);
		sfCircleShape_setOutlineColor(enemyList[i].circle, sfGreen);
		sfCircleShape_setOutlineThickness(enemyList[i].circle, 2);
		sfCircleShape_setPointCount(enemyList[i].circle, 50);

		sfVector2f enemyPosition = { 0 };
		enemyPosition.x = (float)GetRandom(ENEMY_RADIUS, SCREEN_WIDTH - ENEMY_RADIUS - 1);
		enemyPosition.y = (float)GetRandom(ENEMY_RADIUS, SCREEN_HEIGHT - ENEMY_RADIUS - 1);
		sfCircleShape_setPosition(enemyList[i].circle, enemyPosition);

	
	}
}

void UpdateEnemy(sfRenderWindow* const _renderWindow, float _dt)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		MoveEnemy(i, _dt, _renderWindow);
		CheckCollisionEnemyScreen(i);
	}
}

void DrawEnemy(sfRenderWindow* const _renderWindow)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		sfRenderWindow_drawCircleShape(_renderWindow, enemyList[i].circle, NULL);
	}
}

void CleanupEnemy(void)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		sfCircleShape_destroy(enemyList[i].circle);
		enemyList[i] = (Enemy){ 0 };
	}
}

sfVector2f GetNearestEnemyPosition(sfVector2f _position)
{
	unsigned nearestEnemyIndex = 0;
	float minDistance = GetDistance(_position, sfCircleShape_getPosition(enemyList[0].circle));

	for (unsigned i = 1; i < ENEMY_COUNT; i++)
	{
		float distanceEnemy = GetDistance(_position, sfCircleShape_getPosition(enemyList[i].circle));
		if (distanceEnemy < minDistance)
		{
			minDistance = distanceEnemy;
			nearestEnemyIndex = i;
		}
	}

	return sfCircleShape_getPosition(enemyList[nearestEnemyIndex].circle);
}

void MoveEnemy(unsigned _index, float _dt, sfRenderWindow* const _renderWindow)
{
	sfVector2i mousePosI = sfMouse_getPositionRenderWindow(_renderWindow);
	sfVector2f mousePos = { (float)mousePosI.x, (float)mousePosI.y };
	sfCircleShape_setPosition(enemyList[_index].circle, mousePos);
}

void CheckCollisionEnemyScreen(unsigned _index)
{
	sfFloatRect enemyHitbox = sfCircleShape_getGlobalBounds(enemyList[_index].circle);

	if (enemyHitbox.left < 0)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0 - enemyHitbox.left, 0 });
		
	}
	else if (enemyHitbox.left + enemyHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { SCREEN_WIDTH - (enemyHitbox.left + enemyHitbox.width), 0 });
		
	}

	if (enemyHitbox.top < 0)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, 0 - enemyHitbox.top });
		
	}
	else if (enemyHitbox.top + enemyHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, SCREEN_HEIGHT - (enemyHitbox.top + enemyHitbox.height) });
		
	}
}
