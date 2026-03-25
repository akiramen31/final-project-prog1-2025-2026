#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Scene.h"

typedef enum Type
{
	SOLDIER,
	FLYER,
	ALEATORY
}Type;

typedef struct Jetpack
{
	float trust;
	float consomation;
	float life;
} Jetpack;

typedef struct EnnemyData
{
	float life;
	float energyMax;
	float energy;
	float energyRegen;
	float accelerationMax;
	float speedMax;
	float jumForce;
} EnnemyData;

typedef struct EnnemyEntity
{
	enum Type type;
	struct EnnemyData ennemydata;
	sfBool isJetpack;
	struct Jetpack jetpack;
	sfVector2f acceleration;
	sfVector2f move;
	enum State state;
}EnnemyEntity;

typedef struct Ennemy
{
	EnnemyEntity ennemyEntity;
	sfSprite* sprite;
	sfImage* imageColideur;
}Ennemy;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt, int _index);
void AddEnnemy(sfVector2f _position, enum Type _type);
sfBool HitEnnemy(unsigned _index, sfVector2f _touch, float _degat);


#endif // !GAME_H
