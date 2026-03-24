#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CrateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy);

Ennemy ennemy;

void LoadEnnemy(void)
{
	ennemy = (Ennemy){ 0 };
}

void UpdateEnnemy(float _dt)
{

}

void CreateEnnemyRandom(EnnemyEntity* _ennemy)
{
	enum Type temp;
	switch (temp)
	{
	case SOLDIER:
		CreateEnnemyRandom(_ennemy, 0, 20, 500);
		break;
	case FLYER:
		CreateEnnemyRandom(_ennemy, 1, 10, 800);
		break;
	default:
		break;
	}
}

void CrateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy)
{
	_ennemy->type = _type;
	_ennemy->life = _life;
	_ennemy->energyMax = _energy;
	_ennemy->energy = _energy;
	_ennemy->move = (sfVector2f){ 0,0 };
}

void AddEnnemy(sfVector2f _position, enum Type _type)
{
	enum Type temp;
	switch (temp)
	{
	case SOLDIER:
		break;
	case FLYER:
		break;
	case TOTAL:
		break;
	default:
		break;
	}
}

void HitEnnemy(unsigned _index)
{
	
}