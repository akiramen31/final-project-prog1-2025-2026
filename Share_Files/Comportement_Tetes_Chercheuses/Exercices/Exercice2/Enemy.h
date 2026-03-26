#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

#define ENEMY_RADIUS 25
#define ENEMY_COUNT 1
#define SPEED_MISSILE 400.0f
#define DEGRE_ROTATION 90.0f
#define PI 3.1415

typedef struct Enemy
{
	sfCircleShape* circle;
	float rotation;
	float angle;
	float speed;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Enemy;

void LoadEnemy(void);
void UpdateEnemy(sfRenderWindow* const _renderWindow,sfVector2f _playerPos, float _dt);
void DrawEnemy(sfRenderWindow* const _renderWindow);
void CleanupEnemy(void);

sfVector2f GetNearestEnemyPosition(sfVector2f _position);

#endif // !ENEMY_H
