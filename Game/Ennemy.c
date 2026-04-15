#include "Ennemy.h"
#include "Map.h"

#if DEV_PIERRE_ENEMY

void UpdateEnemyI(float _dt, int _index);
void CreateEnemyRandom(EnnemyEntity* _ennemy);
void CreateEnemy(EnnemyEntity* _ennemy, EnemyType _type);
void CalculMoveEnemy(float _dt, int _index);
ActionDemander AStar(int _index, sfVector2f _positionCible);
float CalculResultAStar(Case _case);
int MinResultCase(void);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);
sfBool TestColision(unsigned x, unsigned y);
void DebugTab(Case _case);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);
sfColor GetColorsPixelMap(sfVector2f _position);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);

List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
MapData* mapData;
Case** aStarMap;
List* listeWait;

sfSprite* sprite;
sfTexture* texture;

void LoadEnemy(void)
{
	listEnnemy = CreateList();
	listeWait = CreateList();
	if (!DEV_MODE)
	{
		GetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY);
	}
	else // charger les diférent type d'ennemy
	{
		ennemyEntity[3].type = 0;
		ennemyEntity[3].ennemydata.life = 20.f;
		ennemyEntity[3].ennemydata.energyMax = (float)MAX_ENRGIE;
		ennemyEntity[3].ennemydata.energy = (float)MAX_ENRGIE;
		ennemyEntity[3].ennemydata.energyRegen = 15.f;
		ennemyEntity[3].ennemydata.speedMax = 3.f;
		ennemyEntity[3].ennemydata.accelerationMax = 10.f;
		ennemyEntity[3].ennemydata.jumForce = 700.f;

		ennemyEntity[3].isJetpack = sfTrue;
		ennemyEntity[3].jetpack.consomation = 50.f;
		ennemyEntity[3].jetpack.life = 5.f;
		ennemyEntity[3].jetpack.trust = 10.f;


		ennemyEntity[6].type = 1;
		ennemyEntity[6].ennemydata.life = 10.f;
		ennemyEntity[6].ennemydata.energyMax = (float)MAX_ENRGIE;
		ennemyEntity[6].ennemydata.energy = (float)MAX_ENRGIE;
		ennemyEntity[6].ennemydata.energyRegen = 15.f;
		ennemyEntity[6].ennemydata.speedMax = 3.f;
		ennemyEntity[6].ennemydata.accelerationMax = 10.f;
		ennemyEntity[6].ennemydata.jumForce = 700.f;

		ennemyEntity[6].isJetpack = sfTrue;
		ennemyEntity[6].jetpack.consomation = 20.f;
		ennemyEntity[6].jetpack.life = 5.f;
		ennemyEntity[6].jetpack.trust = 20.f;
	}
	//SetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de ennemyEntity
	mapData = GetMapData(); // connaitre la taille de la map
	if (DEV_ENNEMY)
	{
		printf("size x%d y%d\n", mapData->size.x, mapData->size.y);
	}
	aStarMap = (Case**)CreateGrid(mapData->size, sizeof(Case)); // création du tableau pour l'ia (A*)
	texture = sfTexture_createFromImage(mapData->image, NULL);
	sprite = CreateSprite(texture, (sfVector2f) { 0 }, 1.f, 0.f);
	sfSprite_setTexture(sprite, texture, sfTrue);
	sfSprite_setColor(sprite, (sfColor) { 255, 255, 255, 50 });
}

void UpdateEnemy(float _dt)
{
	for (unsigned i = GetEnemyCount(); i > 0; i--)
	{
		UpdateEnemyI(_dt, i - 1);
	}
}

void UpdateEnemyI(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.timer += _dt;
	ennemy->ennemyEntity.ennemydata.energy += ennemy->ennemyEntity.ennemydata.energyRegen * _dt; //regen de l'energie passive
	if (ennemy->ennemyEntity.timer >= TIMER_ASTAR)
	{
		ennemy->actiondemander = AStar(_index, GetPlayerPosition());
		ennemy->ennemyEntity.timer -= TIMER_ASTAR;
	}
	//printf("Action demander Droite%d Gauche%d Saut%d\n", ennemy->actiondemander.droite, ennemy->actiondemander.gauche, ennemy->actiondemander.Saut);
	CalculMoveEnemy(_dt, _index); // calcul du mouvement
	sfSprite_move(ennemy->sprite, ennemy->ennemyEntity.move);
	// sécuriter pour le max d'énergie en stock
	if (ennemy->ennemyEntity.ennemydata.energy > ennemy->ennemyEntity.ennemydata.energyMax)
	{
		ennemy->ennemyEntity.ennemydata.energy = ennemy->ennemyEntity.ennemydata.energyMax;
	}

	sfVector2f collision = Colision(GetBounsEnemy(_index), AXIS_BOTH);
	collision.y += CollisionPassThrough(GetBounsEnemy(_index)).y;
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
	if (ennemy->ennemyEntity.ennemydata.life <= 0)
	{
		DestroyVisualEntity(ennemy->sprite);
		RemoveElement(listEnnemy, _index);
	}
}

void CreateEnemyRandom(EnnemyEntity* _ennemy)
{
	enum EnemyType temp = rand() % ALEATORY;
	switch (temp)
	{
	case CROWLER_SMALL:
		CreateEnemy(_ennemy, CROWLER_SMALL);
		break;
	case SOLDIER_SMALL:
		CreateEnemy(_ennemy, SOLDIER_SMALL);
		break;
	default:
		CreateEnemy(_ennemy, SOLDIER_SMALL);
		break;
	}
}

void CreateEnemy(EnnemyEntity* _ennemy, EnemyType _type)
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

void CalculMoveEnemy(float _dt, int _index)
{
	sfBool test = 0;
	sfBool test2 = 0;
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.acceleration = (sfVector2f){ 0,0 };
	if (sfKeyboard_isKeyPressed(sfKeyNumpad0) && ennemy->ennemyEntity.isJetpack && ennemy->ennemyEntity.jetpack.consomation * _dt < ennemy->ennemyEntity.ennemydata.energy)
	{
		ennemy->ennemyEntity.ennemydata.energy -= ennemy->ennemyEntity.jetpack.consomation * _dt;

		ennemy->ennemyEntity.acceleration.y -= G * 4;
		if (sfKeyboard_isKeyPressed(sfKeyM) || ennemy->actiondemander.droite)
		{
			ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.jetpack.trust;
			test = 1;
		}
		if (sfKeyboard_isKeyPressed(sfKeyK) || ennemy->actiondemander.gauche)
		{
			ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.jetpack.trust;
			test = 1;
		}
		if (sfKeyboard_isKeyPressed(sfKeyO) || ennemy->actiondemander.Saut)
		{
			ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.jetpack.trust;
			test2 = 1;
		}
		if (sfKeyboard_isKeyPressed(sfKeyL))
		{
			ennemy->ennemyEntity.acceleration.y += ennemy->ennemyEntity.jetpack.trust;
			test2 = 1;
		}
		if (ennemy->ennemyEntity.move.y > ennemy->ennemyEntity.jetpack.trust / 3)
		{
			ennemy->ennemyEntity.move.y = ennemy->ennemyEntity.ennemydata.speedMax;
		}
		else if (ennemy->ennemyEntity.move.y < -ennemy->ennemyEntity.jetpack.trust / 3)
		{
			ennemy->ennemyEntity.move.y = -ennemy->ennemyEntity.ennemydata.speedMax;
		}
		if (!test)
		{
			if (ennemy->ennemyEntity.move.x > 0)
			{
				ennemy->ennemyEntity.move.x -= ennemy->ennemyEntity.jetpack.trust * _dt;
			}
			else if (ennemy->ennemyEntity.move.x < 0)
			{
				ennemy->ennemyEntity.move.x += ennemy->ennemyEntity.jetpack.trust * _dt;;
			}

			if (ennemy->ennemyEntity.move.x > (float)-0.5 && ennemy->ennemyEntity.move.x < (float)0.5)
			{
				ennemy->ennemyEntity.move.x = 0;
			}
		}
		if (!test2)
		{
			if (ennemy->ennemyEntity.move.y > 0)
			{
				ennemy->ennemyEntity.move.y -= ennemy->ennemyEntity.jetpack.trust * _dt;
			}
			else if (ennemy->ennemyEntity.move.y < 0)
			{
				ennemy->ennemyEntity.move.y += ennemy->ennemyEntity.jetpack.trust * _dt;;
			}

			if (ennemy->ennemyEntity.move.y > (float)-0.5 && ennemy->ennemyEntity.move.y < (float)0.5)
			{
				ennemy->ennemyEntity.move.y = 0;
			}
		}
	}
	else
	{
		if (sfKeyboard_isKeyPressed(sfKeyM) || ennemy->actiondemander.droite)
		{
			ennemy->ennemyEntity.acceleration.x += ennemy->ennemyEntity.ennemydata.accelerationMax;
			test = 1;
		}
		if (sfKeyboard_isKeyPressed(sfKeyK) || ennemy->actiondemander.gauche)
		{
			ennemy->ennemyEntity.acceleration.x += -ennemy->ennemyEntity.ennemydata.accelerationMax;
			test = 1;
		}
		if (sfKeyboard_isKeyPressed(sfKeyO) || ennemy->actiondemander.Saut)
		{
			sfFloatRect collisionEnnemy = GetBounsEnemy(_index);
			collisionEnnemy.top += 1;
			sfVector2f collision = Colision(collisionEnnemy, AXIS_BOTH);
			//printf("collision %f", collision.y);
			if (collision.y)
			{
				ennemy->ennemyEntity.acceleration.y += -ennemy->ennemyEntity.ennemydata.jumForce;
			}
			test = 1;
		}
		if (test == 0)
		{
			sfFloatRect collisionEnnemy = GetBounsEnemy(_index);
			collisionEnnemy.top += 1;
			sfVector2f collision = Colision(collisionEnnemy, AXIS_BOTH);
			//printf("collision %f", collision.y);
			if (collision.y)
			{
				if (ennemy->ennemyEntity.move.x > 0)
				{
					ennemy->ennemyEntity.move.x -= 1;
				}
				else if (ennemy->ennemyEntity.move.x < 0)
				{
					ennemy->ennemyEntity.move.x += 1;
				}

				if (ennemy->ennemyEntity.move.x > (float)-1.5 && ennemy->ennemyEntity.move.x < (float)1.5)
				{
					ennemy->ennemyEntity.move.x = 0;
				}

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
	if (positionCibleCase.x <= 0 || positionCibleCase.y <= 0 || positionCibleCase.x >= mapData->size.x || positionCibleCase.y >= mapData->size.y)
	{
		TpPlayerToSpawn();
		return (ActionDemander) { 0 };
	}
	positionCibleCase.y -= 1;
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfVector2u positionDebutCase = RealPositionConvertTableauPosition(sfSprite_getPosition(ennemy->sprite));
	positionDebutCase.y -= 1;
	//printf("position cible x:%d y:%d position debut x:%d y:%d\n", positionCibleCase.x, positionCibleCase.y, positionDebutCase.x, positionDebutCase.y);
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
	aStarMap[positionDebutCase.y][positionDebutCase.x].rangeToDestination = sqrtf((float)NORM_POW2(positionCibleCase, positionDebutCase));
	aStarMap[positionDebutCase.y][positionDebutCase.x].energie = ennemy->ennemyEntity.ennemydata.energy + ennemy->ennemyEntity.ennemydata.energyRegen;
	if (aStarMap[positionDebutCase.y][positionDebutCase.x].energie > MAX_ENRGIE)
	{
		aStarMap[positionDebutCase.y][positionDebutCase.x].energie = MAX_ENRGIE;
	}
	aStarMap[positionDebutCase.y][positionDebutCase.x].resultat = CalculResultAStar(aStarMap[positionDebutCase.y][positionDebutCase.x]);

	/*if (ennemy->actiondemander.jetPack)
	{
		aStarMap[positionDebutCase.y][positionDebutCase.x].jumpForce = -1;
	}
	else if (ennemy->ennemyEntity.move.y < 0)
	{
		aStarMap[positionDebutCase.y][positionDebutCase.x].jumpForce = -1;
	}
	else if (ennemy->ennemyEntity.move.y == 0)
	{
		aStarMap[positionDebutCase.y][positionDebutCase.x].jumpForce = -1;
	}
	else
	{
		aStarMap[positionDebutCase.y][positionDebutCase.x].jumpForce = 1;
	}*/

	// préparation des variable nécéssaire
	sfBool flag = sfTrue;
	sfVector2u caseRecherche = { 0 };
	int indexMin = 0;
	sfVector2u* caseRecup = NULL;
	sfVector2u caseGet = { 0 };
	if (positionDebutCase.x != positionCibleCase.x || positionDebutCase.y != positionCibleCase.y)
	{
		//remplisage / exécution du code principale de l'algorytme A*
		while (GetListSize(listeWait) && flag == sfTrue)
		{
			indexMin = MinResultCase();
			caseRecup = GetElement(listeWait, indexMin)->value;
			caseGet = (sfVector2u){ caseRecup->x,caseRecup->y };
			//Droite
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{

						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = LEFT;
						caseTemp.jumpForce = 0;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}
			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce == 0) // sinon si on est au sommet du saut ()
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{

						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = LEFT;
						caseTemp.jumpForce = -1;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}


			}

			//Bas Droite
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y + 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{
			}
			else
			{
				if (aStarMap[caseGet.y][caseGet.x].jumpForce <= 0)
				{


					if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.direction = UP_LEFT;
							caseTemp.jumpForce = -1;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}
			}
			//Haut Droite
			caseRecherche = (sfVector2u){ caseGet.x + 1, caseGet.y - 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{

				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseGet.x + 1, caseGet.y) && !TestColision(caseGet.x, caseGet.y - 2)) // si chemin libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.direction = DOWN_LEFT;
							caseTemp.jumpForce = JUMP_FORCE;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}
			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce > 0) // sinon si on est en plein saut ()
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseGet.x + 1, caseGet.y) && !TestColision(caseGet.x, caseGet.y - 2)) // si chemin libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.direction = DOWN_LEFT;
							caseTemp.jumpForce = aStarMap[caseGet.y][caseGet.x].jumpForce - 1;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}
			}

			//Gauche
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{
						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = RIGHT;
						caseTemp.jumpForce = 0;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}

			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce == 0)
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{
						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = RIGHT;
						caseTemp.jumpForce = -1;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}
			}

			//Bas Gauche
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y + 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{

			}
			else
			{
				if (aStarMap[caseGet.y][caseGet.x].jumpForce <= 0)
				{


					if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.jumpForce = -1;
							caseTemp.direction = UP_RIGHT;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}
			}
			//Haut Gauche
			caseRecherche = (sfVector2u){ caseGet.x - 1, caseGet.y - 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseGet.x - 1, caseGet.y) && !TestColision(caseGet.x, caseGet.y - 2)) // si chemin libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.direction = DOWN_RIGHT;
							caseTemp.jumpForce = JUMP_FORCE;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}

			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce > 0) // sinon si on est en plein saut ()
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseGet.x - 1, caseGet.y) && !TestColision(caseGet.x, caseGet.y - 2)) // si chemin libre
					{
						if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
						{
							Case caseTemp = { 0 };
							caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
							caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
							caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
							if (caseTemp.energie > MAX_ENRGIE)
							{
								caseTemp.energie = MAX_ENRGIE;
							}
							caseTemp.resultat = CalculResultAStar(caseTemp);
							caseTemp.direction = DOWN_RIGHT;
							caseTemp.jumpForce = aStarMap[caseGet.y][caseGet.x].jumpForce - 1;
							if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
							else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
							{
								aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
								AjoutListWait(caseRecherche);
							}
						}
					}
				}
			}

			//Haut
			caseRecherche = (sfVector2u){ caseGet.x , caseGet.y - 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{
						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = DOWN;
						caseTemp.jumpForce = JUMP_FORCE;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}
			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce > 0) // sinon si on est en plein saut ()
			{
				if (!TestColision(caseRecherche.x, caseRecherche.y)) // si espace libre
				{
					if (!TestColision(caseRecherche.x, caseRecherche.y - 1)) // si espace au dessu de cible libre
					{
						Case caseTemp = { 0 };
						caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
						caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
						caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
						if (caseTemp.energie > MAX_ENRGIE)
						{
							caseTemp.energie = MAX_ENRGIE;
						}
						caseTemp.resultat = CalculResultAStar(caseTemp);
						caseTemp.direction = DOWN;
						caseTemp.jumpForce = aStarMap[caseGet.y][caseGet.x].jumpForce - 1;
						if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
						else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
						{
							aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
							AjoutListWait(caseRecherche);
						}
					}
				}
			}

			//Bas
			caseRecherche = (sfVector2u){ caseGet.x , caseGet.y + 1 };
			if (TestColision(caseGet.x, caseGet.y + 1)) // si sur sol
			{

			}
			else if (aStarMap[caseGet.y][caseGet.x].jumpForce <= 0)
			{
				Case caseTemp = { 0 };
				caseTemp.rangeToDestination = sqrtf((float)NORM_POW2(caseRecherche, positionCibleCase));
				caseTemp.action = sqrtf((float)NORM_POW2(caseRecherche, caseGet)) + aStarMap[caseGet.y][caseGet.x].action;
				caseTemp.energie = aStarMap[caseGet.y][caseGet.x].energie + ennemy->ennemyEntity.ennemydata.energyRegen;
				if (caseTemp.energie > MAX_ENRGIE)
				{
					caseTemp.energie = MAX_ENRGIE;
				}
				caseTemp.resultat = CalculResultAStar(caseTemp);
				caseTemp.direction = UP;
				caseTemp.jumpForce = -1;
				if (aStarMap[caseRecherche.y][caseRecherche.x].resultat == 0)
				{
					aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
					AjoutListWait(caseRecherche);
				}
				else if (aStarMap[caseRecherche.y][caseRecherche.x].resultat > caseTemp.resultat)
				{
					aStarMap[caseRecherche.y][caseRecherche.x] = caseTemp;
					AjoutListWait(caseRecherche);
				}

			}
			RetirerListWait(indexMin);
			if (aStarMap[positionCibleCase.y][positionCibleCase.x].resultat)
			{
				flag = sfFalse;
			}

		}

		for (int i = GetListSize(listeWait) - 1; i >= 0; i--)
		{
			RetirerListWait(i);
		}

		if (aStarMap[positionCibleCase.y][positionCibleCase.x].resultat)
		{
			//printf("Cible Ateinte");
			flag = sfTrue;
		}
		else
		{
			//printf("Cible Non Ateinte");
			flag = sfFalse;
		}
		caseGet = (sfVector2u){ positionCibleCase.x, positionCibleCase.y };
		if (DEBUG_MODE_A_STAR)
		{
			system("cls");
		}
		sfVector2u vectortemp = { 0 };
		while (flag) // rechercher les action demander
		{
			switch (aStarMap[caseGet.y][caseGet.x].direction) // retrace la première action pour le chemin trouver
			{
			case NO_DIRECTION:
				return(ActionDemander) { 0 };
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
			if (caseRecherche.x == positionDebutCase.x && caseRecherche.y == positionDebutCase.y) // retourne le bloc d'action nésésaire
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
				if (aStarMap[caseGet.y][caseGet.x].jetPackActive)
				{
					actionDemander.jetPack = 1;
				}
				//printf("droite: %d gauche:%d saut:%d jetpack:%d\n", actionDemander.droite, actionDemander.gauche, actionDemander.Saut, actionDemander.jetPack);
				return actionDemander;
			}
			if (vectortemp.x == caseRecherche.x && vectortemp.y == caseRecherche.y)
			{
				return (ActionDemander) { 0 };
			}
			vectortemp = caseGet;
			caseGet = caseRecherche;
		}
	}
	return (ActionDemander) { 0 };
}

float CalculResultAStar(Case _case)
{
	return (float) { _case.rangeToDestination + _case.action + MAX_ENRGIE - _case.energie };
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
			if (aStarMap[caseGet->y][caseGet->x].resultat < aStarMap[caseMin->y][caseMin->x].resultat)
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
	InsertElement(listeWait, elementTemp, GetListSize(listeWait));
}

void RetirerListWait(int _index)
{
	Element* elementTemp = GetElement(listeWait, _index)->value;
	Free(elementTemp);
	RemoveElement(listeWait, _index);
}

sfBool TestColision(unsigned x, unsigned y)
{
	if (sfImage_getPixel(mapData->image, x, y).a == 255)
	{
		sfColor color = sfImage_getPixel(mapData->image, x, y);
		if (color.r == 0 && color.g == 255 && color.b == 0
			|| color.r == 255 && color.g == 0 && color.b == 255)
		{
			return sfFalse;
		}
		else
		{
			return sfTrue;
		}
	}
	else
	{
		return sfFalse;
	}
}

void DebugTab(Case _case)
{
	if (DEBUG_MODE_A_STAR)
	{

	}
}

sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal)
{
	//printf("ratio %f\n", mapData->caseSize.x);
	_positionReal.x = _positionReal.x / mapData->caseSize.x;
	_positionReal.y = _positionReal.y / mapData->caseSize.y;
	sfVector2u newposition = { (unsigned)_positionReal.x, (unsigned)_positionReal.y };
	return newposition;
}

sfVector2f GetPositionEnemy(int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	return sfSprite_getPosition(ennemy->sprite);
}

sfFloatRect GetBounsEnemy(int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	return sfSprite_getGlobalBounds(ennemy->sprite);
}

int GetEnemyCount()
{
	return GetListSize(listEnnemy);
}

sfColor GetColorsPixelMap(sfVector2f _position)
{
	sfVector2u positionMap = RealPositionConvertTableauPosition(_position);
	return sfImage_getPixel(mapData->image, positionMap.x, positionMap.y);
}

void SetPositionEnemy(sfVector2f _position, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfSprite_setPosition(ennemy->sprite, _position);
}

void ResetEnemy(void)
{
	for (unsigned i = 0; i < GetListSize(listEnnemy); i++)
	{
		Ennemy* ennemy = GetElement(listEnnemy, i)->value;
		DestroyVisualEntity(ennemy->sprite);
	}
	RemoveList(listEnnemy);
	if (mapData)
	{
		for (unsigned i = 0; i < mapData->size.y; i++)
		{
			Free(aStarMap[i]);
		}
		Free(aStarMap);
	}

	RemoveList(listeWait);
	sfTexture_destroy(texture);
	DestroyVisualEntity(sprite);

}

int GetNearestEnemy(List* _listeIgnore, sfVector2f _position)
{
	int index = -1;
	sfBool test = 1;
	for (unsigned i = 0; i < GetListSize(listEnnemy); i++)
	{
		if (index >= 0)
		{
			if (NORM_POW2(GetPositionEnemy(i), _position) < NORM_POW2(GetPositionEnemy(index), _position))
			{
				for (unsigned r = 0; r < GetListSize(_listeIgnore); r++)
				{
					int* temp = GetElement(listEnnemy, r)->value;
					if (index == *temp)
					{
						test = 0;
					}
				}
				if (test)
				{
					index = i;
				}
			}
		}
		else
		{
			index = 0;
		}
	}
	return index;
}

void AddEnemy(sfVector2f _position, enum EnemyType _type)
{
	Ennemy* ennemy = Calloc(1, sizeof(Ennemy));

	Element* element = CreateElement(ennemy);
	element->value = ennemy;

	switch (_type)
	{
	case CROWLER_SMALL:
		CreateEnemy(&ennemy->ennemyEntity, CROWLER_SMALL);
		break;
	case SOLDIER_SMALL:
		CreateEnemy(&ennemy->ennemyEntity, SOLDIER_SMALL);
		break;
	case ALEATORY:
		CreateEnemyRandom(&ennemy->ennemyEntity);
		break;
	default:
		break;
	}

	ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), _position, 1, 1);
	ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
	ennemy->actiondemander = (ActionDemander){ 0 };
	sfFloatRect floatRect = sfSprite_getGlobalBounds(ennemy->sprite);
	sfSprite_setOrigin(ennemy->sprite, (sfVector2f) { floatRect.width / 2, floatRect.height });

	InsertElement(listEnnemy, element, 0);
	SetPositionEnemy(_position, 0);
}

sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat, sfFloatRect* _hitbox)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	sfColor pixelColor = sfImage_getPixel(ennemy->imageColideur, (int)_touch.x, (int)_touch.y);
	sfBool isTouch = sfFalse;
	printf("Color a= %d ", pixelColor.a);
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
			printf("Enemy toucher avec degat");
		}
		printf("Vie %f\n", ennemy->ennemyEntity.ennemydata.life);
	}
	return isTouch;
}

sfBool HitEnemy(float _degat, sfFloatRect _hitbox)
{
	sfFloatRect hitboxEnemy = { 0 };
	for (int i = 0; i < GetEnemyCount(); i++)
	{
		hitboxEnemy = GetBounsEnemy(i);
		if (sfFloatRect_intersects(&_hitbox, &hitboxEnemy, NULL))
		{
			HitEnemyI(i, (sfVector2f) { 1, 1 }, _degat, & _hitbox);
			return sfTrue;
		}
	}
	return sfFalse;
}

#else
Enemy enemy;

void LoadEnemy(void)
{
	enemy = (Enemy){ 0 };
	enemy.entity = Calloc(1, sizeof(EnemyEntity));

	enemy.data[SOLDIER_SMALL] = (EnemyData){ GetAsset("Assets/Sprites/spider_small.png") , 100.f, 5.f, 200.f, 50.f, 1.f };
	enemy.data[SOLDIER_MEDIUM] = (EnemyData){ GetAsset("Assets/Sprites/spider_medium.png") , 100.f, 5.f, 400.f, 100.f , 3.f };
	enemy.data[SOLDIER_HEAVY] = (EnemyData){ GetAsset("Assets/Sprites/spider_large.png") , 100.f, 5.f, 1000.f, 300.f , 10.f };
}

void UpdateEnemy(float _dt)
{
	sfFloatRect enemyHitbox = { 0 };
	sfVector2f playerPosition = GetPlayerPosition();
	for (unsigned i = 0; i < enemy.count; i++)
	{
		sfVector2f pos = sfSprite_getPosition(enemy.entity[i].sprite);
		printf("%f , %f\n", pos.x, pos.y);
		if (sfFloatRect_contains(&enemy.entity[i].region, playerPosition.x, playerPosition.y - 1))
		{
			enemyHitbox = sfSprite_getGlobalBounds(enemy.entity[i].sprite);

			if (enemyHitbox.left + enemyHitbox.width < playerPosition.x)
			{
				enemy.entity[i].velocity.x += enemy.data[enemy.entity[i].type].speed * _dt;
			}
			else if (enemyHitbox.left > playerPosition.x)
			{
				enemy.entity[i].velocity.x -= enemy.data[enemy.entity[i].type].speed * _dt;
			}
			else
			{
				if (playerPosition.y > enemyHitbox.top && playerPosition.y < enemyHitbox.top + enemyHitbox.height)
				{
					KillPlayer();
				}
				enemy.entity[i].velocity.x = 0;
			}

			if (!enemy.entity[i].velocity.y)
			{
				if (playerPosition.y < enemyHitbox.top && enemy.entity[i].velocity.x)
				{
					if (1)
					{
						if (enemy.entity[i].velocity.x < 0)
						{
							enemy.entity[i].velocity.y = -enemy.data[enemy.entity[i].type].jumpForce;
						}
						else
						{
							enemy.entity[i].velocity.y = -enemy.data[enemy.entity[i].type].jumpForce;
						}
					}
					else
					{
						if (enemy.entity[i].velocity.x < 0)
						{
							sfVector2f colision = Colision((sfFloatRect) { enemyHitbox.left - enemyHitbox.width, enemyHitbox.top, enemyHitbox.width, enemyHitbox.height }, AXIS_BOTH);
							if (colision.x)
							{
								enemy.entity[i].velocity.y = -enemy.data[enemy.entity[i].type].jumpForce;
							}
						}
						else if (enemy.entity[i].velocity.x > 0)
						{
							sfVector2f colision = Colision((sfFloatRect) { enemyHitbox.left + enemyHitbox.width, enemyHitbox.top, enemyHitbox.width, enemyHitbox.height }, AXIS_BOTH);
							if (colision.x)
							{
								enemy.entity[i].velocity.y = -enemy.data[enemy.entity[i].type].jumpForce;
							}
						}
					}
				}
			}
		}
		else
		{
			enemy.entity[i].velocity.x = 0;
		}

		enemy.entity[i].velocity.y += G * enemy.data[enemy.entity[i].type].weight * _dt;
		if (enemy.entity[i].velocity.y > MAX_FALL_SPEED_ENEMY)
		{
			enemy.entity[i].velocity.y = MAX_FALL_SPEED_ENEMY;
		}
		else if (enemy.entity[i].velocity.y < -MAX_FALL_SPEED_ENEMY)
		{
			enemy.entity[i].velocity.y = -MAX_FALL_SPEED_ENEMY;
		}

		sfSprite_move(enemy.entity[i].sprite, (sfVector2f) { enemy.entity[i].velocity.x* _dt, enemy.entity[i].velocity.y* _dt });

		sfVector2f colision = Colision(sfSprite_getGlobalBounds(enemy.entity[i].sprite), AXIS_BOTH);
		colision.y += CollisionPassThrough(sfSprite_getGlobalBounds(enemy.entity[i].sprite)).y;
		if (colision.y > 0)
		{
			enemy.entity[i].velocity.y = -G * enemy.data[enemy.entity[i].type].weight * _dt;
		}
		else if (colision.y < 0)
		{
			enemy.entity[i].velocity.y = 0;
		}
		else if (colision.x)
		{
			colision.y -= abs(colision.x);
			enemy.entity[i].velocity.x = 0;
		}

		sfSprite_move(enemy.entity[i].sprite, colision);

	}
}

sfBool HitEnemy(float _degat, sfFloatRect _hitbox)
{
	sfFloatRect hitboxEnemy = { 0 };
	for (unsigned i = 0; i < enemy.count; i++)
	{
		hitboxEnemy = sfSprite_getGlobalBounds(enemy.entity[i].sprite);
		if (sfFloatRect_intersects(&_hitbox, &hitboxEnemy, NULL))
		{
			enemy.entity[i].life -= _degat;
			if (enemy.entity[i].life <= 0)
			{
				enemy.count--;
				DestroyVisualEntity(enemy.entity[i].sprite);
				enemy.entity[i].sprite = enemy.entity[enemy.count].sprite;
				enemy.entity = Realloc(enemy.entity, (size_t)(enemy.count) * sizeof(EnemyEntity));
			}
			return sfTrue;
		}
	}
	return sfFalse;
}

void ResetEnemy(void)
{
	for (unsigned i = 0; i < enemy.count; i++)
	{
		DestroyVisualEntity(enemy.entity[i].sprite);
	}
	enemy.count = 0;
}

void AddEnemy(sfVector2f _position, EnemyType _type, sfFloatRect _region)
{
	if (_type == ALEATORY)
	{
		_type = rand() % (SOLDIER_HEAVY + 1);
	}
	enemy.entity = Realloc(enemy.entity, (size_t)(enemy.count + 1) * sizeof(EnemyEntity));
	enemy.entity[enemy.count].sprite = CreateSprite(enemy.data[_type].texture, _position, 1.f, 1.f);
	sfVector2f pos = sfSprite_getPosition(enemy.entity[enemy.count].sprite);
	enemy.entity[enemy.count].type = _type;
	enemy.entity[enemy.count].region = _region;
	enemy.entity[enemy.count].life = enemy.data[_type].lifeMax;
	enemy.count++;
}

#endif //  DEV_PIERRE_ENEMY == 1
