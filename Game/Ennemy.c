#include "Ennemy.h"

List* listeEnnemy;

void LoadEnnemy(void)
{
	listeEnnemy = CreateList();
}

void CreateRandomEnnemy(Ennemy* _ennemy)
{
	_ennemy->typeEnnemy = GetRandomInRange(0, TOTAL_TYPE_ENNEMY - 1 - 1);
	switch (_ennemy->typeEnnemy)
	{
	case BALLOM:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 100;
		_ennemy->vitesse = LENT;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 40.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 30 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,18,20,22 }, sfTrue, 4, 0.2,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,58,20,22 }, sfTrue, 4, 0.2,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,98,20,22 }, sfTrue, 4, 0.2,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,138,20,22 }, sfTrue, 4, 0.2,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,178,20,22 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,18,20,22 }, sfTrue, 1, 0.2,0.f };
		break;
	case ONIL:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 200;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 40.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 24 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,823,20,17 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,863,20,17 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,903,20,17 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,943,20,17 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,982,20,17 }, sfTrue, 4, 0.2,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,823,20,17 }, sfTrue, 1, 0.2,0.f };
		break;
	case DALL:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 400;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 40.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 30 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,218,20,24 }, sfTrue, 3, 0.2,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,258,20,24 }, sfTrue, 3, 0.2,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,298,20,24 }, sfTrue, 3, 0.2,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,338,20,24 }, sfTrue, 3, 0.2,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,378,20,24 }, sfTrue, 6, 0.2,0.f };
		_ennemy->animation[5] = (Animation){ (sfIntRect) { 0,218,20,24 }, sfTrue, 1, 0.2,0.f };
		break;
	case DENKYUN:
		_ennemy->life = 2;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 600;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 40.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 30 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,614,20,25 }, sfTrue, 6, 0.2,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,654,20,25 }, sfTrue, 6, 0.2,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,774,20,25 }, sfTrue, 5, 0.2,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,614,20,25 }, sfTrue, 1, 0.2,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,654,20,25 }, sfTrue, 1, 0.2,0.f };
		break;
	case KONDORIA:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 1;
		_ennemy->points = 1000;
		_ennemy->vitesse = TRES_LENT;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 40.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 30 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,416,20,22 }, sfTrue, 2, 0.2,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,458,20,22 }, sfTrue, 2, 0.2,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,498,20,22 }, sfTrue, 2, 0.2,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,538,20,22 }, sfTrue, 2, 0.2,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,578,20,22 }, sfTrue, 5, 0.2,0.f };
		break;
	default:
		break;
	}
	sfSprite_setScale(_ennemy->sprite, (sfVector2f) { 3, 3 });
}


void AddEnnemy(sfVector2i _position, CasePosibility _casePosibility)
{
	printf("add ennemy\n");
	Ennemy* newEnnemy = calloc(1, sizeof(Ennemy));
	if (newEnnemy == NULL)
	{
		exit(EXIT_FAILURE);
	}
	CreateRandomEnnemy(newEnnemy);
	sfVector2f position = TransformVector2iToVector2f(_position);
	sfSprite_setPosition(newEnnemy->sprite, position);
	newEnnemy->position = _position;
	InsertElement(listeEnnemy, CreateElement(newEnnemy), 0);
	NewChoiceDirection(_casePosibility, 0);
}

Ennemy* GetEnnemy(unsigned _index)
{
	Element* element = GetElement(listeEnnemy, _index);
	return element->value;
}

void UpdateEnnemy(float _dt, CasePosibility _casePosibility, int _i)
{

	switch (GetEnnemy(_i)->direction)
	{
	case DOWN:
		sfSprite_move(GetEnnemy(_i)->sprite, (sfVector2f) { 0, GetEnnemy(_i)->vitesse*_dt });
		break;
	case LEFT:
		sfSprite_move(GetEnnemy(_i)->sprite, (sfVector2f) { -GetEnnemy(_i)->vitesse * _dt, 0 });
		break;
	case RIGHT:
		sfSprite_move(GetEnnemy(_i)->sprite, (sfVector2f) { GetEnnemy(_i)->vitesse* _dt, 0 });
		break;
	case UP:
		sfSprite_move(GetEnnemy(_i)->sprite, (sfVector2f) { 0, -GetEnnemy(_i)->vitesse * _dt});
		break;
	case BLOCK:
		NewChoiceDirection(_casePosibility, _i);
		break;
	default:
		printf("error direction");
		break;
	}
	printf("direction %d\n", GetEnnemy(_i)->direction);
	printf("ancienne position x:%d y:%d\n", GetEnnemy(_i)->position.x, GetEnnemy(_i)->position.y);
	if (64 < abs(TransformVector2iToVector2f(GetEnnemy(_i)->position).x - sfSprite_getPosition(GetEnnemy(_i)->sprite).x)
		|| 64 < abs(TransformVector2iToVector2f(GetEnnemy(_i)->position).y - sfSprite_getPosition(GetEnnemy(_i)->sprite).y))
	{
		switch (GetEnnemy(_i)->direction)
		{
		case DOWN:
			GetEnnemy(_i)->position.y++;
			break;
		case LEFT:
			GetEnnemy(_i)->position.x--;
			break;
		case RIGHT:
			GetEnnemy(_i)->position.x++;
			break;
		case UP:
			GetEnnemy(_i)->position.y--;
			break;
		default:
			printf("error direction");
			break;
		}
		sfSprite_setPosition(GetEnnemy(_i)->sprite, TransformVector2iToVector2f(GetEnnemy(_i)->position));
		NewChoiceDirection(_casePosibility, _i);
		printf("new direction %d\n", GetEnnemy(_i)->direction);
		printf("position x:%d y:%d\n", GetEnnemy(_i)->position.x, GetEnnemy(_i)->position.y);
	}

	UpdateAnimationAndGiveIfStop(GetEnnemy(_i)->sprite, &(GetEnnemy(_i)->animation[GetAnimation(GetEnnemy(_i))]), _dt);
}

unsigned GetAnimation(Ennemy* _ennemy)
{
	if (_ennemy->life)
	{
		if (_ennemy->typeEnnemy == 3)
		{
			if (_ennemy->life == 2)
			{
				if (_ennemy->animation == BLOCK)
				{
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if (_ennemy->animation == BLOCK)
				{
					return 3;
				}
				else
				{
					return 1;
				}
			}
		}
		else
		{
			switch (_ennemy->direction)
			{
			case DOWN:
				return 0;
				break;
			case LEFT:
				return 2;
				break;
			case RIGHT:
				return 3;
				break;
			case UP:
				return 1;
				break;
			case BLOCK:
				return 5;
			default:
				break;
			}
		}
	}
	else
	{
		return 4;
	}
}

int GetNumberEnnemy(void)
{
	return GetListSize(listeEnnemy);
}

sfVector2i GetPositionEnnemy(unsigned _index)
{
	sfVector2i positionI = GetEnnemy(_index)->position;
	return positionI;
}

void NewChoiceDirection(CasePosibility _casePosibility, int _i)
{
	sfBool confirmDirection = 0;
	if (!(_casePosibility.down + _casePosibility.left + _casePosibility.right + _casePosibility.up))
	{
		GetEnnemy(_i)->direction = BLOCK;
		confirmDirection = 1;
	}
	while (confirmDirection == 0)
	{
		enum Direction temp = GetRandomInRange(0, 3);
		switch (temp)
		{
		case DOWN:
			if (_casePosibility.down)
			{
				GetEnnemy(_i)->direction = DOWN;
				confirmDirection = 1;
			}
			break;
		case LEFT:
			if (_casePosibility.left)
			{
				GetEnnemy(_i)->direction = LEFT;
				confirmDirection = 1;
			}
			break;
		case RIGHT:
			if (_casePosibility.right)
			{
				GetEnnemy(_i)->direction = RIGHT;
				confirmDirection = 1;
			}
			break;
		case UP:
			if (_casePosibility.up)
			{
				GetEnnemy(_i)->direction = UP;
				confirmDirection = 1;
			}
			break;
		default:
			break;
		}
		printf("direction demander %d. struc up:%d down %d left %d right %d \n", GetEnnemy(_i)->direction, _casePosibility.up, _casePosibility.down, _casePosibility.left, _casePosibility.right);
	}

}

sfVector2i GetFuturPositionEnnemy(unsigned _index)
{
	enum Direction temp = GetEnnemy(_index)->direction;
	sfVector2i positionActuel = GetEnnemy(_index)->position;
	switch (temp)
	{
	case DOWN:
		return (sfVector2i) { positionActuel.x, positionActuel.y + 1 };
		break;
	case LEFT:
		return (sfVector2i) { positionActuel.x - 1, positionActuel.y };
		break;
	case RIGHT:
		return (sfVector2i) { positionActuel.x + 1, positionActuel.y };
		break;
	case UP:
		return (sfVector2i) { positionActuel.x, positionActuel.y - 1 };
		break;
	case BLOCK:
		return positionActuel;
		break;
	default:
		break;
	}
	return (sfVector2i) { 0, 0 };
}
