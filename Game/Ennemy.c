#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CreateEnnemy(EnnemyEntity* _ennemy, Type _type);
void CalculMoveEnnemy(float _dt, int _index);

List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
void LoadEnnemy(void)
{
	listEnnemy = CreateList();
	if (DEV_MODE)
	{
		GetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY);
	}
	else
	{
		ennemyEntity[0].type = 0;
		ennemyEntity[0].life = 20.f;
		ennemyEntity[0].energyMax = 500.f;
		ennemyEntity[0].energy = 500.f;
		ennemyEntity[0].speedMax = 10.f;
		ennemyEntity[0].accelerationMax = 30.f;
		ennemyEntity[0].jumForce = 500.f;

		ennemyEntity[1].type = 1;
		ennemyEntity[1].life = 10.f;
		ennemyEntity[1].energyMax = 800.f;
		ennemyEntity[1].energy = 800.f;
		ennemyEntity[1].speedMax = 7.f;
		ennemyEntity[1].accelerationMax = 30.f;
		ennemyEntity[1].jumForce = 500.f;
	}
	//SetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de ennemyEntity
}

void UpdateEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	CalculMoveEnnemy(_dt, _index);
	sfSprite_move(ennemy->sprite, ennemy->ennemyEntity.move);
}

void CreateEnnemyRandom(EnnemyEntity* _ennemy)
{
	enum Type temp = rand() % ALEATORY;
	switch (temp)
	{
	case SOLDIER:
		CreateEnnemy(_ennemy, 0);
		break;
	case FLYER:
		CreateEnnemy(_ennemy, 1);
		break;
	default:
		break;
	}
}

void CreateEnnemy(EnnemyEntity* _ennemy, Type _type)
{
	if (DEV_MODE)
	{
		printf("création d'un ennemy de type %d\n", _type);
	}
	_ennemy->type = _type;
	_ennemy->life = ennemyEntity[_type].life;
	_ennemy->energyMax = ennemyEntity[_type].energy;
	_ennemy->energy = ennemyEntity[_type].energy;
	_ennemy->speedMax = ennemyEntity[_type].speedMax;
	_ennemy->jumForce = ennemyEntity[_type].jumForce;
	_ennemy->accelerationMax = ennemyEntity[_type].accelerationMax;
	_ennemy->acceleration = (sfVector2f){ 0,0 };
	_ennemy->move = (sfVector2f){ 0,0 };
	_ennemy->state = IDLE;
}

void CalculMoveEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.acceleration = (sfVector2f){ 0,0 };
	if (sfKeyboard_isKeyPressed(sfKeyRight))
	{
		ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyLeft))
	{
		ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyUp))
	{
		ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.jumForce;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown))
	{
		ennemy->ennemyEntity.acceleration.y += ennemy->ennemyEntity.accelerationMax;
	}
	ennemy->ennemyEntity.acceleration.y += G;
	ennemy->ennemyEntity.move.x += ennemy->ennemyEntity.acceleration.x * _dt;
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

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/E.png"), _position, 1, 1);

	InsertElement(listEnnemy, element, 0);
}

void HitEnnemy(unsigned _index)
{

}