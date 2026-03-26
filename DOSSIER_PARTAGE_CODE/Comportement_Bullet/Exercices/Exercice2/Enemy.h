#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

#define ENEMY_RADIUS 20
#define ENEMY_COUNT 1

typedef struct Enemy
{
	sfCircleShape* circle;
}Enemy;

void LoadEnemy(void);
void UpdateEnemy(sfRenderWindow* const _renderWindow, float _dt);
void DrawEnemy(sfRenderWindow* const _renderWindow);
void CleanupEnemy(void);

sfVector2f GetNearestEnemyPosition(sfVector2f _position);

#endif // !ENEMY_H
