#ifndef MISSILE_H
#define MISSILE_H

#include "Common.h"
#include "Box.h"

#define MISSILE_HEIGHT 6
#define MISSILE_WEIGHT 8
#define MISSILE_MAX 3
#define MISSILE_DURATION 3
#define SPEED_MISSILE 200.0f
#define DEGRE_ROTATION 280.0f
#define PI 3.1415

typedef struct Missile
{
	sfSprite* sprite;
	float rotation;
	float angle;
	float speed;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Missile;

void LoadMissile(void);
void UpdateMissile(sfVector2f _pos,float _dt);
void AddMissile(sfVector2f _pos);
void CheckCollisionMissilesList(void);

#endif // !ENEMY_H