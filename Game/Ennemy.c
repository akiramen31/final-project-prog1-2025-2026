#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CreateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy);

List* listEnnemy;

void LoadEnnemy(void)
{
	listEnnemy = CreateList();
}

void UpdateEnnemy(float _dt)
{

}

void CreateEnnemyRandom(EnnemyEntity* _ennemy)
{
	enum Type temp = rand()% ALEATORY;
	switch (temp)
	{
	case SOLDIER:
		CreateEnnemy(_ennemy, 0, 20, 500);
		break;
	case FLYER:
		CreateEnnemy(_ennemy, 1, 10, 800);
		break;
	default:
		break;
	}
}

void CreateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy)
{
	_ennemy->type = _type;
	_ennemy->life = _life;
	_ennemy->energyMax = _energy;
	_ennemy->energy = _energy;
	_ennemy->move = (sfVector2f){ 0,0 };
	_ennemy->state = IDLE;
}

void AddEnnemy(sfVector2f _position, enum Type _type)
{
	Ennemy* ennemy = Calloc(1, sizeof(Ennemy));
	
	Element* element = CreateElement(ennemy);
	element->value = ennemy;
	
	switch (_type)
	{
	case SOLDIER:
		break;
	case FLYER:
		break;
	case ALEATORY:
		CreateEnnemyRandom(&ennemy->ennemyEntity);
		break;
	default:
		break;
	}

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/E.png"),(sfVector2f){100,100},1,1);

	InsertElement(listEnnemy, element, 0);
}

void HitEnnemy(unsigned _index)
{
	
}