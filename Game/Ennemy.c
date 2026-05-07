#include "Enemy.h"
#include "Map.h"
#include "Projectiles.h"

#define INIT_STRUCT_ENEMY_ENTITY(type,enemydata, isJetpack, jetpack,move, state, timer, timerTir, powerGel, timerGel, region)(EnemyEntity) {type,enemydata, isJetpack, jetpack, move, state, timer, timerTir, powerGel, timerGel, region}
#define INIT_STRUCT_ENEMY_DATA(life, energyMax, energy, energyRegen, accelerationMax, speedMax, jumForce, nbCaseJump, armor) (EnemyData) {life, energyMax, energy, energyRegen, accelerationMax, speedMax, jumForce, nbCaseJump, armor}
#define INIT_STRUCT_ENEMY_JETPACK( trust, consomation, life) (Jetpack) {trust, consomation, life}

void UpdateColisionEnemy(Enemy* _enemy);
void UpdateEnemyI(float _dt, int _index, Enemy* _enemy);
void CreateEnemyRandom(Enemy* _enemy);
void CreateEnemy(Enemy* _enemy, EnemyType _type);
void CalculMoveEnemy(float _dt, Enemy* _enemy);
ActionDemander AStar2(int _index, sfFloatRect _cible);
float CalculResultAStar(Case _case);
int MinResultCase(int _type);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);
int TestColision(sfIntRect _intRect);
int TestJump(sfIntRect _intRect);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);
sfIntRect FloatRectIntoIntRect(sfFloatRect _floatRect);
sfBool PlayerVisibility(int _index);
void shootPlayer(int _index);
sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat, AttackType _attaque);
void EffectGelEnemy(Enemy* _enemy, int _puissance, float _time);

List* listEnemy;
EnemyEntity enemyEntity[ALEATORY];
MapData* mapData;
Case** aStarMap;
List* listeWait;

sfSprite* sprite;
sfTexture* texture;

Tableau tableau;
int enemyZone;

void LoadEnemy(void)
{
	listEnemy = 0;
	mapData = 0;
	aStarMap = 0;
	listeWait = 0;
	sprite = 0;
	texture = 0;
	tableau = (Tableau){ 0 };
	enemyZone = 0;
	for (int i = 0; i < ALEATORY; i++)
	{
		enemyEntity[i] = (EnemyEntity){ 0 };
	}

	listEnemy = CreateList();
	listeWait = CreateList();
	if (!DEV_MODE)
	{
		GetSaveTemp(enemyEntity, sizeof(EnemyEntity), ALEATORY);
	}
	else // charger les diférent type d'enemy
	{
		Jetpack jetpack = INIT_STRUCT_ENEMY_JETPACK(10.f, 50.f, 5.f);
		EnemyData data = INIT_STRUCT_ENEMY_DATA(3.f, (float)MAX_ENRGIE, (float)MAX_ENRGIE, 15.f, 10.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 6, MEDIUM_ARMOR);
		enemyEntity[SOLDIER_SMALL] = INIT_STRUCT_ENEMY_ENTITY(0, data, sfTrue, jetpack, 10.f, 0.f, 0, 0.f, 0.f, 0.f, 0.f, 0);

		data = INIT_STRUCT_ENEMY_DATA(3.f, (float)MAX_ENRGIE, (float)MAX_ENRGIE, 15.f, 10.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 6, LIGHT_ARMOR);
		enemyEntity[DRONE_SMALL] = INIT_STRUCT_ENEMY_ENTITY(0, data, sfTrue, jetpack, 10.f, 0.f, 0, 0.f, 0.f, 0.f, 0.f, 0);

		data = INIT_STRUCT_ENEMY_DATA(3.f, (float)MAX_ENRGIE, (float)MAX_ENRGIE, 15.f, 10.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 6, HEAVY_ARMOR);
		enemyEntity[GROUND_HEAVY] = INIT_STRUCT_ENEMY_ENTITY(0, data, sfTrue, jetpack, 10.f, 0.f, 0, 0.f, 0.f, 0.f, 0.f, 0);

		/*enemyEntity[CROWLER_SMALL].type = 0;
		enemyEntity[CROWLER_SMALL].enemydata.life = 3.f;
		enemyEntity[CROWLER_SMALL].enemydata.energyMax = (float)MAX_ENRGIE;
		enemyEntity[CROWLER_SMALL].enemydata.energy = (float)MAX_ENRGIE;
		enemyEntity[CROWLER_SMALL].enemydata.energyRegen = 15.f;
		enemyEntity[CROWLER_SMALL].enemydata.speedMax = 1.f;
		enemyEntity[CROWLER_SMALL].enemydata.accelerationMax = 10.f;
		enemyEntity[CROWLER_SMALL].enemydata.nbCaseJump = 5;
		enemyEntity[CROWLER_SMALL].enemydata.jumForce = enemyEntity[CROWLER_SMALL].enemydata.nbCaseJump * TILE_SIZE / G / 5.5f;

		enemyEntity[CROWLER_SMALL].isJetpack = sfTrue;
		enemyEntity[CROWLER_SMALL].jetpack.consomation = 500.f;
		enemyEntity[CROWLER_SMALL].jetpack.life = 5.f;
		enemyEntity[CROWLER_SMALL].jetpack.trust = 10.f;


		enemyEntity[SOLDIER_SMALL].type = 1;
		enemyEntity[SOLDIER_SMALL].enemydata.life = 2.f;
		enemyEntity[SOLDIER_SMALL].enemydata.energyMax = (float)MAX_ENRGIE;
		enemyEntity[SOLDIER_SMALL].enemydata.energy = (float)MAX_ENRGIE;
		enemyEntity[SOLDIER_SMALL].enemydata.energyRegen = 15.f;
		enemyEntity[SOLDIER_SMALL].enemydata.speedMax = 1.f;
		enemyEntity[SOLDIER_SMALL].enemydata.accelerationMax = 10.f;
		enemyEntity[SOLDIER_SMALL].enemydata.nbCaseJump = 5;
		enemyEntity[SOLDIER_SMALL].enemydata.jumForce = enemyEntity[SOLDIER_SMALL].enemydata.nbCaseJump * TILE_SIZE / G / 5.5f;

		enemyEntity[SOLDIER_SMALL].isJetpack = sfTrue;
		enemyEntity[SOLDIER_SMALL].jetpack.consomation = 20.f;
		enemyEntity[SOLDIER_SMALL].jetpack.life = 5.f;
		enemyEntity[SOLDIER_SMALL].jetpack.trust = 20.f;*/
	}
	//SetSaveTemp(enemyEntity, sizeof(EnemyEntity), ALEATORY); // a relancer 1 fois a chaque changement de enemyEntity
	mapData = GetMapData(); // connaitre la taille de la map
}

void UpdateEnemy(float _dt)
{
	enemyZone = 0;
	sfVector2f playerPos = GetPlayerPosition();

	Element* elementActualy = listEnemy->first;
	int i = 0;
	while (elementActualy)
	{
		Enemy* enemy = elementActualy->value;
		if (enemy->enemyEntity.region.left + TILE_SIZE <= playerPos.x && enemy->enemyEntity.region.left + TILE_SIZE + enemy->enemyEntity.region.width - TILE_SIZE * 2 >= playerPos.x && enemy->enemyEntity.region.top + TILE_SIZE <= playerPos.y && enemy->enemyEntity.region.top + TILE_SIZE + enemy->enemyEntity.region.height - TILE_SIZE * 2 >= playerPos.y)
		{
			UpdateEnemyI(_dt, i, enemy);
			enemyZone++;
		}
		i++;
		elementActualy = elementActualy->next;
	}

	for (char i = 0; i < ALEATORY; i++)
	{
		tableau.new[i] = sfTrue;
	}
}

void UpdateEnemyI(float _dt, int _index, Enemy* _enemy)
{
	if (_enemy->enemyEntity.timerGel > 0)
	{
		_enemy->enemyEntity.timerGel -= _dt;
		_dt = _dt / _enemy->enemyEntity.powerGel;
		if (_enemy->enemyEntity.timerGel < 0)
		{
			_enemy->enemyEntity.timerGel = 0;
		}
	}
	_enemy->enemyEntity.timer += _dt;
	_enemy->enemyEntity.timerTir += _dt;
	if (_enemy->enemyEntity.enemydata.energy < _enemy->enemyEntity.enemydata.energyMax)
	{
		_enemy->enemyEntity.enemydata.energy += _enemy->enemyEntity.enemydata.energyRegen * _dt;
	}
	else
	{
		_enemy->enemyEntity.enemydata.energy = _enemy->enemyEntity.enemydata.energyMax;
	}
	if (PlayerVisibility(_index))
	{
		if (_enemy->enemyEntity.timerTir >= 1)
		{
			_enemy->enemyEntity.timerTir = 0;
			shootPlayer(_index);
		}
		_enemy->actiondemander = (ActionDemander){ 0 };
	}
	else
	{
		if (_enemy->enemyEntity.timer >= TIMER_ASTAR)
		{
			_enemy->actiondemander = AStar2(_index, GetPlayerRect());
			_enemy->enemyEntity.timer = 0;
		}
	}
	CalculMoveEnemy(_dt, _enemy);
	UpdateColisionEnemy(_enemy);

}

void UpdateColisionEnemy(Enemy* _enemy)
{
	sfFloatRect enemyRect = sfSprite_getGlobalBounds(_enemy->sprite);
	sfFloatRect realRegion = { _enemy->enemyEntity.region.left + TILE_SIZE ,_enemy->enemyEntity.region.top + TILE_SIZE , _enemy->enemyEntity.region.width - TILE_SIZE * 2, _enemy->enemyEntity.region.height - TILE_SIZE * 2 };

	sfVector2f collision = Colision(sfSprite_getGlobalBounds(_enemy->sprite), AXIS_BOTH);
	collision.y += CollisionPassThrough(sfSprite_getGlobalBounds(_enemy->sprite)).y;
	sfSprite_move(_enemy->sprite, collision);
	if (collision.x)
	{
		_enemy->enemyEntity.move.x = 0;
	}
	if (collision.y)
	{
		_enemy->enemyEntity.move.y = 0;
	}

	if (realRegion.left > enemyRect.left)
	{
		sfSprite_move(_enemy->sprite, (sfVector2f) { realRegion.left - enemyRect.left, 0 });
	}
	else if (realRegion.left + realRegion.width < (enemyRect.left + enemyRect.width))
	{
		sfSprite_move(_enemy->sprite, (sfVector2f) { (realRegion.left + realRegion.width) - (enemyRect.left + enemyRect.width), 0 });
	}
	if (realRegion.top > enemyRect.top)
	{
		sfSprite_move(_enemy->sprite, (sfVector2f) { 0, realRegion.top - enemyRect.top });
	}
	else if (realRegion.top + realRegion.height < enemyRect.top + enemyRect.height)
	{
		sfSprite_move(_enemy->sprite, (sfVector2f) { 0, (realRegion.top + realRegion.height) - (enemyRect.top + enemyRect.height) });
	}
}

void CreateEnemyRandom(Enemy* _enemy)
{
	enum EnemyType temp = rand() % ALEATORY;
	switch (temp)
	{
	case GROUND_HEAVY:
		CreateEnemy(_enemy, GROUND_HEAVY);
		break;
	case SOLDIER_SMALL:
		CreateEnemy(_enemy, SOLDIER_SMALL);
		break;
	case DRONE_SMALL:
		CreateEnemy(_enemy, DRONE_SMALL);
		break;
	default:
		CreateEnemy(_enemy, DRONE_SMALL);
		break;
	}
}

void CreateEnemy(Enemy* _enemy, EnemyType _type)
{
	*_enemy = (Enemy){ 0 };
	//création et aplication des donné de l'enemy
	_enemy->enemyEntity.type = _type;
	_enemy->enemyEntity.enemydata = enemyEntity[_type].enemydata;
	_enemy->enemyEntity.move = (sfVector2f){ 0,0 };
	_enemy->enemyEntity.timer = 0.f;

	_enemy->enemyEntity.isJetpack = enemyEntity[_type].isJetpack;
	if (_enemy->enemyEntity.isJetpack)
	{
		_enemy->enemyEntity.jetpack = enemyEntity[_type].jetpack;
	}

	switch (_type)
	{
	case DRONE_SMALL:
		_enemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		break;
	case GROUND_HEAVY:
		_enemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		break;
	case SOLDIER_SMALL:
		_enemy->sprite = CreateSprite(GetAsset("Assets/Sprites/spider_small.png"), (sfVector2f) { 500, 500 }, 1, 1);
		break;
	default:
		break;
	}

	SetSpriteOriginFoot(_enemy->sprite);
}

void CalculMoveEnemy(float _dt, Enemy* _enemy)
{
	// 1 = droite / -1 = gauche
	char droitOuGauche = _enemy->actiondemander.droite - _enemy->actiondemander.gauche;
	if (droitOuGauche)
	{
		_enemy->enemyEntity.move.x += droitOuGauche * _enemy->enemyEntity.enemydata.accelerationMax * _dt;
		if (droitOuGauche * _enemy->enemyEntity.move.x > droitOuGauche * _enemy->enemyEntity.enemydata.speedMax)
		{
			_enemy->enemyEntity.move.x = droitOuGauche * _enemy->enemyEntity.enemydata.speedMax;
		}
	}
	else
	{
		_enemy->enemyEntity.move.x = 0;
	}

	if (_enemy->actiondemander.Saut)
	{
		sfFloatRect enemyRect = sfSprite_getGlobalBounds(_enemy->sprite);
		enemyRect.top += 1;
		sfVector2f collision = Colision(enemyRect, AXIS_BOTH);
		collision.y += CollisionPassThrough(enemyRect).y;
		if (collision.y > -2 && collision.y < 0)
		{
			_enemy->enemyEntity.move.y = -_enemy->enemyEntity.enemydata.jumForce;
		}
		else if (0 && _enemy->enemyEntity.move.y >= 0 && _enemy->enemyEntity.isJetpack && _enemy->enemyEntity.jetpack.consomation * _dt < _enemy->enemyEntity.enemydata.energy)
		{
			_enemy->enemyEntity.move.y -= _enemy->enemyEntity.enemydata.jumForce;
			_enemy->enemyEntity.enemydata.energy -= _enemy->enemyEntity.jetpack.consomation * _dt;
		}
	}
	_enemy->enemyEntity.move.y += G * _dt;

	if (_enemy->enemyEntity.move.y > _enemy->enemyEntity.enemydata.jumForce)
	{
		_enemy->enemyEntity.move.y = _enemy->enemyEntity.enemydata.jumForce;
	}

	sfSprite_move(_enemy->sprite, _enemy->enemyEntity.move);

}

ActionDemander AStar2(int _index, sfFloatRect _cible)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;
	sfVector2u gridSize = { enemy->enemyEntity.region.width / TILE_SIZE , enemy->enemyEntity.region.height / TILE_SIZE };
	if (enemy->enemyEntity.region.top != tableau.region[enemy->enemyEntity.type].top || enemy->enemyEntity.region.left != tableau.region[enemy->enemyEntity.type].left)
	{
		// liberer lancienne GRID
		tableau.region[enemy->enemyEntity.type] = enemy->enemyEntity.region;

		FreeGrid(tableau.grid[enemy->enemyEntity.type]);
		FreeGrid(tableau.collision);
		tableau.grid[enemy->enemyEntity.type] = CreateGrid(gridSize.x, gridSize.y, sizeof(Case2));
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
					sfVector2f reaction = Colision((sfFloatRect) { enemy->enemyEntity.region.left + x * TILE_SIZE, enemy->enemyEntity.region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
					if (reaction.x || reaction.y)
					{
						grid[y][x] = 2;
					}
					else if (CollisionPassThrough((sfFloatRect) { enemy->enemyEntity.region.left + x * TILE_SIZE, enemy->enemyEntity.region.top + y * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
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
			printf("\n\n\n");
		}
	};

	sfFloatRect bouns = GetBounsEnemy(_index);
	bouns.left -= enemy->enemyEntity.region.left;
	bouns.top -= enemy->enemyEntity.region.top + 1;
	sfIntRect bounsEnemy = FloatRectIntoIntRect(bouns);
	_cible.left -= enemy->enemyEntity.region.left;
	_cible.top -= enemy->enemyEntity.region.top + 1;
	sfIntRect bounsCible = FloatRectIntoIntRect(_cible);
	// problème colision plafon
	while (bounsCible.top + bounsEnemy.height < 1)
	{
		bounsCible.top += 1;
	}
	//problème collision mur de droite
	while (bounsCible.left + bounsEnemy.width > gridSize.x - 1)
	{
		bounsCible.left -= 1;
	}


	if (tableau.new[enemy->enemyEntity.type])
	{
		for (int y = 0; y < (int) { enemy->enemyEntity.region.height / TILE_SIZE }; y++)
		{
			for (int x = 0; x < (int) { enemy->enemyEntity.region.width / TILE_SIZE }; x++)
			{
				tableau.grid[enemy->enemyEntity.type][y][x].direction = EMPTY_DIRECTION;
				tableau.grid[enemy->enemyEntity.type][y][x].compteur = 0;
				tableau.grid[enemy->enemyEntity.type][y][x].jumpForce = 0;
			}
		}

		tableau.grid[enemy->enemyEntity.type][bounsCible.top][bounsCible.left].compteur = 0;
		tableau.grid[enemy->enemyEntity.type][bounsCible.top][bounsCible.left].jumpForce = 0;
		tableau.grid[enemy->enemyEntity.type][bounsCible.top][bounsCible.left].direction = NO_DIRECTION;

		sfVector2u caseGet = { bounsCible.left, bounsCible.top };
		sfIntRect caseRecherche = { 0 };

		tableau.new[enemy->enemyEntity.type] = sfFalse;

		AjoutListWait((sfVector2u) { bounsCible.left, bounsCible.top });

		while (GetListSize(listeWait) > 0)
		{
			// test sol
			int min = MinResultCase(enemy->enemyEntity.type);
			sfVector2u* temp = GetElement(listeWait, min)->value;
			caseGet = *temp;
			caseRecherche = (sfIntRect){ caseGet.x, caseGet.y + 1,bounsEnemy.width,bounsEnemy.height };
			if (TestJump(caseRecherche))//si sur sol
			{
				//test gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left + 1, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = DOWN_RIGHT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					sfBool temp = sfTrue;
					int compt = 1;
					int compt2 = 1;
					sfBool flag = sfTrue;
					while (temp && compt2 <= JUMP_FORCE && flag)
					{
						while (temp && compt <= JUMP_FORCE - compt2 + 1 && flag)
						{
							if (TestJump((sfIntRect) { caseRecherche.left - compt2 + 1, caseRecherche.top + compt, caseRecherche.width, caseRecherche.height }))
							{
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
									tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP_RIGHT;
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 1;
								AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
								temp = sfFalse;
							}
							compt++;
						}
						compt = 1;
						compt2++;
						if (TestColision((sfIntRect) { caseRecherche.left - compt2 + 1, caseRecherche.top + compt2 - 1, caseRecherche.width, caseRecherche.height }) == 2)
						{
							flag = sfFalse;
						}
					}
				}

				//test Droite
				caseRecherche = (sfIntRect){ caseGet.x + 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = LEFT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut droite
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width - 1, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].direction == EMPTY_DIRECTION)

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].direction = DOWN_LEFT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left + 1, caseRecherche.top });
				}

				//test bas droite
				caseRecherche = (sfIntRect){ caseGet.x + 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					sfBool temp = sfTrue;
					int compt = 1;
					int compt2 = 1;
					sfBool flag = sfTrue;
					while (temp && compt2 <= JUMP_FORCE && flag)
					{
						while (temp && compt <= JUMP_FORCE - compt2 + 1 && flag)
						{
							if (TestJump((sfIntRect) { caseRecherche.left + compt2 - 1, caseRecherche.top + compt, caseRecherche.width, caseRecherche.height }))
							{
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
									tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP_LEFT;
								tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 1;
								AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
								temp = sfFalse;
							}
							compt++;
						}
						compt = 1;
						compt2++;
						if (TestColision((sfIntRect) { caseRecherche.left + compt2 - 1, caseRecherche.top + compt2 - 1, caseRecherche.width, caseRecherche.height }) == 2)
						{
							flag = sfFalse;
						}
					}
				}

				//test haut
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top - 1, caseRecherche.width, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = DOWN;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Bas
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y + 1 ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) == 1 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					sfBool temp = sfTrue;
					char compt = 1;
					while (temp && compt <= JUMP_FORCE)
					{
						if (TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + compt, caseRecherche.width, caseRecherche.height }))
						{
							tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
								tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
							tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP;
							AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
							temp = sfFalse;
						}
						compt++;
					}
				}
			}
			else // si pas su sol
			{

				//test gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })
					)

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left + 1, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					(tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = DOWN_RIGHT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].direction == UP_RIGHT &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP_RIGHT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce + 1;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Droite
				caseRecherche = (sfIntRect){ caseGet.x + 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })
					)
				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = LEFT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}
				//test haut droite
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width - 1, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].direction == EMPTY_DIRECTION &&
					(tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))

				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left + 1].direction = DOWN_LEFT;
					AjoutListWait((sfVector2u) { caseRecherche.left + 1, caseRecherche.top });
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top + 1][caseRecherche.left].jumpForce = 0;
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x + 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].direction == UP_LEFT &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 2;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP_LEFT;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce + 1;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width,bounsEnemy.height };
				if (!TestColision(caseRecherche) &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					(tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))
				{
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
						tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = DOWN;
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Bas
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y + 1 ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].direction != DOWN_LEFT &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].direction != DOWN_RIGHT &&
					tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].direction != DOWN)
				{
					sfBool temp = sfTrue;
					char compt = tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce + 1;
					while (temp && compt <= JUMP_FORCE)
					{
						if (TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + compt - tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].jumpForce, caseRecherche.width, caseRecherche.height }))
						{
							tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].compteur =
								tableau.grid[enemy->enemyEntity.type][caseGet.y][caseGet.x].compteur + 1;
							tableau.grid[enemy->enemyEntity.type][caseRecherche.top][caseRecherche.left].direction = UP;
							AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
							temp = sfFalse;
						}
						compt++;
					}
				}
			}
			RetirerListWait(min);
		}
		if (DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < (int) { enemy->enemyEntity.region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { enemy->enemyEntity.region.width / TILE_SIZE }; x++)
				{
					if (!tableau.grid[enemy->enemyEntity.type][y][x].direction)
					{
						if (tableau.collision[y][x])
						{
							printf("X");
						}
						else
						{
							printf("%d", tableau.grid[enemy->enemyEntity.type][y][x].direction);
						}
					}
					else
					{
						printf("%d", tableau.grid[enemy->enemyEntity.type][y][x].direction);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
			for (int y = 0; y < (int) { enemy->enemyEntity.region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { enemy->enemyEntity.region.width / TILE_SIZE }; x++)
				{
					if (!tableau.grid[enemy->enemyEntity.type][y][x].direction)
					{
						if (tableau.collision[y][x])
						{
							printf(" X/");
						}
						else
						{
							printf(" M/");
						}
					}
					else
					{
						printf("%2d/", tableau.grid[enemy->enemyEntity.type][y][x].compteur);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
		}
	}

	ActionDemander actionDemander = { 0 };

	switch (tableau.grid[enemy->enemyEntity.type][bounsEnemy.top][bounsEnemy.left].direction)
	{
	case EMPTY_DIRECTION:
		break;
	case DOWN_LEFT:
		actionDemander.gauche = sfTrue;
		break;
	case DOWN:
		break;
	case DOWN_RIGHT:
		actionDemander.droite = sfTrue;
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
		actionDemander.gauche = sfTrue;
		actionDemander.Saut = sfTrue;
		break;
	case UP:
		actionDemander.Saut = sfTrue;
		break;
	case UP_RIGHT:
		actionDemander.droite = sfTrue;
		actionDemander.Saut = sfTrue;
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

int TestColision(sfIntRect _intRect)
{
	int temp = 0;
	for (int i = 0; i < abs(_intRect.height); i++)
	{
		for (int t = 0; t < _intRect.width; t++)
		{
			if (tableau.collision[_intRect.top - i][_intRect.left + t] == 1 && temp == 0)
			{
				temp = 1;
			}
			if (tableau.collision[_intRect.top - i][_intRect.left + t] == 2)
			{
				temp = 2;
			}

		}
	}
	return temp;
}

int TestJump(sfIntRect _intRect)
{
	int temp = 0;

	for (int t = 0; t < _intRect.width; t++)
	{
		if (tableau.collision[_intRect.top][_intRect.left + t] == 1 && temp == 0)
		{
			temp = 1;
		}
		if (tableau.collision[_intRect.top][_intRect.left + t] == 2)
		{
			temp = 2;
		}

	}
	return temp;
}

sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal)
{
	_positionReal.x = _positionReal.x / mapData->caseSize.x;
	_positionReal.y = _positionReal.y / mapData->caseSize.y;
	sfVector2u newposition = { (unsigned)_positionReal.x, (unsigned)_positionReal.y };
	return newposition;
}

sfVector2f GetPositionEnemy(int _index)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;
	return sfSprite_getPosition(enemy->sprite);
}

sfFloatRect GetBounsEnemy(int _index)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;
	return sfSprite_getGlobalBounds(enemy->sprite);
}

int GetEnemyCount()
{
	return GetListSize(listEnemy);
}

void SetPositionEnemy(sfVector2f _position, int _index)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;
	sfSprite_setPosition(enemy->sprite, _position);
}

void ResetEnemy(void)
{
	for (unsigned i = 0; i < GetListSize(listEnemy); i++)
	{
		Enemy* enemy = GetElement(listEnemy, i)->value;
		DestroyVisualEntity(enemy->sprite);
	}
	RemoveList(listEnemy);

	RemoveList(listeWait);
	sfTexture_destroy(texture);
	DestroyVisualEntity(sprite);

}

int GetEnemyZone(void)
{
	return enemyZone;
}

void EffectGelEnemy(Enemy* _enemy, int _puissance, float _time)
{
	_enemy->enemyEntity.powerGel = _puissance;
	_enemy->enemyEntity.timerGel = _time;
}

int GetNearestEnemy(List* _listeIgnore, sfVector2f _position)
{
	int index = -1;
	sfBool test = 1;
	for (unsigned i = 0; i < GetListSize(listEnemy); i++)
	{
		if (index >= 0)
		{
			if (NORM_POW2(GetPositionEnemy(i), _position) < NORM_POW2(GetPositionEnemy(index), _position))
			{
				for (unsigned r = 0; r < GetListSize(_listeIgnore); r++)
				{
					int* temp = GetElement(listEnemy, r)->value;
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

sfBool PlayerVisibility(int _index)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;

	sfFloatRect bounsEnemy = GetBounsEnemy(_index);

	sfFloatRect bounsPlayer = GetPlayerRect();

	sfVector2f pas = { ((bounsPlayer.left + (bounsPlayer.width / 2)) - (bounsEnemy.left + (bounsEnemy.width / 2))) / 50,((bounsPlayer.top + (bounsPlayer.height / 2)) - (bounsEnemy.top + (bounsEnemy.height / 2))) / 50 };

	for (int i = 0; i < 50; i++)
	{
		if (Colision((sfFloatRect) { bounsEnemy.left + (pas.x * i), bounsEnemy.top + (pas.y * i), 10, 10 }, AXIS_BOTH).x || Colision((sfFloatRect) { bounsEnemy.left + (pas.x * i), bounsEnemy.top + (pas.y * i), 10, 10 }, AXIS_BOTH).y)
		{
			return 0;
		}
	}
	return 1;
}

void shootPlayer(int _index)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;

	sfFloatRect bounsEnemy = GetBounsEnemy(_index);
	sfVector2f posGun = (sfVector2f){ bounsEnemy.left + (bounsEnemy.width / 2),bounsEnemy.top + (bounsEnemy.height / 2) };

	sfFloatRect bounsPlayer = GetPlayerRect();
	sfVector2f posTarget = (sfVector2f){ bounsPlayer.left + (bounsPlayer.width / 2),bounsPlayer.top + (bounsPlayer.height / 2) };

	ShooterType type = { 0 };
	type.isAlly = sfFalse;
	type.bulletType = LIGHT;

	AddBullet(posGun, posTarget, type);
}

void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region)
{
	_region.left -= TILE_SIZE;
	_region.top -= TILE_SIZE;
	_region.width += TILE_SIZE * 2;
	_region.height += TILE_SIZE * 2;

	Enemy* enemy = Calloc(1, sizeof(Enemy));
	Element* element = CreateElement(enemy);
	element->value = enemy;

	if (_type == ALEATORY)
	{
		_type = rand() % ALEATORY;
	}
	CreateEnemy(enemy, _type);

	enemy->enemyEntity.region = _region;

	element->next = listEnemy->first;
	listEnemy->first = element;
	SetPositionEnemy(_position, 0);
}

sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat, AttackType _attaque)
{
	Enemy* enemy = GetElement(listEnemy, _index)->value;

	sfImage* colideur = sfTexture_copyToImage(sfSprite_getTexture(enemy->sprite));
	sfColor pixelColor = sfImage_getPixel(colideur, (int)_touch.x, (int)_touch.y);
	sfImage_destroy(colideur);

	sfBool isTouch = sfFalse;
	if (pixelColor.a == 255)
	{
		isTouch = sfTrue;
		switch (_attaque)
		{
		case NOATTACK:
			break;
		case FREEZE:
			EffectGelEnemy(enemy, 2, 5);
			break;
		case LIGHT:
			enemy->enemyEntity.enemydata.life -= _degat / (enemy->enemyEntity.enemydata.armure + 1);
			if (enemy->enemyEntity.enemydata.life < 0)
			{
				DestroyVisualEntity(enemy->sprite);
				Free(enemy);
				RemoveElement(listEnemy, _index);
			}
			break;
		case MEDIUM:
			if (enemy->enemyEntity.enemydata.armure == 2)
			{
				enemy->enemyEntity.enemydata.life -= _degat / 2;
			}
			else
			{
				enemy->enemyEntity.enemydata.life -= _degat;
			}

			if (enemy->enemyEntity.enemydata.life < 0)
			{
				DestroyVisualEntity(enemy->sprite);
				Free(enemy);
				RemoveElement(listEnemy, _index);
			}
			break;
		case HEAVY:
			enemy->enemyEntity.enemydata.life -= _degat;
			if (enemy->enemyEntity.enemydata.life < 0)
			{
				DestroyVisualEntity(enemy->sprite);
				Free(enemy);
				RemoveElement(listEnemy, _index);
			}
			break;
		default:
			break;
		}

	}
	return isTouch;
}

sfBool HitEnemy(float _degat, sfFloatRect _hitbox, AttackType _attaque)
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
			return HitEnemyI(i, touch, _degat, _attaque);
		}
	}
	return sfFalse;
}