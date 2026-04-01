#ifndef MISSILE_H
#define MISSILE_H

#include "Common.h"

#define MISSILE_RADIUS 25
#define MISSILE_MAX 3
#define MISSILE_DURATION 10
#define SPEED_MISSILE 400.0f
#define DEGRE_ROTATION 180.0f
#define PI 3.1415

typedef struct Missile
{
	sfCircleShape* circle;
	float rotation;
	float angle;
	float speed;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Missile;

void LoadMissile(void);
void UpdateMissile(sfRenderWindow* const _renderWindow,sfVector2f _playerPos, float _dt);
void DrawMissile(sfRenderWindow* const _renderWindow);
void CleanupMissile(void);
void AddMissile(sfVector2f _playerPos);
void CheckCollisionMissilesList(void);
CircleHitbox GetMissileCircleHitbox(int _index);

#endif // !ENEMY_H