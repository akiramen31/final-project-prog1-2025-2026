#ifndef SECONDARIES_H
#define SECONDARIES_H

#include "Common.h"
#include "Box.h"

#define MISSILE_HEIGHT 6
#define MISSILE_WEIGHT 8
#define MISSILE_MAX 3

#define SECONDARY_PROJECTILE_DURATION 3

#define SPEED_MISSILE 180.0f
#define DEGRE_ROTATION 300.0f
#define PI 3.1415

typedef enum SecondaryType
{
	DRONE,
	COLDBREATH
}SecondaryType;

typedef struct Drone
{
	sfSprite* sprite;
	sfMusic* ambientSound;
	float rotation;
	float turnSpeed;
	float lifetime;
	sfBool isAlive;
}Drone;

typedef struct ColdBreath
{
	sfSprite* sprite;
	sfMusic* music;
	sfVector2f velocity;
	float lifetime;
	sfBool isAlive;
}ColdBreath;

typedef struct SecondaryData
{
	Drone droneList[MISSILE_MAX];
	ColdBreath coldBreath;
	SecondaryType secondaryType;
	sfTexture* droneTexture;
}SecondaryData;

void LoadSecondary(void);
void AddDrone(sfVector2f _pos, sfBool _isRighted);
void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType);
void UpdateSecondary(sfVector2f _pos,float _dt);
void UpdateDrone(sfVector2f _posAim, float _dt);
void UpdateColdBreath(float _dt);
void MoveDrone(unsigned _index, sfVector2f _playerPos, float _dt);

void CheckCollisionMissilesList(void);

SecondaryType GetSecondaryType(void);
void SetSecondaryType(SecondaryType _type);

#endif // !SECONDARIES_H