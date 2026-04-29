#include "Ennemy.h"
#include "Map.h"

#if DEV_PIERRE_ENEMY

void UpdateEnemyI(float _dt, int _index);
void CreateEnemyRandom(Ennemy* _ennemy);
void CreateEnemy(Ennemy* _ennemy, EnemyType _type);
void CalculMoveEnemy(float _dt, int _index);
ActionDemander AStar2(int _index, sfFloatRect _cible);
float CalculResultAStar(Case _case);
int MinResultCase(int _type);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);
sfBool TestColision(sfIntRect _intRect);
void DebugTab(Case _case);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);
sfIntRect FloatRectIntoIntRect(sfFloatRect _floatRect);

List* listEnnemy;
EnnemyEntity ennemyEntity[ALEATORY];
MapData* mapData;
Case** aStarMap;
List* listeWait;

sfSprite* sprite;
sfTexture* texture;

Tableau tableau;
int enemyZone;

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
		ennemyEntity[DRONE_SMALL].type = 0;
		ennemyEntity[DRONE_SMALL].ennemydata.life = 3.f;
		ennemyEntity[DRONE_SMALL].ennemydata.energyMax = (float)MAX_ENRGIE;
		ennemyEntity[DRONE_SMALL].ennemydata.energy = (float)MAX_ENRGIE;
		ennemyEntity[DRONE_SMALL].ennemydata.energyRegen = 15.f;
		ennemyEntity[DRONE_SMALL].ennemydata.speedMax = 3.f;
		ennemyEntity[DRONE_SMALL].ennemydata.accelerationMax = 10.f;
		ennemyEntity[DRONE_SMALL].ennemydata.jumForce = 700.f;

		ennemyEntity[DRONE_SMALL].isJetpack = sfTrue;
		ennemyEntity[DRONE_SMALL].jetpack.consomation = 50.f;
		ennemyEntity[DRONE_SMALL].jetpack.life = 5.f;
		ennemyEntity[DRONE_SMALL].jetpack.trust = 10.f;

		ennemyEntity[CROWLER_SMALL].type = 0;
		ennemyEntity[CROWLER_SMALL].ennemydata.life = 3.f;
		ennemyEntity[CROWLER_SMALL].ennemydata.energyMax = (float)MAX_ENRGIE;
		ennemyEntity[CROWLER_SMALL].ennemydata.energy = (float)MAX_ENRGIE;
		ennemyEntity[CROWLER_SMALL].ennemydata.energyRegen = 15.f;
		ennemyEntity[CROWLER_SMALL].ennemydata.speedMax = 3.f;
		ennemyEntity[CROWLER_SMALL].ennemydata.accelerationMax = 10.f;
		ennemyEntity[CROWLER_SMALL].ennemydata.jumForce = 700.f;

		ennemyEntity[CROWLER_SMALL].isJetpack = sfTrue;
		ennemyEntity[CROWLER_SMALL].jetpack.consomation = 50.f;
		ennemyEntity[CROWLER_SMALL].jetpack.life = 5.f;
		ennemyEntity[CROWLER_SMALL].jetpack.trust = 10.f;


		ennemyEntity[SOLDIER_SMALL].type = 1;
		ennemyEntity[SOLDIER_SMALL].ennemydata.life = 2.f;
		ennemyEntity[SOLDIER_SMALL].ennemydata.energyMax = (float)MAX_ENRGIE;
		ennemyEntity[SOLDIER_SMALL].ennemydata.energy = (float)MAX_ENRGIE;
		ennemyEntity[SOLDIER_SMALL].ennemydata.energyRegen = 15.f;
		ennemyEntity[SOLDIER_SMALL].ennemydata.speedMax = 3.f;
		ennemyEntity[SOLDIER_SMALL].ennemydata.accelerationMax = 10.f;
		ennemyEntity[SOLDIER_SMALL].ennemydata.jumForce = 700.f;

		ennemyEntity[SOLDIER_SMALL].isJetpack = sfTrue;
		ennemyEntity[SOLDIER_SMALL].jetpack.consomation = 20.f;
		ennemyEntity[SOLDIER_SMALL].jetpack.life = 5.f;
		ennemyEntity[SOLDIER_SMALL].jetpack.trust = 20.f;
	}
	//SetSaveTemp(ennemyEntity, sizeof(EnnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de ennemyEntity
	mapData = GetMapData(); // connaitre la taille de la map
	if (DEV_ENNEMY)
	{
		printf("size x%d y%d\n", mapData->size.x, mapData->size.y);
	}
}

void UpdateEnemy(float _dt)
{
	enemyZone = 0;
	sfVector2f playerPos = GetPlayerPosition();
	for (int i = GetEnemyCount() - 1; i >= 0; i--)
	{
		Ennemy* enemy = GetElement(listEnnemy, i)->value;
		if (enemy->ennemyEntity.region.left + TILE_SIZE <= playerPos.x && enemy->ennemyEntity.region.left + TILE_SIZE + enemy->ennemyEntity.region.width - TILE_SIZE * 2 >= playerPos.x && enemy->ennemyEntity.region.top + TILE_SIZE <= playerPos.y && enemy->ennemyEntity.region.top + TILE_SIZE + enemy->ennemyEntity.region.height - TILE_SIZE * 2 >= playerPos.y)
		{
			UpdateEnemyI(_dt, i);
			enemyZone++;
		}
	}
	for (char i = 0; i < ALEATORY; i++)
	{
		tableau.new[i] = sfTrue;
	}
}

void UpdateEnemyI(float _dt, int _index)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;
	ennemy->ennemyEntity.timer += _dt;
	ennemy->ennemyEntity.ennemydata.energy += ennemy->ennemyEntity.ennemydata.energyRegen * _dt; //regen de l'energie passive
	if (ennemy->ennemyEntity.timer >= TIMER_ASTAR)
	{
		ennemy->actiondemander = AStar2(_index, GetPlayerRect());
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

void CreateEnemyRandom(Ennemy* _ennemy)
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

void CreateEnemy(Ennemy* _ennemy, EnemyType _type)
{
	if (DEV_MODE)
	{
		printf("creation d'un ennemy de type %d\n", _type);
	}
	//création et aplication des donné de l'ennemy
	_ennemy->ennemyEntity.type = _type;
	_ennemy->ennemyEntity.ennemydata = ennemyEntity[_type].ennemydata;
	_ennemy->ennemyEntity.acceleration = (sfVector2f){ 0,0 };
	_ennemy->ennemyEntity.move = (sfVector2f){ 0,0 };
	_ennemy->ennemyEntity.state = IDLE;
	_ennemy->ennemyEntity.timer = 0.f;

	_ennemy->ennemyEntity.isJetpack = ennemyEntity[_type].isJetpack;
	if (_ennemy->ennemyEntity.isJetpack)
	{
		_ennemy->ennemyEntity.jetpack = ennemyEntity[_type].jetpack;
	}

	switch (_type)
	{
	case DRONE_SMALL:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case DRONE_MEDIUM:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case DRONE_HEAVY:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case CROWLER_SMALL:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case CROWLER_MEDIUM:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case CROWLER_HEAVY:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case SOLDIER_SMALL:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case SOLDIER_MEDIUM:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	case SOLDIER_HEAVY:
		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/capsul.png"), (sfVector2f) { 500, 500 }, 1, 1);
		_ennemy->imageColideur = sfTexture_copyToImage(GetAsset("Assets/Sprites/capsul.png"));
		break;
	default:
		break;
	}

	_ennemy->actiondemander = (ActionDemander){ 0 };
	sfFloatRect floatRect = sfSprite_getGlobalBounds(_ennemy->sprite);
	sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { floatRect.width / 2, floatRect.height });
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

ActionDemander AStar2(int _index, sfFloatRect _cible)
{
	Ennemy* ennemy = GetElement(listEnnemy, _index)->value;

	if (ennemy->ennemyEntity.region.top != tableau.region[ennemy->ennemyEntity.type].top
		|| ennemy->ennemyEntity.region.left != tableau.region[ennemy->ennemyEntity.type].left)
	{

		// liberer lancienne GRID
		tableau.region[ennemy->ennemyEntity.type] = ennemy->ennemyEntity.region;
		sfVector2u gridSize = { ennemy->ennemyEntity.region.width / TILE_SIZE , ennemy->ennemyEntity.region.height / TILE_SIZE };
		tableau.grid[ennemy->ennemyEntity.type] = CreateGrid(gridSize.x, gridSize.y, sizeof(Case2));
		char** grid = CreateGrid(gridSize.x, gridSize.y, sizeof(char));


		for (int y = 0; y < gridSize.y; y++)
		{
			for (int x = 0; x < gridSize.x; x++)
			{
				if (y == 0 || y == gridSize.y - 1 || x == 0 || x == gridSize.x - 1)
				{
					grid[y][x] = 2;
				}
				else
				{
					sfVector2f reaction = Colision((sfFloatRect) { ennemy->ennemyEntity.region.left + x * TILE_SIZE, ennemy->ennemyEntity.region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
					if (reaction.x || reaction.y)
					{
						grid[y][x] = 2;
					}
					else if (CollisionPassThrough((sfFloatRect) { ennemy->ennemyEntity.region.left + x * TILE_SIZE, ennemy->ennemyEntity.region.top + y  * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
					{
						grid[y][x] = 1;
					}
				}
			}
		}
		tableau.collision = grid;
		if (DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < gridSize.y; y++)
			{
				for (int x = 0; x < gridSize.x; x++)
				{
					printf("%d", tableau.collision[y][x]);
				}
				printf("\n");
			}
			printf("\n");
			printf("\n");
			printf("\n");
		}
	};

	sfFloatRect bouns = GetBounsEnemy(_index);
	bouns.left -= ennemy->ennemyEntity.region.left;
	bouns.top -= ennemy->ennemyEntity.region.top;
	sfIntRect bounsEnnemy = FloatRectIntoIntRect(bouns);
	_cible.left -= ennemy->ennemyEntity.region.left;
	_cible.top -= ennemy->ennemyEntity.region.top;
	sfIntRect bounsCible = FloatRectIntoIntRect(_cible);


	if (tableau.new[ennemy->ennemyEntity.type])
	{
		for (int y = 0; y < (int) { ennemy->ennemyEntity.region.height / TILE_SIZE }; y++)
		{
			for (int x = 0; x < (int) { ennemy->ennemyEntity.region.width / TILE_SIZE }; x++)
			{
				tableau.grid[ennemy->ennemyEntity.type][y][x].direction = EMPTY_DIRECTION;
			}
		}

		tableau.grid[ennemy->ennemyEntity.type][bounsCible.top][bounsCible.left].compteur = 0;
		tableau.grid[ennemy->ennemyEntity.type][bounsCible.top][bounsCible.left].direction = NO_DIRECTION;

		sfVector2u caseGet = { bounsCible.left, bounsCible.top };
		sfIntRect caseRecherche = { 0 };

		tableau.new[ennemy->ennemyEntity.type] = sfFalse;

		AjoutListWait((sfVector2u) { bounsCible.left, bounsCible.top });

		while (GetListSize(listeWait) > 0)
		{
			// test sol
			sfVector2u* temp = GetElement(listeWait, MinResultCase(ennemy->ennemyEntity.type))->value;
			caseGet = *temp;
			caseRecherche = (sfIntRect){ caseGet.x, caseGet.y + 1,bounsCible.width,bounsCible.height };
			if (TestColision(caseRecherche))//si sur sol
			{
				//test gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y ,bounsCible.width,bounsCible.height };
				if (!TestColision(caseRecherche) &&
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].compteur++;
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}
				//test Droite
				caseRecherche = (sfIntRect){ caseGet.x + 1, caseGet.y ,bounsCible.width,bounsCible.height };
				if (!TestColision(caseRecherche) &&
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].compteur++;
					tableau.grid[ennemy->ennemyEntity.type][caseRecherche.top][caseRecherche.left].direction = LEFT;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}
			}
			RetirerListWait(0);
		}
		if (DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < (int) { ennemy->ennemyEntity.region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { ennemy->ennemyEntity.region.width / TILE_SIZE }; x++)
				{
					if (!tableau.grid[ennemy->ennemyEntity.type][y][x].direction)
					{
						if (tableau.collision[y][x])
						{
							printf("X");
						}
						else
						{
							printf("%d", tableau.grid[ennemy->ennemyEntity.type][y][x].direction);
						}
					}
					else
					{
						printf("%d", tableau.grid[ennemy->ennemyEntity.type][y][x].direction);
					}
				}
				printf("\n");
			}
			printf("\n");
			printf("\n");
			printf("\n");
		}
	}

	ActionDemander actionDemander = { 0 };

	switch (tableau.grid[ennemy->ennemyEntity.type][bounsEnnemy.left][bounsEnnemy.top].direction)
	{
	case EMPTY_DIRECTION:
		break;
	case DOWN_LEFT:
		break;
	case DOWN:
		break;
	case DOWN_RIGHT:
		break;
	case LEFT:
		actionDemander.gauche = sfTrue;
		break;
	case NO_DIRECTION:
		break;
	case RIGHT:
		actionDemander.droite = sfTrue;
		break;
	case UP_LEFT:
		break;
	case UP:
		break;
	case UP_RIGHT:
		break;
	default:
		break;
	}

	return actionDemander;
}

float CalculResultAStar(Case _case)
{
	return (float) { _case.rangeToDestination + _case.action + MAX_ENRGIE - _case.energie };
}

int MinResultCase(int _type) // recherche du plus petit resultat dans la liste chainé listeWait
{
	int min = 0;
	if (GetListSize(listeWait) > 1)
	{
		for (unsigned i = 1; i < GetListSize(listeWait); i++)
		{
			sfVector2u* caseGet = GetElement(listeWait, i)->value;
			sfVector2u* caseMin = GetElement(listeWait, min)->value;
			if (tableau.grid[_type][caseGet->y][caseGet->x].compteur < tableau.grid[_type][caseMin->y][caseMin->x].compteur)
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

sfBool TestColision(sfIntRect _intRect)
{
	sfBool temp = sfFalse;
	for (int i = 0; i < abs(_intRect.height); i++)
	{
		for (int t = 0; t < _intRect.width; t++)
		{
			if (tableau.collision[_intRect.top - i][_intRect.left + t])
			{
				temp = sfTrue;
			}
		}
	}
	return temp;
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
	if (mapData && aStarMap)
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

int GetEnemyZone(void)
{
	return enemyZone;
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

sfIntRect FloatRectIntoIntRect(sfFloatRect _floatRect)
{
	sfIntRect intRect = { 0 };
	intRect.height = (int)(_floatRect.height / TILE_SIZE) + 1;
	intRect.width = (int)(_floatRect.width / TILE_SIZE) + 1;
	intRect.left = (int)(_floatRect.left / TILE_SIZE);
	intRect.top = (int)(_floatRect.top / TILE_SIZE) + intRect.height - 1;
	intRect.height = -intRect.height;

	return intRect;
}

void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region)
{
	_region.left -= TILE_SIZE;
	_region.top -= TILE_SIZE;
	_region.width += TILE_SIZE * 2;
	_region.height += TILE_SIZE * 2;


	Ennemy* ennemy = Calloc(1, sizeof(Ennemy));
	Element* element = CreateElement(ennemy);
	element->value = ennemy;
	switch (_type)
	{
	case DRONE_SMALL:
		CreateEnemy(ennemy, DRONE_SMALL);
		break;
	case DRONE_MEDIUM:
		break;
	case DRONE_HEAVY:
		break;
	case CROWLER_SMALL:
		CreateEnemy(ennemy, CROWLER_SMALL);
		break;
	case CROWLER_MEDIUM:
		break;
	case CROWLER_HEAVY:
		break;
	case SOLDIER_SMALL:
		CreateEnemy(ennemy, SOLDIER_SMALL);
		break;
	case SOLDIER_MEDIUM:
		break;
	case SOLDIER_HEAVY:
		break;
	case ALEATORY:
		CreateEnemyRandom(ennemy);
		break;
	default:
		break;
	}

	ennemy->ennemyEntity.region = _region;

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

Enemy enemy;

void LoadEnemy(void)
{
	enemy = (Enemy){ 0 };
	enemy.entity = Calloc(1, sizeof(EnemyEntity));

	enemy.data[SOLDIER_SMALL] = (EnemyData){ GetAsset("Assets/Sprites/spider_small.png") , 100.f, 5.f, 300.f, 100.f, 1.f };
	enemy.data[SOLDIER_MEDIUM] = (EnemyData){ GetAsset("Assets/Sprites/spider_medium.png") , 100.f, 5.f, 300.f, 100.f , 3.f };
	enemy.data[SOLDIER_HEAVY] = (EnemyData){ GetAsset("Assets/Sprites/spider_large.png") , 100.f, 5.f, 300.f, 100.f , 10.f };
}

void UpdateEnemy(float _dt)
{
#if DEV_ENEMY_BASIC
	sfVector2f playerPosition = GetPlayerPosition();
#else
#endif
	sfVector2i enemyMove = { 0 };
	for (int i = 0; i < enemy.count; i++)
	{
#if DEV_ENEMY_BASIC
		enemyMove = GetMoveEnemyAITemp(i, playerPosition);
#else
		enemyMove = GetMoveEnemyAI(i, playerPosition);
#endif


		if (enemyMove.x)
		{
			enemy.entity[i].velocity.x = enemyMove.x * enemy.data[enemy.entity[i].type].speed;
		}
		else
		{
			enemy.entity[i].velocity.x = 0;
		}
		if (enemyMove.y < 0 && !enemy.entity[i].velocity.y)
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
			colision.y -= (int)abs((int)colision.x);
			enemy.entity[i].velocity.x = 0;
		}

		sfSprite_move(enemy.entity[i].sprite, colision);

		sfFloatRect eBox = sfSprite_getGlobalBounds(enemy.entity[i].sprite);
		sfFloatRect pBox = GetPlayerRect();
		if (sfFloatRect_intersects(&pBox, &eBox, NULL))
		{
			DamagePlayer(1);
		}
	}
}

sfBool HitEnemy(float _degat, sfFloatRect _hitbox)
{
	sfFloatRect hitboxEnemy = { 0 };
	//sfVector2f playerPos = GetPlayerPosition();
	sfVector2i enemyAction = { 0 };
	for (int i = 0; i < enemy.count; i++)
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
	for (int i = 0; i < enemy.count; i++)
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
	//sfVector2f pos = sfSprite_getPosition(enemy.entity[enemy.count].sprite);
	enemy.entity[enemy.count].type = _type;
	enemy.entity[enemy.count].region = _region;
	enemy.entity[enemy.count].life = enemy.data[_type].lifeMax;
	enemy.count++;
}

sfVector2i AStar1(char** _grid, sfVector2i _gridSize, sfIntRect _startRect, sfIntRect _goalRect, int _jumpHeightTiles);
sfVector2i AStar2(char** grid, sfVector2i gridSize, sfIntRect start, sfIntRect goal, int jumpHeightTiles);
sfVector2i AStar3(char** _grid, sfVector2i _gridSize, sfIntRect _start, sfIntRect _end, int _jumpCase);

sfVector2i GetMoveEnemyAI(unsigned _i, sfVector2f _playerPos)
{
	sfVector2i move = { 0 };
	if (enemy.entity[_i].region.left <= _playerPos.x && enemy.entity[_i].region.left + enemy.entity[_i].region.width >= _playerPos.x && enemy.entity[_i].region.top <= _playerPos.y && enemy.entity[_i].region.top + enemy.entity[_i].region.height >= _playerPos.y)
	{
		sfFloatRect playerBox = GetPlayerRect();
		sfIntRect playerHitbox = { (int)(playerBox.left - enemy.entity[_i].region.left) / TILE_SIZE,(int)(playerBox.top - enemy.entity[_i].region.top) / TILE_SIZE, (int)playerBox.width / TILE_SIZE, (int)playerBox.height / TILE_SIZE };

		sfVector2i gridSize = { (int)enemy.entity[_i].region.width / TILE_SIZE, (int)enemy.entity[_i].region.height / TILE_SIZE };
		char** grid = (char**)CreateGrid(gridSize.x, gridSize.y, sizeof(char));

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
		sfIntRect enemyHitbox = { (int)(enemybox.left - enemy.entity[_i].region.left) / TILE_SIZE + 1,(int)(enemybox.top - enemy.entity[_i].region.top) / TILE_SIZE,(int)enemybox.width / TILE_SIZE - 1,(int)enemybox.height / TILE_SIZE - 1 };

		move = AStar3(grid, gridSize, enemyHitbox, playerHitbox, 5);

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

typedef struct AStarNode
{
	sfVector2i directionActual;
	sfBool right;
	sfBool left;
	sfBool up;
	sfBool down;
}AStarNode;

sfVector2i AStar1(char** _grid, sfVector2i _gridSize, sfIntRect _startRect, sfIntRect _goalRect, int _jumpHeightTiles)
{
	sfVector2i nextMove = { 0 };
	if (_goalRect.left < 0 || _goalRect.top < 0 || _goalRect.left + _goalRect.width > _gridSize.x || _goalRect.top + _goalRect.height - 1 > _gridSize.y)
	{
		return nextMove;
	}
	if (_startRect.left < 0 || _startRect.top < 0 || _startRect.left + _startRect.width > _gridSize.x || _startRect.top + _startRect.height - 1 > _gridSize.y)
	{
		return nextMove;
	}

	AStarNode** gridAS = (AStarNode**)CreateGrid(_gridSize.x, _gridSize.y, sizeof(AStarNode));
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

	/*
	system("cls");
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			printf("%d %d %d %d    ", gridAS[y][x].right, gridAS[y][x].left, gridAS[y][x].up, gridAS[y][x].down);
		}
		printf("\n");
	}
	*/

	if (gridAS[_goalRect.top][_goalRect.left].left && gridAS[_goalRect.top][_goalRect.left - 1].right)
	{
		gridAS[_goalRect.top][_goalRect.left - 1].directionActual.x = 1;
	}
	if (gridAS[_goalRect.top][_goalRect.left].right && gridAS[_goalRect.top][_goalRect.left + 1].left)
	{
		gridAS[_goalRect.top][_goalRect.left + 1].directionActual.x = -1;
	}
	if (gridAS[_goalRect.top][_goalRect.left].down && gridAS[_goalRect.top + 1][_goalRect.left].up)
	{
		gridAS[_goalRect.top + 1][_goalRect.left].directionActual.y = -1;
	}
	else if (gridAS[_goalRect.top][_goalRect.left].up && gridAS[_goalRect.top - 1][_goalRect.left].down)
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
					if (gridAS[y][x].left && !gridAS[y][x - 1].directionActual.x && gridAS[y][x - 1].right)
					{
						gridAS[y][x - 1].directionActual.x = 1;
						flag = sfTrue;
					}
					if (gridAS[y][x].right && !gridAS[y][x + 1].directionActual.x && gridAS[y][x + 1].left)
					{
						gridAS[y][x + 1].directionActual.x = -1;
						flag = sfTrue;
					}
					if (gridAS[y][x].down && !gridAS[y + 1][x].directionActual.y && gridAS[y + 1][x].up)
					{
						gridAS[y + 1][x].directionActual.y = -1;
						flag = sfTrue;
					}
					else if (gridAS[y][x].up && !gridAS[y - 1][x].directionActual.y && gridAS[y - 1][x].down)
					{
						gridAS[y - 1][x].directionActual.y = 1;
						flag = sfTrue;
					}
				}
			}
		}
	}
	/*
	system("cls");
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			printf("%d %d\t", gridAS[y][x].directionActual.x, gridAS[y][x].directionActual.y);
		}
		printf("\n");
	}
	*/
	nextMove = gridAS[_startRect.top][_startRect.left].directionActual;

	printf("%d %d\t", nextMove.x, nextMove.y);
	return nextMove;
}

static sfBool CheckColisiontMap(sfIntRect _enemyBox, char** map, sfVector2i _mapSize)
{
	for (int y = 0; y < _enemyBox.height; y++)
	{
		for (int x = 0; x < _enemyBox.width; x++)
		{
			if (_enemyBox.left < 0 || _enemyBox.top < 0 || _enemyBox.left + _enemyBox.width > _mapSize.x || _enemyBox.top + _enemyBox.height > _mapSize.y || map[_enemyBox.top + y][_enemyBox.left + x])
			{
				return sfTrue;
			}
		}
	}
	return sfFalse;

}

sfVector2i AStar3(char** _grid, sfVector2i _gridSize, sfIntRect _start, sfIntRect _end, int _jumpTile)
{
	if (CheckColisiontMap(_start, _grid, _gridSize) || CheckColisiontMap(_end, _grid, _gridSize))
	{
		return (sfVector2i) { 0 };
	}

	sfVector2i andCase = { 0 };
	AStarNode** gridAS = (AStarNode**)CreateGrid(_gridSize.x, _gridSize.y, sizeof(AStarNode));
	for (int y = 0; y < _end.top + _start.height; y++)
	{
		for (int x = 0; x < _end.left + _end.width; x++)
		{
			if (!CheckColisiontMap((sfIntRect) { _end.left - _start.width + x, _end.top - _start.height + y, _start.width, _start.height }, _grid, _gridSize))
			{
				gridAS[_end.top - _start.height + y][_end.left - _start.width + x].directionActual = (sfVector2i){ 9, 9 };
			}
		}
	}

	system("cls");
	for (int y = 0; y < _gridSize.y; y++)
	{
		for (int x = 0; x < _gridSize.x; x++)
		{
			printf("%d %d  ", gridAS[y][x].directionActual.x, gridAS[y][x].directionActual.y);
		}
		printf("\n");
	}

	sfBool flag = sfTrue;
	while (flag)
	{
		flag = sfFalse;
		for (int y = 0; y < _gridSize.y; y++)
		{
			for (int x = 0; x < _gridSize.x; x++)
			{
				if (gridAS[y][x].directionActual.x || gridAS[y][x].directionActual.y)
				{
					if (x - 1 > 0 && !(gridAS[y][x - 1].directionActual.x + gridAS[y][x - 1].directionActual.y) && !CheckColisiontMap((sfIntRect) { x - 1, y, _start.width, _start.height }, _grid, _gridSize))
					{
						gridAS[y][x - 1].directionActual.x = -1;
						flag = sfTrue;
						if (x - 1 == _start.left && y == _start.top)
						{
							return (sfVector2i) { 1, 0 };
						}
					}
					if (x + 1 < _gridSize.x && !(gridAS[y][x + 1].directionActual.x + gridAS[y][x + 1].directionActual.y) && !CheckColisiontMap((sfIntRect) { x + 1, y, _start.width, _start.height }, _grid, _gridSize))
					{
						gridAS[y][x + 1].directionActual.x = 1;
						flag = sfTrue;
						if (x + 1 == _start.left && y == _start.top)
						{
							return (sfVector2i) { 1, 0 };
						}
					}
					if (y - 1 > 0 && !(gridAS[y - 1][x].directionActual.x + gridAS[y - 1][x].directionActual.y) && !CheckColisiontMap((sfIntRect) { x, y - 1, _start.width, _start.height }, _grid, _gridSize))
					{
						gridAS[y - 1][x].directionActual.x = 1;
						flag = sfTrue;
						if (x == _start.left && y - 1 == _start.top)
						{
							return (sfVector2i) { 1, 0 };
						}
					}
					if (y + 1 < _gridSize.y && !(gridAS[y + 1][x].directionActual.x + gridAS[y + 1][x].directionActual.y) && !CheckColisiontMap((sfIntRect) { x, y + 1, _start.width, _start.height }, _grid, _gridSize))
					{
						int jumpTotal = 0;
						for (int i = 1; i < _jumpTile + 1; i++)
						{
							if (y + i < _gridSize.y && !(gridAS[y + 1][x].directionActual.x + gridAS[y + i][x].directionActual.y) && !CheckColisiontMap((sfIntRect) { x, y + i, _start.width, _start.height }, _grid, _gridSize))
							{
								jumpTotal = i;
							}
							else
							{
								continue;
							}
						}

						if (jumpTotal < _jumpTile)
						{
							for (int i = 0; i < jumpTotal; i++)
							{
								gridAS[y + i][x].directionActual.x = 1;
								flag = sfTrue;
								if (x == _start.left && y + i == _start.top)
								{
									return (sfVector2i) { 1, 0 };
								}
							}
						}
					}
					system("cls");
					for (int y = 0; y < _gridSize.y; y++)
					{
						for (int x = 0; x < _gridSize.x; x++)
						{
							printf("%d %d,", gridAS[y][x].directionActual.x, gridAS[y][x].directionActual.y);
						}
						printf("\n");
					}
				}

			}
		}
	}



	return (sfVector2i) { 0 };
}

#endif //  DEV_PIERRE_ENEMY == 1
