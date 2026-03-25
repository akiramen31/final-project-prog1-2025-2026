#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CreateEnnemy(EnnemyEntity* _ennemy, Type _type);
void CalculMoveEnnemy(float _dt, int _index);
Direction AStar(int _index, sfVector2f _positionCible);
float CalculResultAStar(Case _case);


List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
HitboxMap* hitboxMap;
Case** aStarMap;
List* listeWait;

void LoadEnnemy(void)
{
	listEnnemy = CreateList();
	listeWait = CreateList();
	if (!DEV_MODE)
	{
		GetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY);
	}
	else
	{
		ennemyEntity[0].type = 0;
		ennemyEntity[0].ennemydata.life = 20.f;
		ennemyEntity[0].ennemydata.energyMax = 500.f;
		ennemyEntity[0].ennemydata.energy = 500.f;
		ennemyEntity[0].ennemydata.energyRegen = 50.f;
		ennemyEntity[0].ennemydata.speedMax = 10.f;
		ennemyEntity[0].ennemydata.accelerationMax = 30.f;
		ennemyEntity[0].ennemydata.jumForce = 500.f;

		ennemyEntity[0].isJetpack = sfFalse;
		ennemyEntity[0].jetpack.consomation = 50.f;
		ennemyEntity[0].jetpack.life = 5.f;
		ennemyEntity[0].jetpack.trust = 10.f;


		ennemyEntity[1].type = 1;
		ennemyEntity[1].ennemydata.life = 10.f;
		ennemyEntity[1].ennemydata.energyMax = 800.f;
		ennemyEntity[1].ennemydata.energy = 800.f;
		ennemyEntity[1].ennemydata.energyRegen = 50.f;
		ennemyEntity[1].ennemydata.speedMax = 7.f;
		ennemyEntity[1].ennemydata.accelerationMax = 30.f;
		ennemyEntity[1].ennemydata.jumForce = 500.f;

		ennemyEntity[1].isJetpack = sfTrue;
		ennemyEntity[1].jetpack.consomation = 20.f;
		ennemyEntity[1].jetpack.life = 5.f;
		ennemyEntity[1].jetpack.trust = 20.f;
	}
	SetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de ennemyEntity
	hitboxMap = GetSceneImageHitbox();
	printf("ratio %d aille x%d   y%d\n", hitboxMap->ratio, hitboxMap->size.x, hitboxMap->size.y);
	aStarMap = CreateGrid(hitboxMap->size,sizeof(Case));
}

void UpdateEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.ennemydata.energyRegen += ennemy->ennemyEntity.ennemydata.energyRegen;
	CalculMoveEnnemy(_dt, _index);
	sfSprite_move(ennemy->sprite, ennemy->ennemyEntity.move);
	if (ennemy->ennemyEntity.ennemydata.energy > ennemy->ennemyEntity.ennemydata.energyMax)
	{
		ennemy->ennemyEntity.ennemydata.energy = ennemy->ennemyEntity.ennemydata.energyMax;
	}
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
	_ennemy->ennemydata = ennemyEntity[_type].ennemydata;
	_ennemy->acceleration = (sfVector2f){ 0,0 };
	_ennemy->move = (sfVector2f){ 0,0 };
	_ennemy->state = IDLE;
	_ennemy->timer = 0.f;

	_ennemy->isJetpack = ennemyEntity[_type].isJetpack;
	if (_ennemy->isJetpack)
	{
		_ennemy->jetpack = ennemyEntity[_type].jetpack;
	}
}

void CalculMoveEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.acceleration = (sfVector2f){ 0,0 };
	if (sfKeyboard_isKeyPressed(sfKeyRight))
	{
		ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.ennemydata.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyLeft))
	{
		ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.ennemydata.accelerationMax;
	}
	if (sfKeyboard_isKeyPressed(sfKeyUp))
	{
		ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.ennemydata.jumForce;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown))
	{
		ennemy->ennemyEntity.acceleration.y += ennemy->ennemyEntity.ennemydata.accelerationMax;
	}
	ennemy->ennemyEntity.acceleration.y += G;
	ennemy->ennemyEntity.move.x += ennemy->ennemyEntity.acceleration.x * _dt;
	ennemy->ennemyEntity.move.y += ennemy->ennemyEntity.acceleration.y * _dt;

	if (ennemy->ennemyEntity.move.x > ennemy->ennemyEntity.ennemydata.speedMax)
	{
		ennemy->ennemyEntity.move.x = ennemy->ennemyEntity.ennemydata.speedMax;
	}
	else if (ennemy->ennemyEntity.move.x < -ennemy->ennemyEntity.ennemydata.speedMax)
	{
		ennemy->ennemyEntity.move.x = -ennemy->ennemyEntity.ennemydata.speedMax;
	}

	if (ennemy->ennemyEntity.move.y > ennemy->ennemyEntity.ennemydata.speedMax)
	{
		ennemy->ennemyEntity.move.y = ennemy->ennemyEntity.ennemydata.speedMax;
	}
	else if (ennemy->ennemyEntity.move.y < -ennemy->ennemyEntity.ennemydata.speedMax)
	{
		ennemy->ennemyEntity.move.y = -ennemy->ennemyEntity.ennemydata.speedMax;
	}

}

Direction AStar(int _index, sfVector2f _positionCible)
{
	sfVector2u positionCibleCase = RealPositionConvertTableauPosition(_positionCible);
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfVector2u positionDebutCase = RealPositionConvertTableauPosition(sfSprite_getPosition(ennemy->sprite));
	int x = 0;
	int y = 0;
	// reset du tableau
	for (unsigned y = 0; y < hitboxMap->size.y; y++)
	{
		for (unsigned x = 0; x < hitboxMap->size.x; x++)
		{
			aStarMap[y][x] = (Case){0};
		}
	}
	
	sfVector2u* emplacement = Calloc(1, sizeof(sfVector2u));
	Element* element = CreateElement(ennemy);
	element->value = emplacement;
	*emplacement = positionDebutCase;
	
	aStarMap[positionDebutCase.y][positionDebutCase.x].action = 0.f;
	aStarMap[positionDebutCase.y][positionDebutCase.x].direction = NO_DIRECTION;
	//AStarMap[positionDebutCase.y][positionDebutCase.x].rangeToDestination = ;
	aStarMap[positionDebutCase.y][positionDebutCase.x].Résultat = CalculResultAStar(aStarMap[positionDebutCase.y][positionDebutCase.x]);

	sfBool flag = sfFalse;

	while (GetListSize(listeWait) && flag == sfFalse)
	{

	}


	//return Direction();
}

float CalculResultAStar(Case _case)
{
	return (float) { _case.rangeToDestination + _case.action + (_case.energie * 0.2) };
}

sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal)
{
	printf("ratio %d\n", hitboxMap->ratio);
	_positionReal.x = _positionReal.x / hitboxMap->ratio;
	_positionReal.y = _positionReal.y / hitboxMap->ratio;
	sfVector2u newposition = { (unsigned)_positionReal.x, (unsigned)_positionReal.y };
	return newposition;
}

void AddEnnemy(sfVector2f _position, enum Type _type)
{
	Ennemy* ennemy = Calloc(1, sizeof(Ennemy));

	Element* element = CreateElement(ennemy);
	element->value = ennemy;

	switch (_type)
	{
	case SOLDIER:
		CreateEnnemy(&ennemy->ennemyEntity, 0);
		break;
	case FLYER:
		CreateEnnemy(&ennemy->ennemyEntity, 1);
		break;
	case ALEATORY:
		CreateEnnemyRandom(&ennemy->ennemyEntity);
		break;
	default:
		break;
	}

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/E.png"), _position, 1, 1);
	ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/E.png"));

	InsertElement(listEnnemy, element, 0);
}

sfBool HitEnnemy(unsigned _index, sfVector2f _touch, float _degat)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfColor pixelColor = sfImage_getPixel(ennemy->imageColideur, (int)_touch.x, (int)_touch.y);
	sfBool isTouch = sfFalse;
	if (pixelColor.a == 255)
	{
		isTouch = sfTrue;
		if (DEV_MODE)
		{
			printf("Ennemie %d Toucher fait %f degat", _index, _degat);
		}
		ennemy->ennemyEntity.ennemydata.life -= _degat;
		if (ennemy->ennemyEntity.ennemydata.life < 0)
		{
			ennemy->ennemyEntity.ennemydata.life = 0;
		}
	}
	return isTouch;
}