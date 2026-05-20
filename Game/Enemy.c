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
void DamageEnemyI(unsigned _i, float _degat, AttackType _type);

Enemy enemy;

void LoadEnemy(void)
{
	enemy = (Enemy){ 0 };

	enemy.listeWait = CreateList();
	enemy.entity = Calloc(1, sizeof(EnemyEntity));
	enemy.dataByType[DRONE_SMALL] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), LIGHT_ARMOR, 5.f, 10.f, 1.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f, 50 };
	enemy.dataByType[DRONE_SMALL_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_medium.png"), LIGHT_ARMOR, 5.f, 10.f, 2.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f,100 };
	enemy.dataByType[DRONE_SMALL_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_large.png"), LIGHT_ARMOR, 5.f, 10.f, 3.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.5f,200 };
	enemy.dataByType[GROUND_HEAVY] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), HEAVY_ARMOR, 1.f, 100.f, 2.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f, 50 };
	enemy.dataByType[GROUND_HEAVY_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_medium.png"), HEAVY_ARMOR, 1.f, 100.f, 3.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f,100 };
	enemy.dataByType[GROUND_HEAVY_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_large.png"), HEAVY_ARMOR, 1.f, 100.f, 4.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 0.f, 200 };
	enemy.dataByType[SOLDIER_SMALL] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_small.png"), MEDIUM, 2.f, 50.f, 1.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f, 50 };
	enemy.dataByType[SOLDIER_SMALL_MEDIUM] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_medium.png"), MEDIUM, 2.f, 50.f, 2.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f, 100 };
	enemy.dataByType[SOLDIER_SMALL_LARGE] = (EnemyDataByType){ GetAsset("Assets/Sprites/spider_large.png"), MEDIUM, 2.f, 50.f, 3.f, (float)MAX_ENRGIE, 15.f, 1.f, 6 * TILE_SIZE / G / 3.5f, 1.f,200 };

#if DEV_PRINT_ERROR
	for (int i = 0; i < ALEATORY / GROUND_HEAVY; i++)
	{
		sfVector2u size = sfTexture_getSize(enemy.dataByType[i * 3].texture);
		if (size.x > 14 || size.y > 14)
		{
			printf("bad size %d\n", i * 3);
		}
		size = sfTexture_getSize(enemy.dataByType[i * 3 + 1].texture);
		if (size.x > 30 || size.y > 14)
		{
			printf("bad size %d\n", i * 3 + 1);
		}
		size = sfTexture_getSize(enemy.dataByType[i * 3 + 2].texture);
		if (size.x > 46 || size.y > 30)
		{
			printf("bad size %d\n", i * 3 + 2);
		}
	}
#endif
}

void UpdateEnemy(float _dt)
{
	enemy.activeCount = 0;
	for (unsigned i = 0; i < enemy.count; i++)
	{
		UpdateEnemyI(_dt, i);
	}

	for (char i = 0; i < ALEATORY; i++)
	{
		enemy.tableau.new[i] = sfTrue;
	}
}

void UpdateEnemyI(float _dt, unsigned _i)
{
	if (enemy.entity[_i].domageTimer > 0)
	{
		enemy.entity[_i].domageTimer -= _dt;
		sfSprite_setColor(enemy.entity[_i].sprite, sfRed);
		if (enemy.entity[_i].domageTimer < 0)
		{
			sfSprite_setColor(enemy.entity[_i].sprite, sfWhite);
			enemy.entity[_i].domageTimer = 0;
		}
	}

	if (enemy.entity[_i].freezeTimer > 0)
	{
		enemy.entity[_i].freezeTimer -= _dt;
		_dt /= enemy.entity[_i].freezePower;
		if (CompareColor(sfSprite_getColor(enemy.entity[_i].sprite), sfRed))
		{
			sfSprite_setColor(enemy.entity[_i].sprite, COLOR_ORANGE);
		}
		else
		{
			sfSprite_setColor(enemy.entity[_i].sprite, sfBlue);
		}
		if (enemy.entity[_i].freezeTimer < 0)
		{
			enemy.entity[_i].freezeTimer = 0;
			sfSprite_setColor(enemy.entity[_i].sprite, sfWhite);
		}
	}

	enemy.entity[_i].shootTimer += _dt;
	if (enemy.entity[_i].energy < enemy.dataByType[enemy.entity[_i].type].energyMax)
	{
		enemy.entity[_i].energy += enemy.dataByType[enemy.entity[_i].type].energyRegen * _dt;
	}
	else
	{
		enemy.entity[_i].energy = enemy.dataByType[enemy.entity[_i].type].energyMax;
	}

	sfVector2f playerPos = GetPlayerPosition();
	if (enemy.entity[_i].region.left + TILE_SIZE <= playerPos.x && enemy.entity[_i].region.left + TILE_SIZE + enemy.entity[_i].region.width - TILE_SIZE * 2 >= playerPos.x && enemy.entity[_i].region.top + TILE_SIZE <= playerPos.y && enemy.entity[_i].region.top + enemy.entity[_i].region.height - TILE_SIZE >= playerPos.y)
	{
		enemy.entity[_i].aStarTimer += _dt;
		if (enemy.entity[_i].type < GROUND_HEAVY)
		{
			sfFloatRect rectEnemy = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
			if (playerPos.x < rectEnemy.left + rectEnemy.width && playerPos.x > rectEnemy.left)
			{
				if (enemy.entity[_i].shootTimer >= enemy.dataByType[enemy.entity[_i].type].shootCooldown)
				{
					enemy.entity[_i].shootTimer = 0;
					SpawnGrenade(sfSprite_getPosition(enemy.entity[_i].sprite), (rectEnemy.width + rectEnemy.height) / 24.f, (rectEnemy.width + rectEnemy.height) / 24.f * 3);
				}
				enemy.entity[_i].action = (ActionDemander){ 0, 0, enemy.entity[_i].action.saut, 0 };
			}
			else if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
			{
				UpdateColisionEnemy(_i);
				enemy.entity[_i].action = AStar3(&enemy.entity[_i], GetPlayerRect());
				enemy.entity[_i].aStarTimer = 0;
			}
		}
		else if (enemy.entity[_i].type < SOLDIER_SMALL)
		{
			sfFloatRect rectPlayer = GetPlayerRect();
			sfFloatRect rectEnemy = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
			if (sfFloatRect_intersects(&rectPlayer, &rectEnemy, NULL))
			{
				DamagePlayer(1);
				enemy.entity[_i].action = (ActionDemander){ 0 };
			}
			else if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
			{
				UpdateColisionEnemy(_i);
				enemy.entity[_i].action = AStar2(&enemy.entity[_i], GetPlayerRect());
				enemy.entity[_i].aStarTimer = 0;
			}
		}
		else if (enemy.entity[_i].type < ALEATORY)
		{
			if (PlayerVisibility(_i))
			{
				if (enemy.entity[_i].shootTimer >= enemy.dataByType[enemy.entity[_i].type].shootCooldown)
				{
					enemy.entity[_i].shootTimer = 0;
					shootPlayer(_i);
				}
				enemy.entity[_i].action = (ActionDemander){ 0 };
			}
			else if (enemy.entity[_i].aStarTimer >= TIMER_ASTAR)
			{
				UpdateColisionEnemy(_i);
				enemy.entity[_i].action = AStar2(&enemy.entity[_i], GetPlayerRect());
				enemy.entity[_i].aStarTimer = 0;
			}
		}
		CalculMoveEnemy(_dt, _i);
		enemy.activeCount++;
	}
	if (enemy.entity[_i].type > DRONE_SMALL_LARGE)
	{
		enemy.entity[_i].velocity.y += G * _dt;
	}
	UpdateColisionEnemy(_i);
}

void CalculMoveEnemy(float _dt, unsigned _i)
{
	if (enemy.entity[_i].type < GROUND_HEAVY)
	{
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

		char basOuGauche = enemy.entity[_i].action.bas - enemy.entity[_i].action.saut;
		if (basOuGauche)
		{
			enemy.entity[_i].velocity.y += basOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax * _dt;
			if (basOuGauche * enemy.entity[_i].velocity.y > basOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax)
			{
				enemy.entity[_i].velocity.y = basOuGauche * enemy.dataByType[enemy.entity[_i].type].speedMax;
			}
		}
		else
		{
			enemy.entity[_i].velocity.y = 0;
		}
	}
	else
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

		if (enemy.entity[_i].action.saut)
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

		if (enemy.entity[_i].velocity.y > enemy.dataByType[enemy.entity[_i].type].jumForce)
		{
			enemy.entity[_i].velocity.y = enemy.dataByType[enemy.entity[_i].type].jumForce;
		}
	}
	sfSprite_move(enemy.entity[_i].sprite, enemy.entity[_i].velocity);
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
	else if (realRegion.left > enemyRect.left)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { realRegion.left - enemyRect.left, 0 });
		enemy.entity[_i].velocity.x = 0;
	}
	else if (realRegion.left + realRegion.width < (enemyRect.left + enemyRect.width))
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { (realRegion.left + realRegion.width) - (enemyRect.left + enemyRect.width), 0 });
		enemy.entity[_i].velocity.x = 0;
	}

	if (collision.y)
	{
		enemy.entity[_i].velocity.y = 0;
	}
	else if (realRegion.top > enemyRect.top)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { 0, realRegion.top - enemyRect.top });
		enemy.entity[_i].velocity.y = 0;
	}
	else if (realRegion.top + realRegion.height < enemyRect.top + enemyRect.height)
	{
		sfSprite_move(enemy.entity[_i].sprite, (sfVector2f) { 0, (realRegion.top + realRegion.height) - (enemyRect.top + enemyRect.height) });
		enemy.entity[_i].velocity.y = 0;
	}
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

	AddBullet(posGun, posTarget, type, 1.f);
}

void AddEnemy(sfVector2f _position, EnemyType _type, sfFloatRect _region)
{
	_region.left -= TILE_SIZE;
	_region.top -= TILE_SIZE;
	_region.width += TILE_SIZE * 2;
	_region.height += TILE_SIZE * 2;
	if (_type == ALEATORY)
	{
		_type = rand() % ALEATORY;
	}
	sfSprite* sprite = CreateSprite(enemy.dataByType[_type].texture, _position, 1.f, 1.f);
	enemy.entity = Realloc(enemy.entity, (size_t)(enemy.count + 1) * sizeof(EnemyEntity));
	enemy.entity[enemy.count] = (EnemyEntity){ sprite, _type, (sfVector2f) { 0 }, (ActionDemander) { 0 },_region, enemy.dataByType[_type].lifeMax, 0.f, 0.f, 0.f, 0.f, 0.f };

	SetSpriteOriginFoot(enemy.entity[enemy.count].sprite);
	enemy.count++;
}

void DamageEnemyI(unsigned _i, float _degat, AttackType _type)
{
	if (_type == FREEZE)
	{
		enemy.entity[_i].freezePower = 2;
		enemy.entity[_i].freezeTimer = FREEZE_TIMER;
		PlaySound(FREEZE_EFFECT);
	}

	enemy.entity[_i].life -= _degat / (enemy.dataByType[enemy.entity[_i].type].armure + 1);
	sfSprite_setColor(enemy.entity[_i].sprite, sfColor_fromRGB(255, 0, 0));
	enemy.entity[_i].domageTimer = 0.2f;
	if (enemy.entity[_i].life < 0)
	{
		AddIntToSave(CURRENT_SCORE, enemy.dataByType[enemy.entity[_i].type].scoreValue);
		sfFloatRect rect = sfSprite_getGlobalBounds(enemy.entity[_i].sprite);
		sfVector2f pos = { rect.left + rect.width / 2.f, rect.top + rect.height / 2.f };
		float range = (rect.width + rect.height) / 4.f;
		DestroyVisualEntity(enemy.entity[_i].sprite);
		enemy.count--;
		enemy.entity[_i] = enemy.entity[enemy.count];
		if (enemy.count > 0)
		{
			enemy.entity = Realloc(enemy.entity, enemy.count * sizeof(EnemyEntity));
		}
		SpawnExplosion(pos, sfFalse, range / EXPLOSION_BASIC_RANGE);
	}
	return;
}

sfBool HitEnemyI(unsigned _i, sfVector2f _touch, float _degat, AttackType _type)
{
	sfTexture* texture = sfSprite_getTexture(enemy.entity[_i].sprite);
	sfImage* colideur = sfTexture_copyToImage(texture);
	sfColor pixelColor = sfImage_getPixel(colideur, (int)_touch.x, (int)_touch.y);
	sfImage_destroy(colideur);

	if (pixelColor.a == 255)
	{
		DamageEnemyI(_i, _degat, _type);
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
			//touch = (sfVector2f){ hitboxTir.left + hitboxTir.width / 2 - hitboxEnemy.left, hitboxTir.top + hitboxTir.height / 2 - hitboxEnemy.top };
			//if (HitEnemyI(i, touch, _degat, _type))
			DamageEnemyI(i, _degat, _type);
			{
				if (enemy.dataByType[enemy.entity[i].type].armure == MEDIUM_ARMOR)
				{
					PlaySound(ENNEMY_HURT_MEDIUM);
				}
				else if (enemy.dataByType[enemy.entity[i].type].armure == HEAVY_ARMOR)
				{
					PlaySound(ENNEMY_HURT_HEAVY);
				}
				return sfTrue;
			}
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
			FreeGrid(enemy.tableau.grid[i]);
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
#if DEBUG_MODE_A_STAR
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
#endif
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
#if DEBUG_MODE_A_STAR
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
#endif
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
		actionDemander.saut = sfTrue;
		break;
	case UP:
		actionDemander.saut = sfTrue;
		break;
	case UP_RIGHT:
		actionDemander.droite = sfTrue;
		actionDemander.saut = sfTrue;
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
			FreeGrid(enemy.tableau.grid[i]);
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
#if DEBUG_MODE_A_STAR
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
#endif
	};

	sfFloatRect bouns = sfSprite_getGlobalBounds(_enemy->sprite);
	bouns.left -= _enemy->region.left;
	bouns.top -= _enemy->region.top;
	sfIntRect bounsEnemy = FloatRectIntoIntRectByCase(bouns);
	bounsEnemy.top -= 1;
	_cible.left -= _enemy->region.left;
	_cible.top -= _enemy->region.top;
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
#if DEBUG_MODE_A_STAR
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
#endif
	}

	ActionDemander actionDemander = { 0 };

	if (bounsEnemy.top > 0 && enemy.tableau.grid[_enemy->type][bounsEnemy.top][bounsEnemy.left].direction == 0)
	{
		bounsEnemy.top -= 1;
	}
	switch (enemy.tableau.grid[_enemy->type][bounsEnemy.top][bounsEnemy.left].direction)
	{
	case EMPTY_DIRECTION:
		actionDemander.bas = sfTrue;
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
		actionDemander.saut = sfTrue;
		break;
	case UP:
		actionDemander.saut = sfTrue;
		break;
	case UP_RIGHT:
		actionDemander.droite = sfTrue;
		actionDemander.saut = sfTrue;
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