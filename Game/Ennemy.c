#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CreateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy, float _speedMax, float _accelerationMax);
void CalculMoveEnnemy(float _dt, int _index);

List* listEnnemy;

void LoadEnnemy(void)
{
	listEnnemy = CreateList();
}

void UpdateEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	CalculMoveEnnemy(_dt, _index);
	sfSprite_move(ennemy->sprite, ennemy->ennemyEntity.move);
}

void CreateEnnemyRandom(EnnemyEntity* _ennemy)
{
	enum Type temp = rand()% ALEATORY;
	switch (temp)
	{
	case SOLDIER:
		CreateEnnemy(_ennemy, 0, 20.f, 500.f, 10.f, 30.f);
		break;
	case FLYER:
		CreateEnnemy(_ennemy, 1, 10.f, 800.f, 7.f, 30.f);
		break;
	default:
		break;
	}
}

void CreateEnnemy(EnnemyEntity* _ennemy, Type _type, float _life, float _energy, float _speedMax, float _accelerationMax)
{
	if (DEV_MODE)
	{
		printf("création d'un ennemy de type %d\n", _type);
	}
	_ennemy->type = _type;
	_ennemy->life = _life;
	_ennemy->energyMax = _energy;
	_ennemy->energy = _energy;
	_ennemy->speedMax = _speedMax;
	_ennemy->accelerationMax = _accelerationMax;
	_ennemy->acceleration = (sfVector2f){ 0,0 };
	_ennemy->move = (sfVector2f){ 0,0 };
	_ennemy->state = IDLE;
}

void CalculMoveEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.acceleration = (sfVector2f){0,0};
	//ennemy->ennemyEntity.acceleration.y += GRAVITY;
	if (sfKeyboard_isKeyPressed(sfKeyRight))
	{
		ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.accelerationMax ;
	}
	if (sfKeyboard_isKeyPressed(sfKeyLeft))
	{
		ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyUp))
	{
		ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown))
	{
		ennemy->ennemyEntity.acceleration.y += ennemy->ennemyEntity.accelerationMax;
	}
	ennemy->ennemyEntity.move.x += ennemy->ennemyEntity.acceleration.x*_dt;
	ennemy->ennemyEntity.move.y += ennemy->ennemyEntity.acceleration.y * _dt;

	if (ennemy->ennemyEntity.move.x > ennemy->ennemyEntity.speedMax)
	{
		ennemy->ennemyEntity.move.x = ennemy->ennemyEntity.speedMax;
	}
	else if (ennemy->ennemyEntity.move.x < -ennemy->ennemyEntity.speedMax)
	{
		ennemy->ennemyEntity.move.x = -ennemy->ennemyEntity.speedMax;
	}

	if (ennemy->ennemyEntity.move.y > ennemy->ennemyEntity.speedMax)
	{
		ennemy->ennemyEntity.move.y = ennemy->ennemyEntity.speedMax;
	}
	else if (ennemy->ennemyEntity.move.y < -ennemy->ennemyEntity.speedMax)
	{
		ennemy->ennemyEntity.move.y = -ennemy->ennemyEntity.speedMax;
	}
	
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

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/E.png"),_position,1,1);

	InsertElement(listEnnemy, element, 0);
}

void HitEnnemy(unsigned _index)
{
	
}