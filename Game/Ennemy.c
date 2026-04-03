#include "Ennemy.h"

void CreateEnnemyRandom(EnnemyEntity* _ennemy);
void CreateEnnemy(EnnemyEntity* _ennemy, Type _type);
void CalculMoveEnnemy(float _dt, int _index);
ActionDemander AStar(int _index, sfVector2f _positionCible);
float CalculResultAStar(Case _case);
int MinResultCase(void);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);


List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
MapData* mapData;
Case** aStarMap;
List* listeWait;


sfSprite* sprite;
sfTexture* texture;

void LoadEnnemy(void)
{
	listEnnemy = CreateList();
	listeWait = CreateList();
	if (!DEV_MODE)
	{
		GetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY);
	}
	else // charger les diférent type d'ennemy
	{
		ennemyEntity[0].type = 0;
		ennemyEntity[0].ennemydata.life = 20.f;
		ennemyEntity[0].ennemydata.energyMax = 500.f;
		ennemyEntity[0].ennemydata.energy = 500.f;
		ennemyEntity[0].ennemydata.energyRegen = 50.f;
		ennemyEntity[0].ennemydata.speedMax = 3.f;
		ennemyEntity[0].ennemydata.accelerationMax = 10.f;
		ennemyEntity[0].ennemydata.jumForce = 700.f;

		ennemyEntity[0].isJetpack = sfFalse;
		ennemyEntity[0].jetpack.consomation = 50.f;
		ennemyEntity[0].jetpack.life = 5.f;
		ennemyEntity[0].jetpack.trust = 10.f;


		ennemyEntity[1].type = 1;
		ennemyEntity[1].ennemydata.life = 10.f;
		ennemyEntity[1].ennemydata.energyMax = 800.f;
		ennemyEntity[1].ennemydata.energy = 800.f;
		ennemyEntity[1].ennemydata.energyRegen = 50.f;
		ennemyEntity[1].ennemydata.speedMax = 3.f;
		ennemyEntity[1].ennemydata.accelerationMax = 10.f;
		ennemyEntity[1].ennemydata.jumForce = 700.f;

		ennemyEntity[1].isJetpack = sfTrue;
		ennemyEntity[1].jetpack.consomation = 20.f;
		ennemyEntity[1].jetpack.life = 5.f;
		ennemyEntity[1].jetpack.trust = 20.f;
	}
	SetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de ennemyEntity
	mapData = GetMapData(); // connaitre la taille de la map
	printf("size x%d y%d\n", mapData->size.x, mapData->size.y);
	aStarMap = CreateGrid(mapData->size, sizeof(Case)); // création du tableau pour l'ia (A*) 
	texture = sfTexture_createFromImage(mapData->image, NULL);
	sprite = CreateSprite(texture, (sfVector2f) { 0 }, 1.f, 0.f);
	sfSprite_setColor(sprite, (sfColor) { 255, 255, 255, 50 });

}

void UpdateEnnemy(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.timer += _dt;
	ennemy->ennemyEntity.ennemydata.energyRegen += ennemy->ennemyEntity.ennemydata.energyRegen; //regen de l'energie passive
	if (ennemy->ennemyEntity.type >= TIMER_ASTAR)
	{
		ennemy->actiondemander = AStar(_index, GetPlayerPosition());
		ennemy->ennemyEntity.type -= TIMER_ASTAR;
	}
	printf("Action demander Droite%d Gauche%d Saut%d\n", ennemy->actiondemander.droite, ennemy->actiondemander.gauche, ennemy->actiondemander.Saut);
	CalculMoveEnnemy(_dt, _index); // calcul du mouvement
	sfSprite_move(ennemy->sprite, ennemy->ennemyEntity.move);
	// sécuriter pour le max d'énergie en stock
	if (ennemy->ennemyEntity.ennemydata.energy > ennemy->ennemyEntity.ennemydata.energyMax)
	{
		ennemy->ennemyEntity.ennemydata.energy = ennemy->ennemyEntity.ennemydata.energyMax;
	}

	sfVector2f collision = Colision(GetBounsEnnemy(_index));
	sfSprite_move(ennemy->sprite, collision);
	if (collision.x != 0)
	{
		ennemy->ennemyEntity.move.x = 0;
	}
	if (collision.y != 0)
	{
		ennemy->ennemyEntity.move.y = 0;
	}
	//printf("position x:%f y:%f\n", sfSprite_getPosition(ennemy->sprite).x, sfSprite_getPosition(ennemy->sprite).y);
	sfTexture_destroy(texture);
	texture = sfTexture_createFromImage(mapData->image, NULL);
	sfSprite_setTexture(sprite, texture, sfTrue);
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
		printf("creation d'un ennemy de type %d\n", _type);
	}
	//création et aplication des donné de l'ennemy
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
	sfBool test = 0;
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.acceleration = (sfVector2f){ 0,0 };
	if (sfKeyboard_isKeyPressed(sfKeyRight))
	{
		ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.ennemydata.accelerationMax;
		test = 1;
	}
	if (sfKeyboard_isKeyPressed(sfKeyLeft))
	{
		ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.ennemydata.accelerationMax;
		test = 1;
	}
	if (sfKeyboard_isKeyPressed(sfKeyUp))
	{
		sfFloatRect collisionEnnemy = GetBounsEnnemy(_index);
		collisionEnnemy.top += 1;
		sfVector2f collision = Colision(collisionEnnemy);
		//printf("collision %f", collision.y);
		if (collision.y)
		{
			ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.ennemydata.jumForce;
		}
		test = 1;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown))
	{
		ennemy->ennemyEntity.acceleration.y += ennemy->ennemyEntity.ennemydata.accelerationMax;
		test = 1;
	}
	if (test == 0)
	{
		sfFloatRect collisionEnnemy = GetBounsEnnemy(_index);
		collisionEnnemy.top += 1;
		sfVector2f collision = Colision(collisionEnnemy);
		//printf("collision %f", collision.y);
		if (collision.y)
		{
			if (ennemy->ennemyEntity.move.x > 0)
			{
				ennemy->ennemyEntity.move.x -= 1;
			}
			if (ennemy->ennemyEntity.move.x < 0)
			{
				ennemy->ennemyEntity.move.x += 1;
			}
			if (ennemy->ennemyEntity.move.x > -2 && ennemy->ennemyEntity.move.x < 2)
			{
				ennemy->ennemyEntity.move.x = 0;
			}

		}
	}
	ennemy->ennemyEntity.acceleration.y += G * 4;
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
}

ActionDemander AStar(int _index, sfVector2f _positionCible)
{
	sfVector2u positionCibleCase = RealPositionConvertTableauPosition(_positionCible);
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfVector2u positionDebutCase = RealPositionConvertTableauPosition(sfSprite_getPosition(ennemy->sprite));
	int x = 0;
	int y = 0;
	// reset du tableau
	for (unsigned y = 0; y < mapData->size.y; y++)
	{
		for (unsigned x = 0; x < mapData->size.x; x++)
		{
			aStarMap[y][x] = (Case){ 0 };
		}
	}

	//création du tableau chainé de sfVecteur2u
	sfVector2u* emplacement = Calloc(1, sizeof(sfVector2u));
	Element* element = CreateElement(emplacement);
	*emplacement = positionDebutCase;
	InsertElement(listeWait, element, 0);

	// aplication des donné dans le point de départ de l'agorytme A*
	aStarMap[positionDebutCase.y][positionDebutCase.x].action = 0.f;
	aStarMap[positionDebutCase.y][positionDebutCase.x].direction = NO_DIRECTION;
	aStarMap[positionDebutCase.y][positionDebutCase.x].rangeToDestination = NORM_POW2(positionCibleCase, positionDebutCase);
	aStarMap[positionDebutCase.y][positionDebutCase.x].energie = ennemy->ennemyEntity.ennemydata.energy;
	aStarMap[positionDebutCase.y][positionDebutCase.x].Résultat = CalculResultAStar(aStarMap[positionDebutCase.y][positionDebutCase.x]);

	// préparation des variable nécéssaire
	sfBool flag = sfFalse;
	sfVector2u caseRecherche = { 0 };
	int indexMin = 0;
	sfVector2u* caseRecup = NULL;
	sfVector2u caseGet = { 0 };

	//remplisage / exécution du code principale de l'algorytme A*
	while (GetListSize(listeWait) && flag == sfFalse)
	{
		indexMin = MinResultCase();
		caseRecup = GetElement(listeWait, indexMin)->value;
		caseGet = (sfVector2u){ caseRecup->x,caseRecup->y };
		//Droite
		caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = LEFT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}
		}
		//Bas Droite
		caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y + 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
		}
		else
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = UP_LEFT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}
		}
		//Haut Droite
		caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y - 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = DOWN_LEFT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}
		}
		//Gauche
		caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = RIGHT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}

		}
		//Bas Gauche
		caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y + 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
		}
		else
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = UP_RIGHT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}
		}
		//Haut Gauche
		caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y - 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0)
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = DOWN_RIGHT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}

		}
		//Haut
		caseRecherche = (sfVector2u){ caseGet.x , caseGet.y - 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{
			if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y).a == 0) // si espace libre
			{
				if (sfImage_getPixel(mapData->image, caseRecherche.x, caseRecherche.y - 1).a == 0) // si espace au dessu de cible libre
				{
					Case caseTemp = { 0 };
					caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
					caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
					caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
					caseTemp.Résultat = CalculResultAStar(caseTemp);
					caseTemp.direction = RIGHT;
					if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
					else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
					{
						aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
						AjoutListWait(caseRecherche);
					}
				}
			}
		}
		//Bas
		caseRecherche = (sfVector2u){ caseGet.x , caseGet.y + 1 };
		if (sfImage_getPixel(mapData->image, caseGet.x, caseGet.y + 1).a == 255) // si sur sol
		{

		}
		else
		{
			Case caseTemp = { 0 };
			caseTemp.rangeToDestination = NORM_POW2(caseRecherche, positionCibleCase);
			caseTemp.action = NORM_POW2(caseRecherche, caseGet) + aStarMap[caseGet.y][caseGet.x].action;
			caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
			caseTemp.Résultat = CalculResultAStar(caseTemp);
			caseTemp.direction = RIGHT;
			if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat == 0)
			{
				aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
				AjoutListWait(caseRecherche);
			}
			else if (aStarMap[caseRecherche.y][caseRecherche.x].Résultat > caseTemp.Résultat)
			{
				aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
				AjoutListWait(caseRecherche);
			}

		}
		RetirerListWait(indexMin);
		if (aStarMap[positionCibleCase.y][positionCibleCase.x].Résultat)
		{
			flag = sfTrue;
		}
	}

	flag = sfFalse;
	caseGet = (sfVector2u){ positionCibleCase.x, positionCibleCase.y };

	while (flag == sfFalse) // rechercher les action demander
	{
		switch (aStarMap[caseGet.y][caseGet.x].direction) // retrace la première action pour le chemin trouver
		{
		case NO_DIRECTION:
			break;
		case UP:
			caseRecherche = (sfVector2u){ caseGet.x, caseGet.y - 1 };
			break;
		case DOWN:
			caseRecherche = (sfVector2u){ caseGet.x, caseGet.y + 1 };
			break;
		case LEFT:
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y };
			break;
		case RIGHT:
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y };
			break;
		case UP_LEFT:
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y - 1 };
			break;
		case UP_RIGHT:
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y - 1 };
			break;
		case DOWN_LEFT:
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y + 1 };
			break;
		case DOWN_RIGHT:
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y + 1 };
			break;
		default:
			break;
		}
		if (caseRecherche.x == positionCibleCase.x && caseRecherche.y == positionCibleCase.y) // retourne le bloc d'action nésésaire
		{
			ActionDemander actionDemander = { 0 };
			switch (aStarMap[caseGet.y][caseGet.x].direction)
			{
			case NO_DIRECTION:
				break;
			case UP:

				break;
			case DOWN:
				actionDemander.Saut = 1;
				break;
			case LEFT:
				actionDemander.droite = 1;
				break;
			case RIGHT:
				actionDemander.gauche = 1;
				break;
			case UP_LEFT:
				actionDemander.droite = 1;
				break;
			case UP_RIGHT:
				actionDemander.gauche = 1;
				break;
			case DOWN_LEFT:
				actionDemander.droite = 1;
				actionDemander.Saut = 1;
				break;
			case DOWN_RIGHT:
				actionDemander.gauche = 1;
				actionDemander.Saut = 1;
				break;
			default:
				break;
			}
			return actionDemander;
		}
		caseGet = caseRecherche;
	}
}

float CalculResultAStar(Case _case)
{
	return (float) { _case.rangeToDestination + _case.action - (_case.energie * 1) };
}

int MinResultCase(void) // recherche du plus petit resultat dans la liste chainé listeWait
{
	int min = 0;
	if (GetListSize(listeWait) > 1)
	{
		for (unsigned i = 1; i < GetListSize(listeWait); i++)
		{
			sfVector2u* caseGet = GetElement(listeWait, i)->value;
			sfVector2u* caseMin = GetElement(listeWait, min)->value;
			if (aStarMap[caseGet->y][caseGet->x].Résultat < aStarMap[caseMin->y][caseMin->x].Résultat)
			{
				min = i;
			}
		}
	}
	return min;
}

void AjoutListWait(sfVector2u _caseAjout)
{
	sfVector2u* emplacementTemp = Calloc(1, sizeof(sfVector2u));
	Element* elementTemp = CreateElement(emplacementTemp);
	*emplacementTemp = _caseAjout;
	InsertElement(listeWait, elementTemp, 0);
}

void RetirerListWait(int _index)
{
	Element* elementTemp = GetElement(listeWait, _index)->value;
	free(elementTemp);
	RemoveElement(listeWait, _index);
}

sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal)
{
	//printf("ratio %f\n", mapData->caseSize.x);
	_positionReal.x = _positionReal.x / mapData->caseSize.x;
	_positionReal.y = _positionReal.y / mapData->caseSize.y;
	sfVector2u newposition = { (unsigned)_positionReal.x, (unsigned)_positionReal.y };
	return newposition;
}

sfVector2f GetPositionEnnemy(int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	return sfSprite_getPosition(ennemy->sprite);
}

sfFloatRect GetBounsEnnemy(int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	return sfSprite_getGlobalBounds(ennemy->sprite);
}

int GetNumberEnnemy()
{
	return GetListSize(listEnnemy);
}

sfColor GetColorsPixelMap(sfVector2f _position)
{
	sfVector2u positionMap = RealPositionConvertTableauPosition(_position);
	return sfImage_getPixel(mapData->image,positionMap.x, positionMap.y);
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

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), _position, 1, 1);
	ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
	ennemy->actiondemander = (ActionDemander){ 0 };

	InsertElement(listEnnemy, element, 0);
	sfFloatRect floatRect = GetBounsEnnemy(0);
	sfSprite_setOrigin(ennemy->sprite, (sfVector2f) { floatRect.width / 2, floatRect.height});
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
			//printf("Ennemie %d Toucher fait %f degat\n", _index, _degat);
		}
		ennemy->ennemyEntity.ennemydata.life -= _degat;
		if (ennemy->ennemyEntity.ennemydata.life < 0)
		{
			ennemy->ennemyEntity.ennemydata.life = 0;
		}
	}
	return isTouch;
}