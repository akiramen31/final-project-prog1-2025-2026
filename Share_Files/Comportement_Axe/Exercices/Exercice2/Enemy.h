#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

#define ENEMY_RADIUS 20
#define ENEMY_COUNT 5
#define RESPAWN_TIMER 10

typedef struct Enemy
{
	sfCircleShape* circle;
	sfVector2f direction;
	float speed;
	float respawnTimer;
	sfBool isAlive;
}Enemy;

void LoadEnemy(void);
void UpdateEnemy(sfRenderWindow* const _renderWindow, float _dt);
void DrawEnemy(sfRenderWindow* const _renderWindow);
void CleanupEnemy(void);

sfCircleShape* GetEnemy(int _index);

int GetNearestEnemyPosition(sfVector2f _position);
sfVector2f GetEnemyPosition(int _index);
void ChangeStateEnemy(int _index);

#endif // !ENEMY_H
