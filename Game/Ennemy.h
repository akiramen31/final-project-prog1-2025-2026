#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

enum Vitesse
{
	TRES_LENT = 20,
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
	Animation animation[6];
	sfVector2i position;
	enum Direction direction;
}Ennemy;

void LoadEnnemy(void);
void CreateRandomEnnemy(Ennemy* _ennemy);
void CreateSelectEnnemy(enum TypeEnnemy _type);
void AddEnnemy(sfVector2i _position, CasePosibility _casePosibility);
Ennemy* GetEnnemy(unsigned _index);
void UpdateEnnemy(float _dt, CasePosibility _casePosibility,int _i);
unsigned GetAnimation(Ennemy* _ennemy);
int GetNumberEnnemy(void);
sfVector2i GetPositionEnnemy(unsigned _index);
void NewChoiceDirection(CasePosibility _casePosibility, int _i);
void NewChoiceDirectionV2(int _i);
sfVector2i GetFuturPositionEnnemy(unsigned _index);
void HitEnnemy(unsigned _index);

#endif // !ENNEMY_h
