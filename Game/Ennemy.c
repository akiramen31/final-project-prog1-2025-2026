#include "Ennemy.h"
#include "Map.h"

#if DEV_PIERRE_ENEMY

void UpdateEnemyI(float _dt, int _index);
void CreateEnemyRandom(EnnemyEntity* _ennemy);
void CreateEnemy(EnnemyEntity* _ennemy, EnemyType _type);
void CalculMoveEnemy(float _dt, int _index);
ActionDemander AStar(int _index, sfVector2f _positionCible);
ActionDemander AStar2(int _index, sfFloatRect _cible);
float CalculResultAStar(Case _case);
int MinResultCase(void);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);
sfBool TestColision(unsigned x, unsigned y);
void DebugTab(Case _case);
sfColor GetColorsPixelMap(sfVector2f _position);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);

List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
MapData* mapData;
Case** aStarMap;
List* listeWait;

sfSprite* sprite;
sfTexture* texture;

Tableau tableau[ALEATORY];

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
		ennemyEntity[3].ennemydata.life = 3.f;
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
		ennemyEntity[6].ennemydata.life = 2.f;
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
	aStarMap = (Case**)CreateGrid(mapData->size.x, mapData->size.y, sizeof(Case)); // création du tableau pour l'ia (A*)
	texture = sfTexture_createFromImage(mapData->image, NULL);
	sprite = CreateSprite(texture, (sfVector2f) { 0 }, 1.f, 0.f);
	sfSprite_setTexture(sprite, texture, sfTrue);
	sfSprite_setColor(sprite, (sfColor) { 255, 255, 255, 50 });
}

void UpdateEnemy(float _dt)
{
	sfVector2f playerPos = GetPlayerPosition();
	for (int i = GetEnemyCount() - 1; i >= 0; i--)
	{
		Ennemy* enemy = GetElement(listEnnemy, i)->value;
		if (enemy->ennemyEntity.region.left <= playerPos.x && enemy->ennemyEntity.region.left + enemy->ennemyEntity.region.width >= playerPos.x && enemy->ennemyEntity.region.top <= playerPos.y && enemy->ennemyEntity.region.top + enemy->ennemyEntity.region.height >= playerPos.y)
		{
			UpdateEnemyI(_dt, i);
		}
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
		int itterateur = 0;
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
				itterateur++;
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
			if (caseRecherche.x == positionDebutCase.x && caseRecherche.y == positionDebutCase.y || itterateur == 50) // retourne le bloc d'action nésésaire
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

ActionDemander AStar2(int _index, sfFloatRect _cible)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	char** grid = CreateGrid((int)ennemy->ennemyEntity.region.width / TILE_SIZE,(int) ennemy->ennemyEntity.region.height / TILE_SIZE, sizeof(char));

	for (int y = 0; y < (int) { ennemy->ennemyEntity.region.height / TILE_SIZE }; y++)
	{
		for (int x = 0; x < (int) { ennemy->ennemyEntity.region.width / TILE_SIZE }; x++)
		{
			sfVector2f reaction = Colision((sfFloatRect) { ennemy->ennemyEntity.region.left + x * TILE_SIZE, ennemy->ennemyEntity.region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
			if (reaction.x || reaction.y)
			{
				grid[y][x] = 2;
			}
			else if (CollisionPassThrough((sfFloatRect) { ennemy->ennemyEntity.region.left + x * TILE_SIZE, ennemy->ennemyEntity.region.top + y * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
			{
				grid[y][x] = 1;
			}
		}
	}

	if (ennemy->ennemyEntity.region.top != tableau[ennemy->ennemyEntity.type].region.top
		|| ennemy->ennemyEntity.region.left != tableau[ennemy->ennemyEntity.type].region.left)
	{
		// liberer lancienne GRID
		tableau[ennemy->ennemyEntity.type].region = ennemy->ennemyEntity.region;
		tableau[ennemy->ennemyEntity.type].grid = CreateGrid((int)ennemy->ennemyEntity.region.width / TILE_SIZE, (int)ennemy->ennemyEntity.region.width / TILE_SIZE,sizeof(Case2));
	};

	
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

void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region)
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
	ennemy->ennemyEntity.region = _region;
	sfFloatRect floatRect = sfSprite_getGlobalBounds(ennemy->sprite);
	sfSprite_setOrigin(ennemy->sprite, (sfVector2f) { floatRect.width / 2, floatRect.height });

	InsertElement(listEnnemy, element, 0);
	SetPositionEnemy(_position, 0);
}

sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat)
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
	sfFloatRect hitboxTir = { 0 };
	sfVector2f touch = { 0 };
	for (int i = 0; i < GetEnemyCount(); i++)
	{
		hitboxEnemy = GetBounsEnemy(i);
		if (sfFloatRect_intersects(&_hitbox, &hitboxEnemy, &hitboxTir))
		{
			touch = (sfVector2f){ hitboxTir.left + hitboxTir.width / 2, hitboxTir.top + hitboxTir.height / 2 };
			touch.x -= hitboxEnemy.left;
			touch.y -= hitboxEnemy.top;
			return HitEnemyI(i, touch, _degat);
		}
	}
	return sfFalse;
}

#else

sfVector2i GetMoveEnemyAI(unsigned _i, sfVector2f _playerPos);
sfVector2i GetMoveEnemyAITemp(unsigned _i, sfVector2f _playerPos);
sfVector2i AStar(char** _grid, sfVector2i _gridSize, sfIntRect _startRect, sfIntRect _goalRect, int _jumpHeightTiles);

Enemy enemy;

void LoadEnemy(void)
{
	enemy = (Enemy){ 0 };
	enemy.entity = Calloc(1, sizeof(EnemyEntity));

	enemy.data[SOLDIER_SMALL] = (EnemyData){ GetAsset("Assets/Sprites/spider_small.png") , 100.f, 5.f, 450.f, 100.f, 1.f };
	enemy.data[SOLDIER_MEDIUM] = (EnemyData){ GetAsset("Assets/Sprites/spider_medium.png") , 100.f, 5.f, 450.f, 100.f , 3.f };
	enemy.data[SOLDIER_HEAVY] = (EnemyData){ GetAsset("Assets/Sprites/spider_large.png") , 100.f, 5.f, 450.f, 100.f , 10.f };
}

void UpdateEnemy(float _dt)
{
	sfVector2f playerPosition = GetPlayerPosition();
	sfVector2i enemyMove = { 0 };
	for (unsigned i = 0; i < enemy.count; i++)
	{
		sfVector2f p = sfSprite_getPosition(enemy.entity[i].sprite);
		enemyMove = GetMoveEnemyAI(i, playerPosition);

		if (enemyMove.x)
		{
			enemy.entity[i].velocity.x += enemyMove.x * enemy.data[enemy.entity[i].type].speed * _dt;
		}
		else
		{
			enemy.entity[i].velocity.x = 0;
		}
		if (enemyMove.y < 0)
		{
			enemy.entity[i].velocity.y = -enemy.data[enemy.entity[i].type].jumpForce;
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
	sfVector2f playerPos = GetPlayerPosition();
	sfVector2i enemyAction = { 0 };
	for (unsigned i = 0; i < enemy.count; i++)
	{
		hitboxEnemy = sfSprite_getGlobalBounds(enemy.entity[i].sprite);
		if (sfFloatRect_intersects(&_hitbox, &hitboxEnemy, NULL))
		{
			enemyAction = GetMoveEnemyAITemp(i, playerPos);

			enemy.entity[i].life -= _degat;
			if (enemy.entity[i].life <= 0)
			{
				enemy.count--;
				DestroyVisualEntity(enemy.entity[i].sprite);
				enemy.entity[i].sprite = enemy.entity[enemy.count].sprite;
				if (enemy.count)
				{
					enemy.entity = Realloc(enemy.entity, (size_t)(enemy.count) * sizeof(EnemyEntity));
				}

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

sfVector2i GetMoveEnemyAI(unsigned _i, sfVector2f _playerPos)
{
	sfVector2i move = { 0 };
	if (enemy.entity[_i].region.left <= _playerPos.x && enemy.entity[_i].region.left + enemy.entity[_i].region.width >= _playerPos.x && enemy.entity[_i].region.top <= _playerPos.y && enemy.entity[_i].region.top + enemy.entity[_i].region.height >= _playerPos.y)
	{
		sfIntRect playerHitbox = { (int)(_playerPos.x - enemy.entity[_i].region.left) / TILE_SIZE - 1,(int)(_playerPos.y - enemy.entity[_i].region.top) / TILE_SIZE, 1, 2 };

		sfVector2i gridSize = { (int)enemy.entity[_i].region.width / TILE_SIZE, (int)enemy.entity[_i].region.height / TILE_SIZE };
		char** grid = CreateGrid(gridSize.x, gridSize.y, sizeof(char));

		for (int y = 0; y < gridSize.y; y++)
		{
			for (int x = 0; x < gridSize.x; x++)
			{
				sfVector2f reaction = Colision((sfFloatRect) { enemy.entity[_i].region.left + x * TILE_SIZE, enemy.entity[_i].region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
				if (reaction.x || reaction.y)
				{
					grid[y][x] = 2;
				}
				else if (CollisionPassThrough((sfFloatRect) { enemy.entity[_i].region.left + x * TILE_SIZE, enemy.entity[_i].region.top + y * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
				{
					grid[y][x] = 1;
				}
			}
		}

		sfFloatRect enemybox = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
		sfIntRect enemyHitbox = { (int)(enemybox.left - enemy.entity[_i].region.left) / TILE_SIZE,(int)(enemybox.top - enemy.entity[_i].region.top) / TILE_SIZE,(int)enemybox.width / TILE_SIZE + 1 ,(int)enemybox.height / TILE_SIZE + 1 };

		move = AStar(grid, gridSize, enemyHitbox, playerHitbox, 6);

		FreeGrid(grid);
	}
	return move;
}

sfVector2i GetMoveEnemyAITemp(unsigned _i, sfVector2f _playerPos)
{
	sfVector2i move = { 0 };
	if (enemy.entity[_i].region.left <= _playerPos.x && enemy.entity[_i].region.left + enemy.entity[_i].region.width >= _playerPos.x && enemy.entity[_i].region.top <= _playerPos.y && enemy.entity[_i].region.top + enemy.entity[_i].region.height >= _playerPos.y)
	{
		sfFloatRect enemyHitbox = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
		if (enemyHitbox.left + enemyHitbox.width < _playerPos.x)
		{
			move.x = 1;
		}
		else if (enemyHitbox.left > _playerPos.x)
		{
			move.x = -1;
		}

		if (!enemy.entity[_i].velocity.y)
		{
			if (_playerPos.y < enemyHitbox.top && enemy.entity[_i].velocity.x)
			{
				move.y = -1;
			}
		}
	}
	return move;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Remplace les macros selon ton projet

#define TRAVEL_MAX 1000

typedef struct AStarNode
{
	sfVector2i directionActual;
	sfBool right;
	sfBool left;
	sfBool up;
	sfBool down;
}AStarNode;

sfVector2i AStar(char** _grid, sfVector2i _gridSize, sfIntRect _startRect, sfIntRect _goalRect, int _jumpHeightTiles)
{
	sfVector2i nextMove = { 0 };
	if (_goalRect.left < 0 || _goalRect.top < 0 || _goalRect.left + _goalRect.width > _gridSize.x || _goalRect.top + _goalRect.height > _gridSize.y)
	{
		return nextMove;
	}

	AStarNode** gridAS = CreateGrid(_gridSize.x, _gridSize.y, sizeof(AStarNode));
	sfBool flag = 0;

	// je regarde ou l'enemy peut aller si il est sur une case
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			for (int i = 0; i < _startRect.height; i++)
			{
				for (int j = 0; j < _startRect.width; j++)
				{
					if (y + i >= _gridSize.y || x + j > _gridSize.x || _grid[y + i][x + j] == 2)
					{
						i = _startRect.height;
						j = _startRect.width;
						flag = 1;
					}
				}
			}
			if (flag)
			{
				flag = 0;
				continue;
			}

			if (x < _gridSize.x - 1)
			{
				gridAS[y][x].right = sfTrue;
				for (int i = 0; i < _startRect.height && gridAS[y][x].right; i++)
				{
					gridAS[y][x].right = (y + i < _gridSize.y && _grid[y + i][x + 1] != 2);
				}
			}

			if (x > 0)
			{
				gridAS[y][x].left = sfTrue;
				for (int i = 0; i < _startRect.height && gridAS[y][x].left; i++)
				{
					gridAS[y][x].left = (y + i < _gridSize.y && _grid[y + i][x - 1] != 2);
				}
			}

			if (y > 0)
			{
				gridAS[y][x].up = sfTrue;
				for (int i = 0; i < _startRect.width && gridAS[y][x].up; i++)
				{
					gridAS[y][x].up = (x + i < _gridSize.x && _grid[y - 1][x + i] != 2);
				}
			}

			if (y < _gridSize.y - 1)
			{
				gridAS[y][x].down = sfTrue;
				for (int i = 0; i < _startRect.width && gridAS[y][x].down; i++)
				{
					gridAS[y][x].down = (x + i < _gridSize.x && !_grid[y + 1][x + i]);
				}
			}
		}
	}

	system("cls");
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			printf("%d %d %d %d\t\t", gridAS[y][x].right, gridAS[y][x].left, gridAS[y][x].up, gridAS[y][x].down);
		}
		printf("\n");
	}

	if (gridAS[_goalRect.top][_goalRect.left].left)
	{
		gridAS[_goalRect.top][_goalRect.left - 1].directionActual.x = 1;
	}
	if (gridAS[_goalRect.top][_goalRect.left].right)
	{
		gridAS[_goalRect.top][_goalRect.left + 1].directionActual.x = -1;
	}
	if (gridAS[_goalRect.top][_goalRect.left].down)
	{
		gridAS[_goalRect.top + 1][_goalRect.left].directionActual.y = -1;
	}
	else if (gridAS[_goalRect.top][_goalRect.left].up)
	{
		gridAS[_goalRect.top - 1][_goalRect.left].directionActual.y = 1;
	}

	flag = sfTrue;
	while (flag)
	{
		flag = sfFalse;
		for (int y = 0; y < _gridSize.y; y++)
		{
			for (int x = 0; x < _gridSize.x; x++)
			{
				if (gridAS[y][x].directionActual.x || gridAS[y][x].directionActual.y)
				{
					if (gridAS[y][x].left && !gridAS[y][x - 1].directionActual.x)
					{
						gridAS[y][x - 1].directionActual.x = 1;
						flag = sfTrue;
					}
					if (gridAS[y][x].right && !gridAS[y][x + 1].directionActual.x)
					{
						gridAS[y][x + 1].directionActual.x = -1;
						flag = sfTrue;
					}
					if (gridAS[y][x].down && !gridAS[y + 1][x].directionActual.y)
					{
						gridAS[y + 1][x].directionActual.y = -1;
						flag = sfTrue;
					}
					else if (gridAS[y][x].up && !gridAS[y - 1][x].directionActual.y)
					{
						gridAS[y - 1][x].directionActual.y = 1;
						flag = sfTrue;
					}
				}
			}
		}
	}

	system("cls");
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			printf("%d %d\t", gridAS[y][x].directionActual.x, gridAS[y][x].directionActual.y);
		}
		printf("\n");
	}

	nextMove = gridAS[_startRect.top][_startRect.left].directionActual;

	return nextMove;
}

#endif //  DEV_PIERRE_ENEMY == 1
