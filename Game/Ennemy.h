#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

typedef struct Ennemy
{
	sfSprite* sprite;
}EnnemyEntity;

typedef struct
{
	EnnemyEntity* entity;
}Ennemy;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt);
void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void AddEnnemy(sfVector2f _position);
void HitEnnemy(unsigned _index);

#endif // !GAME_H
