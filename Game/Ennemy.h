#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

enum Vitesse
{
	TRES_LENT = 5,
	LENT = 20,
	NORMAL = 50
};

enum TypeEnnemy
{
	BALLOM,
	ONIL,
	DALL,
	DENKYUN,
	KONDORIA,
	TOTAL_TYPE_ENNEMY
};

typedef struct Personality
{
	int aggressiveness;
	int chaos;
}Personality;

typedef struct Ennemy
{
	sfSprite* sprite;
	int life;
	enum TypeEnnemy typeEnnemy;
	int points;
	enum Vitesse vitesse;
	_Bool passeMuraille;
	Personality personality;
	Animation animation[5];
	sfVector2i position;
	enum Direction direction;
}Ennemy;

void LoadEnnemy(void);
void CreateRandomEnnemy(Ennemy* _ennemy);
void CreateSelectEnnemy(enum TypeEnnemy _type);
void AddEnnemy(sfVector2i _position);
Ennemy* GetEnnemy(unsigned _index);
void UpdateEnnemy(float _dt);
unsigned GetAnimation(Ennemy* _ennemy);
int GetNumberEnnemy(void);
sfVector2i GetPositionEnnemy(unsigned _index);

#endif // !ENNEMY_h
