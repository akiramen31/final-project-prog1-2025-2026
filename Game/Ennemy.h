#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

typedef enum Type
{
	SOLDIER,
	FLYER
};

typedef struct EnnemyEntity
{
	sfSprite* sprite;
	float life;
	enum Type type;
	float energy;
	sfVector2f move;
}EnnemyEntity;

typedef struct Ennemy
{
	EnnemyEntity* entity;
}Ennemy;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt);
void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CrateEnnemy();
void AddEnnemy(sfVector2f _position);
void HitEnnemy(unsigned _index);

#endif // !GAME_H
