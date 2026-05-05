#ifndef MISSILE_H
#define MISSILE_H

#include "Common.h"
#include "Box.h"

#define MISSILE_HEIGHT 6
#define MISSILE_WEIGHT 8
#define MISSILE_MAX 3

#define SECONDARY_PROJECTILE_DURATION 3

#define SPEED_MISSILE 180.0f
#define DEGRE_ROTATION 300.0f
#define PI 3.1415

typedef struct Missile
{
	sfSprite* sprite;
	sfMusic* music;
	float rotation;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Missile;

typedef struct ColdBreath
{
	sfSprite* sprite;
	sfMusic* music;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
}ColdBreath;

void LoadMissile(void);

void UpdateMissile(sfVector2f _pos,float _dt);
void UpdateSecondary(sfVector2f _posAim, float _dt);
void AddMissile(sfVector2f _pos, sfBool _isRighted);
void CheckCollisionMissilesList(void);

void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void UpdateColdBreath(float _dt);

Secondary GetSecondary(void);

#endif // !ENEMY_H