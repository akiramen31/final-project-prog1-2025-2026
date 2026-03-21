#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"

#define ENNEMY_TIME_OF_INVINCIBLE 1.f

typedef enum Vitesse
{
	TRES_LENT = 10,
	LENT = 20,
	NORMAL = 50
}Vitesse;

typedef enum TypeEnnemy
{
	BALLOM,
	DALL,
	KONDORIA,
	DENKYUN,
	ONIL,
	TOTAL_TYPE_ENNEMY
}TypeEnnemy;

typedef struct Personality
{
	int aggressiveness;
	int chaos;
}Personality;

typedef struct Ennemy
{
	sfSprite* sprite;
	int life;
	TypeEnnemy typeEnnemy;
	int points;
	Vitesse vitesse;
	sfBool passeMuraille;
	Personality personality;
	Animation animation[6];
	sfVector2i position;
	Direction direction;
	float invincibleTimer;
}Ennemy;

void LoadEnnemy(void);
void CreateEnnemyRandom(Ennemy* _ennemy);
void CreateEnnemySelect(enum TypeEnnemy _type);
void AddEnnemy(sfVector2i _position, CasePosibility _casePosibility);
Ennemy* GetEnnemy(unsigned _index);
void UpdateEnnemy(float _dt, CasePosibility _casePosibility, int _i);
unsigned GetEnnemyAnimation(Ennemy* _ennemy);
int GetEnnemyCount(void);
sfVector2i GetEnnemyPosition(unsigned _index);
void ChooseNewDirection(CasePosibility _casePosibility, int _i);
void ChooseNewDirectionV2(int _i);
sfVector2i GetEnnemyNextPosition(unsigned _index);
void HitEnnemy(unsigned _index);

#endif // !ENNEMY_h
