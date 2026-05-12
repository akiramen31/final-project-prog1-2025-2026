#include "Enemy.h"
#include "Map.h"
#include "Projectiles.h"
#include "Player.h"

void UpdateColisionEnemy(unsigned _index);
void UpdateEnemyI(float _dt, unsigned _index);
void CalculMoveEnemy(float _dt, unsigned _index);
ActionDemander AStar2(EnemyEntity* _enemy, sfFloatRect _cible);
ActionDemander AStar3(EnemyEntity* _enemy, sfFloatRect _cible);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);
sfBool PlayerVisibility(unsigned _index);
void shootPlayer(unsigned _index);
sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat, AttackType _attaque);

Enemy enemy;

void LoadEnemy(void)
{
	enemy = (Enemy){ 0 };

	enemy.listeWait = CreateList();
	enemy.entity = Calloc(1, sizeof(EnemyEntity));
	enemy.dataByType[DRONE_SMALL] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), LIGHT_ARMOR, 5.f, 10.f, 1.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f };
	enemy.dataByType[DRONE_SMALL_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), LIGHT_ARMOR, 5.f, 10.f, 2.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f };
	enemy.dataByType[DRONE_SMALL_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), LIGHT_ARMOR, 5.f, 10.f, 3.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f };
	enemy.dataByType[GROUND_HEAVY] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), HEAVY_ARMOR, 1.f, 100.f, 3.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f };
	enemy.dataByType[GROUND_HEAVY_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), HEAVY_ARMOR, 1.f, 100.f, 6.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f };
	enemy.dataByType[GROUND_HEAVY_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), HEAVY_ARMOR, 1.f, 100.f, 9.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f };
	enemy.dataByType[SOLDIER_SMALL] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), MEDIUM, 2.f, 50.f, 5.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f };
	enemy.dataByType[SOLDIER_SMALL_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), MEDIUM, 2.f, 50.f, 10.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f };
	enemy.dataByType[SOLDIER_SMALL_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), MEDIUM, 2.f, 50.f, 15.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f };
}

void UpdateEnemy(float _dt)
{
	enemy.activeCount = 0;
	sfVector2f playerPos = GetPlayerPosition();
	for (unsigned i = 0; i < enemy.count; i++)
	{
		if (enemy.entity[i].region.left + TILE_SIZE <= playerPos.x && enemy.entity[i].region.left + TILE_SIZE + enemy.entity[i].region.width - TILE_SIZE * 2 >= playerPos.x && enemy.entity[i].region.top + TILE_SIZE <= playerPos.y && enemy.entity[i].region.top + enemy.entity[i].region.height - TILE_SIZE >= playerPos.y)
		{
			UpdateEnemyI(_dt, i);
			enemy.activeCount++;
		}
	}

	for (char i = 0; i < ALEATORY; i++)
	{
		enemy.tableau.new[i] = sfTrue;
	}
}

void UpdateEnemyI(float _dt, unsigned _i)
{
	if (enemy.entity[_i].freezeTimer > 0)
	{
		enemy.entity[_i].freezeTimer -= _dt;
		_dt /= enemy.entity[_i].freezePower;
		if (enemy.entity[_i].freezeTimer < 0)
		{
			enemy.entity[_i].freezeTimer = 0;
		}
	}

	enemy.entity[_i].aStarTimer += _dt;
	enemy.entity[_i].shootTimer += _dt;
	if (enemy.entity[_i].energy < enemy.dataByType[enemy.entity[_i].type].energyMax)
	{
		enemy.entity[_i].energy += enemy.dataByType[enemy.entity[_i].type].energyRegen * _dt;
	}
	else
	{
		enemy.entity[_i].energy = enemy.dataByType[enemy.entity[_i].type].energyMax;
	}
	switch (enemy.entity[_i].type)
	{
	case DRONE_SMALL:
		if (PlayerVisibility(_i))
		{
			if (enemy.entity[_i].shootTimer >= enemy.dataByType[enemy.entity[_i].type].shootCooldown)
			{
				enemy.entity[_i].shootTimer = 0;
				shootPlayer(_i);
			}
			enemy.entity[_i].action = (ActionDemander){ 0 };
		}
		else
		{
			if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
			{
				enemy.entity[_i].action = AStar3(&enemy.entity[_i], GetPlayerRect());
				enemy.entity[_i].aStarTimer = 0;
			}
		}
		break;
	case GROUND_HEAVY:
		if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
		{
			enemy.entity[_i].action = AStar2(&enemy.entity[_i], GetPlayerRect());
			enemy.entity[_i].aStarTimer = 0;
		}
		break;
	case SOLDIER_SMALL:
		if (PlayerVisibility(_i))
		{
			if (enemy.entity[_i].shootTimer >= enemy.dataByType[enemy.entity[_i].type].shootCooldown)
			{
				enemy.entity[_i].shootTimer = 0;
				shootPlayer(_i);
			}
			enemy.entity[_i].action = (ActionDemander){ 0 };
		}
		else
		{
			if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
			{
				enemy.entity[_i].action = AStar2(&enemy.entity[_i], GetPlayerRect());
				enemy.entity[_i].aStarTimer = 0;
			}
		}
		break;
	case ALEATORY:
		break;
	default:
		break;
	}
	
	CalculMoveEnemy(_dt, _i);
	UpdateColisionEnemy(_i);
}

void UpdateColisionEnemy(unsigned _i)
{
	sfFloatRect enemyRect = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
	sfFloatRect realRegion = { enemy.entity[_i].region.left + TILE_SIZE ,enemy.entity[_i].region.top + TILE_SIZE , enemy.entity[_i].region.width - TILE_SIZE * 2, enemy.entity[_i].region.height - TILE_SIZE * 2 };

	sfVector2f collision = Colision(sfSprite_getGlobalBounds(enemy.entity[_i].sprite), AXIS_BOTH);
	collision.y += CollisionPassThrough(sfSprite_getGlobalBounds(enemy.entity[_i].sprite)).y;
	sfSprite_move(enemy.entity[_i].sprite, collision);
	if (collision.x)
	{
		enemy.entity[_i].velocity.x = 0;
	}
	if (collision.y)
	{
		enemy.entity[_i].velocity.y = 0;
	}

	if (realRegion.left > enemyRect.left)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { realRegion.left - enemyRect.left, 0 });
	}
	else if (realRegion.left + realRegion.width < (enemyRect.left + enemyRect.width))
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { (realRegion.left + realRegion.width) - (enemyRect.left + enemyRect.width), 0 });
	}
	if (realRegion.top > enemyRect.top)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { 0, realRegion.top - enemyRect.top });
	}
	else if (realRegion.top + realRegion.height < enemyRect.top + enemyRect.height)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { 0, (realRegion.top + realRegion.height) - (enemyRect.top + enemyRect.height) });
	}
}

void CalculMoveEnemy(float _dt, unsigned _i)
{
	// 1 = droite / -1 = gauche
	char droitOuGauche = enemy.entity[_i].action.droite - enemy.entity[_i].action.gauche;
	if (droitOuGauche)
	{
		enemy.entity[_i].velocity.x += droitOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax * _dt;
		if (droitOuGauche * enemy.entity[_i].velocity.x > droitOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax)
		{
			enemy.entity[_i].velocity.x = droitOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax;
		}
	}
	else
	{
		enemy.entity[_i].velocity.x = 0;
	}

	if (enemy.entity[_i].action.Saut)
	{
		sfFloatRect enemyRect = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
		enemyRect.top += 1;
		sfVector2f collision = Colision(enemyRect, AXIS_BOTH);
		collision.y += CollisionPassThrough(enemyRect).y;
		if (collision.y > -2 && collision.y < 0)
		{
			enemy.entity[_i].velocity.y = -enemy.dataByType[enemy.entity[_i].type].jumForce;
		}
		else if (0 && enemy.entity[_i].velocity.y >= 0 && enemy.dataByType[enemy.entity[_i].type].trust && enemy.dataByType[enemy.entity[_i].type].consomation * _dt < enemy.entity[_i].energy)
		{
			enemy.entity[_i].velocity.y -= enemy.dataByType[enemy.entity[_i].type].trust;
			enemy.entity[_i].energy -= enemy.dataByType[enemy.entity[_i].type].consomation * _dt;
		}
	}
	enemy.entity[_i].velocity.y += G * _dt;

	if (enemy.entity[_i].velocity.y > enemy.dataByType[enemy.entity[_i].type].jumForce)
	{
		enemy.entity[_i].velocity.y = enemy.dataByType[enemy.entity[_i].type].jumForce;
	}

	sfSprite_move(enemy.entity[_i].sprite, enemy.entity[_i].velocity);
}

sfBool PlayerVisibility(unsigned _i)
{
	sfFloatRect bounsEnemy = GetBounsEnemy(_i);

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

void shootPlayer(unsigned _i)
{
	sfFloatRect bounsEnemy = GetBounsEnemy(_i);
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
	if (_type == ALEATORY)
	{
		_type = rand() % ALEATORY;
	}

	enemy.entity = Realloc(enemy.entity, (size_t)(enemy.count + 1) * sizeof(EnemyEntity));
	enemy.entity[enemy.count] = (EnemyEntity){ CreateSprite(enemy.dataByType[_type].texture, _position, 1, 1), _type, (sfVector2f) { 0 }, (ActionDemander) { 0 },_region, enemy.dataByType[_type].lifeMax, 0.f, 0.f, 0.f, 0.f, 0.f };
	SetSpriteOriginFoot(enemy.entity[enemy.count].sprite);
	enemy.count++;
}

sfBool HitEnemyI(unsigned _i, sfVector2f _touch, float _degat, AttackType _type)
{
	sfImage* colideur = sfTexture_copyToImage(sfSprite_getTexture(enemy.entity[_i].sprite));
	sfColor pixelColor = sfImage_getPixel(colideur, (int)_touch.x, (int)_touch.y);
	sfImage_destroy(colideur);

	if (pixelColor.a == 255)
	{
		if (_type == FREEZE)
		{
			enemy.entity[_i].freezePower = 2;
			enemy.entity[_i].freezeTimer = 5;
		}

		enemy.entity[_i].life -= _degat / ((enemy.dataByType[enemy.entity[_i].type].armure + 1) * (_type - FREEZE));
		if (enemy.entity[_i].life < 0)
		{
			DestroyVisualEntity(enemy.entity[_i].sprite);
			enemy.count--;
			enemy.entity[_i] = enemy.entity[enemy.count];
			enemy.entity = Realloc(enemy.entity, enemy.count * sizeof(EnemyEntity));
		}
		return sfTrue;
	}
	return sfFalse;
}

sfBool HitEnemy(float _degat, sfFloatRect _hitbox, AttackType _type)
{
	sfFloatRect hitboxEnemy = { 0 };
	sfFloatRect hitboxTir = { 0 };
	sfVector2f touch = { 0 };
	for (int i = 0; i < enemy.count; i++)
	{
		hitboxEnemy = sfSprite_getGlobalBounds(enemy.entity[i].sprite);
		if (sfFloatRect_intersects(&_hitbox, &hitboxEnemy, &hitboxTir))
		{
			touch = (sfVector2f){ hitboxTir.left + hitboxTir.width / 2 - hitboxEnemy.left, hitboxTir.top + hitboxTir.height / 2 - hitboxEnemy.top };
			return HitEnemyI(i, touch, _degat, _type);
		}
	}
	return sfFalse;
}

sfVector2f GetPositionEnemy(unsigned _i)
{
	return sfSprite_getPosition(enemy.entity[_i].sprite);
}

sfFloatRect GetBounsEnemy(unsigned _i)
{
	return sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
}

int GetEnemyCount(void)
{
	return enemy.count;
}

void SetPositionEnemy(sfVector2f _position, unsigned _i)
{
	sfSprite_setPosition(enemy.entity[_i].sprite, _position);
}

int GetEnemyZone(void)
{
	return enemy.activeCount;
}

int GetNearestEnemy(List* _listeIgnore, sfVector2f _position)
{
	int proxi = 0;
	sfVector2f positionProxi = sfSprite_getPosition(enemy.entity[proxi].sprite);
	for (unsigned i = 0; i < enemy.count; i++)
	{
		sfVector2f positionI = sfSprite_getPosition(enemy.entity[i].sprite);
		if (NORM_POW2(positionI, _position) < NORM_POW2(positionProxi, _position))
		{
			positionProxi = positionI;
			proxi = i;
		}
	}
	return proxi;
}

#pragma region AStar

int MinResultCase(int _type);
void AjoutListWait(sfVector2u _caseAjout);
void RetirerListWait(int _index);
int TestColision(sfIntRect _intRect);
int TestJump(sfIntRect _intRect);
sfIntRect FloatRectIntoIntRectByCase(sfFloatRect _floatRect);

ActionDemander AStar2(EnemyEntity* _enemy, sfFloatRect _cible)
{
	sfVector2u gridSize = { _enemy->region.width / TILE_SIZE , _enemy->region.height / TILE_SIZE };
	if (_enemy->region.top != enemy.tableau.region.top || _enemy->region.left != enemy.tableau.region.left)
	{
		// liberer lancienne GRID
		enemy.tableau.region = _enemy->region;

		for (int i = 0; i < ALEATORY; i++)
		{
			FreeGrid(enemy.tableau.grid[_enemy->type]);
			enemy.tableau.grid[i] = CreateGrid(gridSize.x, gridSize.y, sizeof(Case2));
		}
		FreeGrid(enemy.tableau.collision);
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
					sfVector2f reaction = Colision((sfFloatRect) { _enemy->region.left + x * TILE_SIZE, _enemy->region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
					if (reaction.x || reaction.y)
					{
						grid[y][x] = 2;
					}
					else if (CollisionPassThrough((sfFloatRect) { _enemy->region.left + x * TILE_SIZE, _enemy->region.top + y * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
					{
						grid[y][x] = 1;
					}
				}
			}
		}
		enemy.tableau.collision = grid;
		if (0 && DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < gridSize.y; y++)
			{
				for (int x = 0; x < gridSize.x; x++)
				{
					printf("%d", enemy.tableau.collision[y][x]);
				}
				printf("\n");
			}
			printf("\n\n\n");
		}
	};

	sfFloatRect bouns = sfSprite_getGlobalBounds(_enemy->sprite);
	bouns.left -= _enemy->region.left;
	bouns.top -= _enemy->region.top + 1;
	sfIntRect bounsEnemy = FloatRectIntoIntRectByCase(bouns);
	_cible.left -= _enemy->region.left;
	_cible.top -= _enemy->region.top + 1;
	sfIntRect bounsCible = FloatRectIntoIntRectByCase(_cible);
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

	if (enemy.tableau.new[_enemy->type])
	{
		for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
		{
			for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
			{
				enemy.tableau.grid[_enemy->type][y][x].direction = EMPTY_DIRECTION;
				enemy.tableau.grid[_enemy->type][y][x].compteur = 0;
				enemy.tableau.grid[_enemy->type][y][x].jumpForce = 0;
			}
		}

		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].compteur = 0;
		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].jumpForce = 0;
		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].direction = NO_DIRECTION;

		sfVector2u caseGet = { bounsCible.left, bounsCible.top };
		sfIntRect caseRecherche = { 0 };

		enemy.tableau.new[_enemy->type] = sfFalse;

		AjoutListWait((sfVector2u) { bounsCible.left, bounsCible.top });

		while (enemy.count > 0)
		{
			// test sol
			int min = MinResultCase(_enemy->type);
			if (!enemy.listeWait->first)
			{
				break;
			}
			sfVector2u* temp = GetElement(enemy.listeWait, min)->value;
			caseGet = *temp;
			caseRecherche = (sfIntRect){ caseGet.x, caseGet.y + 1,bounsEnemy.width,bounsEnemy.height };
			if (TestJump(caseRecherche))//si sur sol
			{
				//test gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left + 1, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN_RIGHT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
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
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
									enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_RIGHT;
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 1;
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
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = LEFT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut droite
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width - 1, caseRecherche.height }) &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction == EMPTY_DIRECTION)

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction = DOWN_LEFT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left + 1, caseRecherche.top });
				}

				//test bas droite
				caseRecherche = (sfIntRect){ caseGet.x + 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
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
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
									enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_LEFT;
								enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 1;
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
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Bas
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y + 1 ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) == 1 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					sfBool temp = sfTrue;
					char compt = 1;
					while (temp && compt <= JUMP_FORCE)
					{
						if (TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + compt, caseRecherche.width, caseRecherche.height }))
						{
							enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
								enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
							enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP;
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
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })
					)

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left + 1, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					(enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN_RIGHT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x - 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].direction == UP_RIGHT &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_RIGHT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce + 1;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Droite
				caseRecherche = (sfIntRect){ caseGet.x + 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })
					)
				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = LEFT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}
				//test haut droite
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width - 1, caseRecherche.height }) &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction == EMPTY_DIRECTION &&
					(enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))

				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction = DOWN_LEFT;
					AjoutListWait((sfVector2u) { caseRecherche.left + 1, caseRecherche.top });
					enemy.tableau.grid[_enemy->type][caseRecherche.top + 1][caseRecherche.left].jumpForce = 0;
				}

				//test bas gauche
				caseRecherche = (sfIntRect){ caseGet.x + 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].direction == UP_LEFT &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_LEFT;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce + 1;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test haut
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width,bounsEnemy.height };
				if (!TestColision(caseRecherche) &&
					!TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height }) &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					(enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce == 0 ||
						TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + 1, caseRecherche.width, caseRecherche.height })))
				{
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
						enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN;
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].jumpForce = 0;
					AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
				}

				//test Bas
				caseRecherche = (sfIntRect){ caseGet.x , caseGet.y + 1 ,bounsEnemy.width,bounsEnemy.height };
				if (TestColision(caseRecherche) != 2 &&
					enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].direction != DOWN_LEFT &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].direction != DOWN_RIGHT &&
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].direction != DOWN)
				{
					sfBool temp = sfTrue;
					char compt = enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce + 1;
					while (temp && compt <= JUMP_FORCE)
					{
						if (TestJump((sfIntRect) { caseRecherche.left, caseRecherche.top + compt - enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].jumpForce, caseRecherche.width, caseRecherche.height }))
						{
							enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
								enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
							enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP;
							AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
							temp = sfFalse;
						}
						compt++;
					}
				}
			}
			RetirerListWait(min);
		}
		if (1 && DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
				{
					if (!enemy.tableau.grid[_enemy->type][y][x].direction)
					{
						if (enemy.tableau.collision[y][x])
						{
							printf("X");
						}
						else
						{
							printf("%d", enemy.tableau.grid[_enemy->type][y][x].direction);
						}
					}
					else
					{
						printf("%d", enemy.tableau.grid[_enemy->type][y][x].direction);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
			for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
				{
					if (!enemy.tableau.grid[_enemy->type][y][x].direction)
					{
						if (enemy.tableau.collision[y][x])
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
						printf("%2d/", enemy.tableau.grid[_enemy->type][y][x].compteur);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
		}
	}

	ActionDemander actionDemander = { 0 };

	switch (enemy.tableau.grid[_enemy->type][bounsEnemy.top][bounsEnemy.left].direction)
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

ActionDemander AStar3(EnemyEntity* _enemy, sfFloatRect _cible)
{
	sfVector2u gridSize = { _enemy->region.width / TILE_SIZE , _enemy->region.height / TILE_SIZE };
	if (_enemy->region.top != enemy.tableau.region.top || _enemy->region.left != enemy.tableau.region.left)
	{
		// liberer lancienne GRID
		enemy.tableau.region = _enemy->region;

		for (int i = 0; i < ALEATORY; i++)
		{
			FreeGrid(enemy.tableau.grid[_enemy->type]);
			enemy.tableau.grid[i] = CreateGrid(gridSize.x, gridSize.y, sizeof(Case2));
		}
		FreeGrid(enemy.tableau.collision);
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
					sfVector2f reaction = Colision((sfFloatRect) { _enemy->region.left + x * TILE_SIZE, _enemy->region.top + y * TILE_SIZE, TILE_SIZE, TILE_SIZE }, AXIS_BOTH);
					if (reaction.x || reaction.y)
					{
						grid[y][x] = 2;
					}
					else if (CollisionPassThrough((sfFloatRect) { _enemy->region.left + x * TILE_SIZE, _enemy->region.top + y * TILE_SIZE - 13, TILE_SIZE, TILE_SIZE }).y)
					{
						grid[y][x] = 1;
					}
				}
			}
		}
		enemy.tableau.collision = grid;
		if (0 && DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < gridSize.y; y++)
			{
				for (int x = 0; x < gridSize.x; x++)
				{
					printf("%d", enemy.tableau.collision[y][x]);
				}
				printf("\n");
			}
			printf("\n\n\n");
		}
	};

	sfFloatRect bouns = sfSprite_getGlobalBounds(_enemy->sprite);
	bouns.left -= _enemy->region.left;
	bouns.top -= _enemy->region.top + 1;
	sfIntRect bounsEnemy = FloatRectIntoIntRectByCase(bouns);
	_cible.left -= _enemy->region.left;
	_cible.top -= _enemy->region.top + 1;
	sfIntRect bounsCible = FloatRectIntoIntRectByCase(_cible);
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

	if (enemy.tableau.new[_enemy->type])
	{
		for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
		{
			for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
			{
				enemy.tableau.grid[_enemy->type][y][x].direction = EMPTY_DIRECTION;
				enemy.tableau.grid[_enemy->type][y][x].compteur = 0;
				enemy.tableau.grid[_enemy->type][y][x].jumpForce = 0;
			}
		}

		sfVector2u caseGet = { bounsCible.left, bounsCible.top };
		sfIntRect caseRecherche = { 0 };

		caseRecherche = (sfIntRect){ caseGet.x, caseGet.y - 1,bounsEnemy.width,bounsEnemy.height };
		if (!TestColision(caseRecherche))
		{
			while (!TestColision(caseRecherche))
			{
				caseRecherche.top--;
				bounsCible.top--;
			}
		}
		else
		{
			if (caseGet.x >= 1)
			{
				caseRecherche.left -= 1;
				while (!TestColision(caseRecherche))
				{
					caseRecherche.top--;
					bounsCible.top--;
				}
			}
		}
		caseGet = (sfVector2u){ bounsCible.left, bounsCible.top };

		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].compteur = 0;
		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].jumpForce = 0;
		enemy.tableau.grid[_enemy->type][bounsCible.top][bounsCible.left].direction = NO_DIRECTION;

		enemy.tableau.new[_enemy->type] = sfFalse;

		AjoutListWait((sfVector2u) { bounsCible.left, bounsCible.top });

		while (enemy.count > 0)
		{
			int min = MinResultCase(_enemy->type);
			if (!enemy.listeWait->first)
			{
				break;
			}
			sfVector2u* temp = GetElement(enemy.listeWait, min)->value;
			caseGet = *temp;
			//test gauche
			caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = RIGHT;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
			}

			//test haut gauche
			caseRecherche = (sfIntRect){ caseGet.x - 1, caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 5;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN_RIGHT;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
			}

			//test bas gauche
			caseRecherche = (sfIntRect){ caseGet.x - 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
			{

				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 3;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_RIGHT;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });

			}

			//test Droite
			caseRecherche = (sfIntRect){ caseGet.x + 1, caseGet.y ,bounsEnemy.width,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)

			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 2;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = LEFT;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
			}

			//test haut droite
			caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width + 1,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction == EMPTY_DIRECTION)

			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 5;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left + 1].direction = DOWN_LEFT;
				AjoutListWait((sfVector2u) { caseRecherche.left + 1, caseRecherche.top });
			}

			//test bas droite
			caseRecherche = (sfIntRect){ caseGet.x + 1 , caseGet.y + 1 ,bounsEnemy.width ,bounsEnemy.height - 1 };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
			{

				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 3;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP_LEFT;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });

			}

			//test haut
			caseRecherche = (sfIntRect){ caseGet.x , caseGet.y - 1 ,bounsEnemy.width,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 3;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = DOWN;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
			}

			//test Bas
			caseRecherche = (sfIntRect){ caseGet.x , caseGet.y + 1 ,bounsEnemy.width,bounsEnemy.height };
			if (!TestColision(caseRecherche) &&
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction == EMPTY_DIRECTION)
			{
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].compteur =
					enemy.tableau.grid[_enemy->type][caseGet.y][caseGet.x].compteur + 1;
				enemy.tableau.grid[_enemy->type][caseRecherche.top][caseRecherche.left].direction = UP;
				AjoutListWait((sfVector2u) { caseRecherche.left, caseRecherche.top });
			}
			RetirerListWait(min);
		}
		if (1 && DEBUG_MODE_A_STAR)
		{
			for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
				{
					if (!enemy.tableau.grid[_enemy->type][y][x].direction)
					{
						if (enemy.tableau.collision[y][x])
						{
							printf("X");
						}
						else
						{
							printf("%d", enemy.tableau.grid[_enemy->type][y][x].direction);
						}
					}
					else
					{
						printf("%d", enemy.tableau.grid[_enemy->type][y][x].direction);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
			for (int y = 0; y < (int) { _enemy->region.height / TILE_SIZE }; y++)
			{
				for (int x = 0; x < (int) { _enemy->region.width / TILE_SIZE }; x++)
				{
					if (!enemy.tableau.grid[_enemy->type][y][x].direction)
					{
						if (enemy.tableau.collision[y][x])
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
						printf("%2d/", enemy.tableau.grid[_enemy->type][y][x].compteur);
					}
				}
				printf("\n");
			}
			printf("\n\n\n");
		}
	}

	ActionDemander actionDemander = { 0 };

	switch (enemy.tableau.grid[_enemy->type][bounsEnemy.top][bounsEnemy.left].direction)
	{
	case EMPTY_DIRECTION:
		break;
	case DOWN_LEFT:
		actionDemander.gauche = sfTrue;
		actionDemander.bas = sfTrue;
		break;
	case DOWN:
		actionDemander.bas = sfTrue;
		break;
	case DOWN_RIGHT:
		actionDemander.droite = sfTrue;
		actionDemander.bas = sfTrue;
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

int MinResultCase(int _type) // recherche du plus petit resultat dans la liste chainé listeWait
{
	int min = 0;
	unsigned sizeListeWait = GetListSize(enemy.listeWait);
	if (sizeListeWait > 1)
	{
		sfVector2u* caseGet = 0;
		sfVector2u* caseMin = GetElement(enemy.listeWait, min)->value;
		for (unsigned i = 1; i < sizeListeWait; i++)
		{
			sfVector2u* caseGet = GetElement(enemy.listeWait, i)->value;
			if (enemy.tableau.grid[_type][caseGet->y][caseGet->x].compteur < enemy.tableau.grid[_type][caseMin->y][caseMin->x].compteur)
			{
				caseMin = caseGet;
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
	InsertElement(enemy.listeWait, elementTemp, GetListSize(enemy.listeWait));
}

void RetirerListWait(int _index)
{
	Element* elementTemp = GetElement(enemy.listeWait, _index)->value;
	Free(elementTemp);
	RemoveElement(enemy.listeWait, _index);
}

int TestColision(sfIntRect _intRect)
{
	int temp = 0;
	for (int i = 0; i < abs(_intRect.height); i++)
	{
		for (int t = 0; t < _intRect.width; t++)
		{
			if (enemy.tableau.collision[_intRect.top - i][_intRect.left + t] == 1 && temp == 0)
			{
				temp = 1;
			}
			if (enemy.tableau.collision[_intRect.top - i][_intRect.left + t] == 2)
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
		if (enemy.tableau.collision[_intRect.top][_intRect.left + t] == 1 && temp == 0)
		{
			temp = 1;
		}
		if (enemy.tableau.collision[_intRect.top][_intRect.left + t] == 2)
		{
			temp = 2;
		}

	}
	return temp;
}

sfIntRect FloatRectIntoIntRectByCase(sfFloatRect _floatRect)
{
	sfIntRect intRect = { 0 };
	intRect.height = (int)(_floatRect.height / TILE_SIZE) + 1;
	intRect.width = (int)(_floatRect.width / TILE_SIZE) + 1;
	intRect.left = (int)(_floatRect.left / TILE_SIZE);
	intRect.top = (int)(_floatRect.top / TILE_SIZE) + intRect.height - 1;
	intRect.height = -intRect.height;

	return intRect;
}

#pragma endregion