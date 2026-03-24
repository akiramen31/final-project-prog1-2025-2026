#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

typedef enum Type
{
	SOLDIER,
	FLYER,
	ALEATORY
}Type;

typedef struct EnnemyEntity
{
	enum Type type;
	float life;
	float energyMax;
	float energy;
	sfVector2f move;
	enum State state;
}EnnemyEntity;

typedef struct Ennemy
{
	EnnemyEntity ennemyEntity;
	sfSprite* sprite;
}Ennemy;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt);
void AddEnnemy(sfVector2f _position, enum Type _type);
void HitEnnemy(unsigned _index);

#endif // !GAME_H
