#include "Ennemy.h"

List* listeEnnemy;

void LoadEnnemy(void)
{
	listeEnnemy = CreateList();
}

void CreateEnnemyRandom(Ennemy* _ennemy)
{
	_ennemy->typeEnnemy = rand() % TOTAL_TYPE_ENNEMY;
	_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 30.f);
	sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10.f, 45.f });
	_ennemy->life = 1;

	switch (_ennemy->typeEnnemy)
	{
	case BALLOM:
		_ennemy->passeMuraille = 0;
		_ennemy->points = 100;
		_ennemy->vitesse = LENT;

		_ennemy->personality.aggressiveness = GetRandomInRange(0, 1);
		_ennemy->personality.chaos = GetRandomInRange(5,7);


		_ennemy->animation[BLOCK] = (Animation){ (sfIntRect) { 0,0,20,40 }, sfTrue, 1, 0.2f,0.f };
		_ennemy->animation[DOWN] = (Animation){ (sfIntRect) { 0,0,20,40 }, sfTrue, 4, 0.2f,0.f };
		_ennemy->animation[UP] = (Animation){ (sfIntRect) { 0,40,20,40 }, sfTrue, 4, 0.2f,0.f };
		_ennemy->animation[LEFT] = (Animation){ (sfIntRect) { 0,80,20,40 }, sfTrue, 4, 0.2f,0.f };
		_ennemy->animation[RIGHT] = (Animation){ (sfIntRect) { 0,120,20,40 }, sfTrue, 4, 0.2f,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,160,20,40 }, sfFalse, 5, 0.2f,0.f };
		break;
	case ONIL:
		_ennemy->passeMuraille = 0;
		_ennemy->points = 200;
		_ennemy->vitesse = NORMAL;

		_ennemy->personality.aggressiveness = GetRandomInRange(1, 2);
		_ennemy->personality.chaos = GetRandomInRange(4, 6);


		_ennemy->animation[BLOCK] = (Animation){ (sfIntRect) { 0,800,20,40 }, sfTrue, 1, 0.2f,0.f };
		_ennemy->animation[DOWN] = (Animation){ (sfIntRect) { 0,800,20,40 }, sfTrue, 5, 0.2f,0.f };
		_ennemy->animation[UP] = (Animation){ (sfIntRect) { 0,840,20,40 }, sfTrue, 5, 0.2f,0.f };
		_ennemy->animation[LEFT] = (Animation){ (sfIntRect) { 0,880,20,40 }, sfTrue, 5, 0.2f,0.f };
		_ennemy->animation[RIGHT] = (Animation){ (sfIntRect) { 0,920,20,40 }, sfTrue, 5, 0.2f,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,960,20,40 }, sfFalse, 4, 0.2f,0.f };
		break;
	case DALL:
		_ennemy->passeMuraille = 0;
		_ennemy->points = 400;
		_ennemy->vitesse = NORMAL;

		_ennemy->personality.aggressiveness = 2;
		_ennemy->personality.chaos = GetRandomInRange(2, 5);


		_ennemy->animation[BLOCK] = (Animation){ (sfIntRect) { 0,200,20,40 }, sfTrue, 1, 0.2f,0.f };
		_ennemy->animation[DOWN] = (Animation){ (sfIntRect) { 0,200,20,40 }, sfTrue, 3, 0.2f,0.f };
		_ennemy->animation[UP] = (Animation){ (sfIntRect) { 0,240,20,40 }, sfTrue, 3, 0.2f,0.f };
		_ennemy->animation[LEFT] = (Animation){ (sfIntRect) { 0,280,20,40 }, sfTrue, 3, 0.2f,0.f };
		_ennemy->animation[RIGHT] = (Animation){ (sfIntRect) { 0,320,20,40 }, sfTrue, 3, 0.2f,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,360,20,40 }, sfFalse, 6, 0.2f,0.f };
		break;
	case DENKYUN:
		_ennemy->life++;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 600;
		_ennemy->vitesse = NORMAL;

		_ennemy->personality.aggressiveness = GetRandomInRange(0, 2);
		_ennemy->personality.chaos = GetRandomInRange(1, 9);


		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,600,20,40 }, sfTrue, 6, 0.2f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,640,20,40 }, sfTrue, 6, 0.2f,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,760,20,40 }, sfFalse, 5, 0.2f,0.f };
		break;
	case KONDORIA:
		_ennemy->passeMuraille = 1;
		_ennemy->points = 1000;
		_ennemy->vitesse = TRES_LENT;

		_ennemy->personality.aggressiveness = 2;
		_ennemy->personality.chaos = 9;

		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10.f, 30.f });

		_ennemy->animation[BLOCK] = (Animation){ (sfIntRect) { 0,416,20,22 }, sfTrue, 1, 0.2f,0.f };
		_ennemy->animation[DOWN] = (Animation){ (sfIntRect) { 0,416,20,22 }, sfTrue, 2, 0.2f,0.f };
		_ennemy->animation[UP] = (Animation){ (sfIntRect) { 0,458,20,22 }, sfTrue, 2, 0.2f,0.f };
		_ennemy->animation[LEFT] = (Animation){ (sfIntRect) { 0,498,20,22 }, sfTrue, 2, 0.2f,0.f };
		_ennemy->animation[RIGHT] = (Animation){ (sfIntRect) { 0,538,20,22 }, sfTrue, 2, 0.2f,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,578,20,22 }, sfFalse, 5, 0.2f,0.f };
		break;
	default:
		break;
	}
	_ennemy->direction = 0;
	sfSprite_setScale(_ennemy->sprite, (sfVector2f) { 3.f, 3.f });
}

void AddEnnemy(sfVector2i _position, CasePosibility _casePosibility)
{
	Ennemy* newEnnemy = calloc(1, sizeof(Ennemy));
	if (newEnnemy == NULL)
	{
		exit(EXIT_FAILURE);
	}
	CreateEnnemyRandom(newEnnemy);
	sfVector2f position = TransformVector2iToVector2f(_position);
	sfSprite_setPosition(newEnnemy->sprite, position);
	newEnnemy->position = _position;
	InsertElement(listeEnnemy, CreateElement(newEnnemy), 0);
	ChooseNewDirection(_casePosibility, 0, (sfVector2i){0,0});
}

Ennemy* GetEnnemy(unsigned _index)
{
	return GetElement(listeEnnemy, _index)->value;
}

void UpdateEnnemy(float _dt, CasePosibility _casePosibility, int _i, sfVector2i _positionjoueur)
{
	Ennemy* ennemy = GetEnnemy(_i);

	if (ennemy->invincibleTimer > 0)
	{
		ennemy->invincibleTimer -= _dt;
		if (ennemy->invincibleTimer < 0)
		{
			ennemy->invincibleTimer = 0;
		}
	}

	switch (ennemy->direction)
	{
	case DOWN:
		sfSprite_move(ennemy->sprite, (sfVector2f) { 0, ennemy->vitesse* _dt });
		break;
	case LEFT:
		sfSprite_move(ennemy->sprite, (sfVector2f) { -ennemy->vitesse * _dt, 0 });
		break;
	case RIGHT:
		sfSprite_move(ennemy->sprite, (sfVector2f) { ennemy->vitesse* _dt, 0 });
		break;
	case UP:
		sfSprite_move(ennemy->sprite, (sfVector2f) { 0, -ennemy->vitesse * _dt });
		break;
	case BLOCK:
		ChooseNewDirection(_casePosibility, _i, _positionjoueur);
		break;
	default:
		printf("error direction");
		break;
	}
	if (64 < abs((int)TransformVector2iToVector2f(ennemy->position).x - (int)sfSprite_getPosition(ennemy->sprite).x) || 64 < abs((int)TransformVector2iToVector2f(ennemy->position).y - (int)sfSprite_getPosition(ennemy->sprite).y))
	{
		switch (ennemy->direction)
		{
		case DOWN:
			ennemy->position.y++;
			break;
		case LEFT:
			ennemy->position.x--;
			break;
		case RIGHT:
			ennemy->position.x++;
			break;
		case UP:
			ennemy->position.y--;
			break;
		default:
			printf("error direction");
			break;
		}
		sfSprite_setPosition(ennemy->sprite, TransformVector2iToVector2f(ennemy->position));
		if (ennemy->passeMuraille)
		{
			ChooseNewDirectionV2(_i);
		}
		else
		{
			ChooseNewDirection(_casePosibility, _i, _positionjoueur);
		}
	}

	if (UpdateAnimationAndGiveIfStop(ennemy->sprite, &ennemy->animation[GetEnnemyAnimation(ennemy)], _dt))
	{
		sfSprite_setScale(ennemy->sprite, (sfVector2f) { 0, 0 });
		AddIntToSave(SCORE, ennemy->points);
		free(ennemy);
		RemoveElement(listeEnnemy, _i);
	}
}

unsigned GetEnnemyAnimation(Ennemy* _ennemy)
{
	if (_ennemy->life)
	{
		if (_ennemy->typeEnnemy == DENKYUN)
		{
			if (_ennemy->life == 2)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return _ennemy->direction;
		}
	}
	else
	{
		return 5;
	}
	return 0;
}

int GetEnnemyCount(void)
{
	return GetListSize(listeEnnemy);
}

sfVector2i GetEnnemyPosition(unsigned _index)
{
	return GetEnnemy(_index)->position;
}

void ChooseNewDirection(CasePosibility _casePosibility, int _i, sfVector2i _positionjoueur)
{
	sfBool searchDirection = sfTrue;
	Ennemy* ennemy = GetEnnemy(_i);
	Direction temp = 0;
	int distanceDown = 0;
	int distanceLeft = 0;
	if (!(_casePosibility.down + _casePosibility.left + _casePosibility.right + _casePosibility.up))
	{
		ennemy->direction = BLOCK;
		return;
	}
	while (searchDirection)
	{
		if (ennemy->personality.chaos >= rand() % 10)
		{
			temp = rand() % 4;
			switch (temp)
			{
			case DOWN:
				if (_casePosibility.down)
				{
					ennemy->direction = DOWN;
					searchDirection = sfFalse;
				}
				break;
			case LEFT:
				if (_casePosibility.left)
				{
					ennemy->direction = LEFT;
					searchDirection = sfFalse;
				}
				break;
			case RIGHT:
				if (_casePosibility.right)
				{
					ennemy->direction = RIGHT;
					searchDirection = sfFalse;
				}
				break;
			case UP:
				if (_casePosibility.up)
				{
					ennemy->direction = UP;
					searchDirection = sfFalse;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			distanceDown = abs(_positionjoueur.y - ennemy->position.y);
			distanceLeft = abs(_positionjoueur.x - ennemy->position.x);
			switch (ennemy->personality.aggressiveness)
			{
			case 0:
				if (distanceDown > distanceLeft)
				{
					if (_positionjoueur.y > ennemy->position.y)
					{
						if (_casePosibility.up)
						{
							ennemy->direction = UP;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.down)
						{
							ennemy->direction = DOWN;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				else
				{
					if (_positionjoueur.x > ennemy->position.x)
					{
						if (_casePosibility.left)
						{
							ennemy->direction = LEFT;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.right)
						{
							ennemy->direction = RIGHT;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				temp = rand() % 4;
				switch (temp)
				{
				case DOWN:
					if (_casePosibility.down)
					{
						ennemy->direction = DOWN;
						searchDirection = sfFalse;
					}
					break;
				case LEFT:
					if (_casePosibility.left)
					{
						ennemy->direction = LEFT;
						searchDirection = sfFalse;
					}
					break;
				case RIGHT:
					if (_casePosibility.right)
					{
						ennemy->direction = RIGHT;
						searchDirection = sfFalse;
					}
					break;
				case UP:
					if (_casePosibility.up)
					{
						ennemy->direction = UP;
						searchDirection = sfFalse;
					}
					break;
				default:
					break;
				}
				break;
			case 1 :
				if (distanceDown > distanceLeft)
				{
					if (rand() % 2)
					{
						if (_casePosibility.left)
						{
							ennemy->direction = LEFT;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.right)
						{
							ennemy->direction = RIGHT;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				else
				{
					if (rand() % 2)
					{
						if (_casePosibility.up)
						{
							ennemy->direction = UP;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.down)
						{
							ennemy->direction = DOWN;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				temp = rand() % 4;
				switch (temp)
				{
				case DOWN:
					if (_casePosibility.down)
					{
						ennemy->direction = DOWN;
						searchDirection = sfFalse;
					}
					break;
				case LEFT:
					if (_casePosibility.left)
					{
						ennemy->direction = LEFT;
						searchDirection = sfFalse;
					}
					break;
				case RIGHT:
					if (_casePosibility.right)
					{
						ennemy->direction = RIGHT;
						searchDirection = sfFalse;
					}
					break;
				case UP:
					if (_casePosibility.up)
					{
						ennemy->direction = UP;
						searchDirection = sfFalse;
					}
					break;
				default:
					break;
				}
				break;
			case 2:
				if (distanceDown > distanceLeft)
				{
					if (_positionjoueur.y > ennemy->position.y)
					{
						if (_casePosibility.down)
						{
							ennemy->direction = DOWN;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.up)
						{
							ennemy->direction = UP;
							searchDirection = sfFalse;
							break;
						}
					}
					if (_positionjoueur.x > ennemy->position.x)
					{
						if (_casePosibility.right)
						{
							ennemy->direction = RIGHT;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.left)
						{
							ennemy->direction = LEFT;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				else
				{
					if (_positionjoueur.x > ennemy->position.x)
					{
						if (_casePosibility.right)
						{
							ennemy->direction = RIGHT;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.left)
						{
							ennemy->direction = LEFT;
							searchDirection = sfFalse;
							break;
						}
					}
					if (_positionjoueur.y > ennemy->position.y)
					{
						if (_casePosibility.down)
						{
							ennemy->direction = DOWN;
							searchDirection = sfFalse;
							break;
						}
					}
					else
					{
						if (_casePosibility.up)
						{
							ennemy->direction = UP;
							searchDirection = sfFalse;
							break;
						}
					}
				}
				temp = rand() % 4;
				switch (temp)
				{
				case DOWN:
					if (_casePosibility.down)
					{
						ennemy->direction = DOWN;
						searchDirection = sfFalse;
					}
					break;
				case LEFT:
					if (_casePosibility.left)
					{
						ennemy->direction = LEFT;
						searchDirection = sfFalse;
					}
					break;
				case RIGHT:
					if (_casePosibility.right)
					{
						ennemy->direction = RIGHT;
						searchDirection = sfFalse;
					}
					break;
				case UP:
					if (_casePosibility.up)
					{
						ennemy->direction = UP;
						searchDirection = sfFalse;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}

}

void ChooseNewDirectionV2(int _i)
{
	sfBool searchDirection = sfTrue;
	Ennemy* ennemy = GetEnnemy(_i);
	Direction temp = 0;
	while (searchDirection)
	{
		temp = rand() % 4;
		switch (temp)
		{
		case DOWN:
			if (ennemy->position.y < NB_GRID_ROW - 1)
			{
				searchDirection = sfFalse;
			}
			break;
		case LEFT:
			if (ennemy->position.x > 0)
			{
				searchDirection = sfFalse;
			}
			break;
		case RIGHT:
			if (ennemy->position.x < NB_GRID_COLUMN - 1)
			{
				searchDirection = sfFalse;
			}
			break;
		case UP:
			if (ennemy->position.y > 0)
			{
				searchDirection = sfFalse;
			}
			break;
		default:
			break;
		}
	}
	ennemy->direction = temp;

}

sfVector2i GetEnnemyNextPosition(unsigned _index)
{
	Ennemy* ennemy = GetEnnemy(_index);
	sfVector2i positionNext = ennemy->position;
	switch (ennemy->direction)
	{
	case DOWN:
		positionNext.y++;
		break;
	case LEFT:
		positionNext.x--;
		break;
	case RIGHT:
		positionNext.x++;
		break;
	case UP:
		positionNext.y--;
		break;
	default:
		break;
	}
	return positionNext;
}

void HitEnnemy(unsigned _index)
{
	Ennemy* ennemy = GetEnnemy(_index);
	if (!ennemy->invincibleTimer)
	{
		ennemy->invincibleTimer = ENNEMY_TIME_OF_INVINCIBLE;
		ennemy->life -= 1;
	}
}