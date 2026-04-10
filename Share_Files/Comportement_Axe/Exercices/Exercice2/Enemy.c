#include "Enemy.h"

Enemy enemyList[ENEMY_COUNT] = { 0 };

//void CheckCollisionEnemyScreen(unsigned _index);

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

		enemyList[i].speed = 300;
		enemyList[i].direction = GetDirectionFromDegree((float)(rand() % 360));
		enemyList[i].respawnTimer = 0;
		enemyList[i].isAlive = sfTrue;
	}
}

void UpdateEnemy(sfRenderWindow* const _renderWindow, float _dt)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		if (!enemyList[i].isAlive)
		{
			enemyList[i].respawnTimer += _dt;
			if (enemyList[i].respawnTimer > RESPAWN_TIMER)
			{
				enemyList[i].isAlive = sfTrue;
				enemyList[i].respawnTimer = 0;
			}
		}
	}
}

void DrawEnemy(sfRenderWindow* const _renderWindow)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemyList[i].isAlive)
		{
			sfRenderWindow_drawCircleShape(_renderWindow, enemyList[i].circle, NULL);
		}
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

int GetNearestEnemyPosition(sfVector2f _position)
{
	unsigned nearestEnemyIndex = -1;
	float minDistance = 3.14 * 1000;

	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemyList[i].isAlive)
		{
			float distanceEnemy = GetDistance(_position, sfCircleShape_getPosition(enemyList[i].circle));
			if (distanceEnemy < minDistance)
			{
				minDistance = distanceEnemy;
				nearestEnemyIndex = i;
			}
		}
	}

	return nearestEnemyIndex;
}

sfVector2f GetEnemyPosition(int _index)
{
	return sfCircleShape_getPosition(enemyList[_index].circle);
}

//void CheckCollisionEnemyScreen(unsigned _index)
//{
//	sfFloatRect enemyHitbox = sfCircleShape_getGlobalBounds(enemyList[_index].circle);
//
//	if (enemyHitbox.left < 0)
//	{
//		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0 - enemyHitbox.left, 0 });
//		enemyList[_index].direction.x *= -1;
//	}
//	else if (enemyHitbox.left + enemyHitbox.width > SCREEN_WIDTH)
//	{
//		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { SCREEN_WIDTH - (enemyHitbox.left + enemyHitbox.width), 0 });
//		enemyList[_index].direction.x *= -1;
//	}
//
//	if (enemyHitbox.top < 0)
//	{
//		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, 0 - enemyHitbox.top });
//		enemyList[_index].direction.y *= -1;
//	}
//	else if (enemyHitbox.top + enemyHitbox.height > SCREEN_HEIGHT)
//	{
//		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, SCREEN_HEIGHT - (enemyHitbox.top + enemyHitbox.height) });
//		enemyList[_index].direction.y *= -1;
//	}
//}

void ChangeStateEnemy(int _index)
{
	enemyList[_index].isAlive = sfFalse;
	printf("BOOM il est mort le numero %d", (_index + 1));
}

sfCircleShape* GetEnemy(int _index)
{
	return enemyList[_index].circle;
}