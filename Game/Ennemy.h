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
	float timer;
}EnnemyEntity;

typedef struct ActionDemander
{
	sfBool gauche;
	sfBool droite;
	sfBool Saut;
}ActionDemander;

typedef struct Ennemy
{
	EnnemyEntity ennemyEntity;
	ActionDemander actiondemander;
	sfSprite* sprite;
	sfImage* imageColideur;
}Ennemy;

typedef struct Case
{
	float rangeToDestination;
	float action;
	float Résultat;
	float energie;
	enum Direction direction;
}Case;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt, int _index);
void AddEnnemy(sfVector2f _position, enum Type _type);
sfBool HitEnnemy(unsigned _index, sfVector2f _touch, float _degat);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);


#endif // !GAME_H
