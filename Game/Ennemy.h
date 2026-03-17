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
	TOTAL_Type_Ennemy
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
}Ennemy;

void LoadEnnemy(void);
Ennemy* CreateRandomEnnemy(void);
Ennemy* CreateSelectEnnemy(enum TypeEnnemy _type);



//sfRectangleShape_getGlobalBounds;

#endif // !ENNEMY_h
