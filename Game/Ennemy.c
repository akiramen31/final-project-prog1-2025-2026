#include "Ennemy.h"

List* listeEnnemy;

void LoadEnnemy(void)
{
	listeEnnemy = CreateList();
}

void CreateRandomEnnemy(Ennemy* _ennemy)
{
	_ennemy->typeEnnemy = GetRandomInRange(0, TOTAL_TYPE_ENNEMY - 1);
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

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 1000.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 20 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,18,20,22 }, sfTrue, 4, 1.f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,58,20,22 }, sfTrue, 4, 1.f,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,98,20,22 }, sfTrue, 4, 1.f,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,138,20,22 }, sfTrue, 4, 1.f,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,178,20,22 }, sfTrue, 5, 1.f,0.f };
		break;
	case ONIL:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 200;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 1000.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 16 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,823,20,17 }, sfTrue, 5, 1.f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,863,20,17 }, sfTrue, 5, 1.f,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,903,20,17 }, sfTrue, 5, 1.f,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,943,20,17 }, sfTrue, 5, 1.f,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,982,20,17 }, sfTrue, 4, 1.f,0.f };
		break;
	case DALL:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 400;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 1000.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 21 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,218,20,24 }, sfTrue, 3, 1.f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,258,20,24 }, sfTrue, 3, 1.f,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,298,20,24 }, sfTrue, 3, 1.f,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,338,20,24 }, sfTrue, 3, 1.f,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,378,20,24 }, sfTrue, 6, 1.f,0.f };
		break;
	case DENKYUN:
		_ennemy->life = 2;
		_ennemy->passeMuraille = 0;
		_ennemy->points = 600;
		_ennemy->vitesse = NORMAL;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 1000.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 21 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,614,20,25 }, sfTrue, 6, 1.f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,654,20,25 }, sfTrue, 6, 1.f,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,774,20,25 }, sfTrue, 5, 1.f,0.f };
		break;
	case KONDORIA:
		_ennemy->life = 1;
		_ennemy->passeMuraille = 1;
		_ennemy->points = 1000;
		_ennemy->vitesse = TRES_LENT;
		_ennemy->direction = 0;

		_ennemy->personality.aggressiveness = 0;
		_ennemy->personality.chaos = 10;

		_ennemy->sprite = CreateSprite(GetAsset("Assets/Sprites/Enemies/Enemies.png"), (sfVector2f) { 0, 0 }, 1.f, 1000.f);
		sfSprite_setOrigin(_ennemy->sprite, (sfVector2f) { 10, 20 });

		_ennemy->animation[0] = (Animation){ (sfIntRect) { 0,416,20,22 }, sfTrue, 2, 1.f,0.f };
		_ennemy->animation[1] = (Animation){ (sfIntRect) { 0,458,20,22 }, sfTrue, 2, 1.f,0.f };
		_ennemy->animation[2] = (Animation){ (sfIntRect) { 0,498,20,22 }, sfTrue, 2, 1.f,0.f };
		_ennemy->animation[3] = (Animation){ (sfIntRect) { 0,538,20,22 }, sfTrue, 2, 1.f,0.f };
		_ennemy->animation[4] = (Animation){ (sfIntRect) { 0,578,20,22 }, sfTrue, 5, 1.f,0.f };
		break;
	default:
		break;
	}
	sfSprite_setScale(_ennemy->sprite, (sfVector2f) { 3, 3 });
}


void AddEnnemy(sfVector2i _position)
{
	Ennemy* newEnnemy = calloc(1, sizeof(Ennemy));
	if (newEnnemy == NULL)
	{
		exit(EXIT_FAILURE);
	}
	CreateRandomEnnemy(newEnnemy);
	sfVector2f position = { (float)_position.x , (float)_position.y };
	sfSprite_setPosition(newEnnemy->sprite, position);
	InsertElement(listeEnnemy, CreateElement(newEnnemy), 0);
}

Ennemy* GetEnnemy(unsigned _index)
{
	Element* element = GetElement(listeEnnemy, _index);
	return element->value;
}

void UpdateEnnemy(float _dt)
{
	for (unsigned i = 0; i < GetListSize(listeEnnemy); i++)
	{
		UpdateAnimationAndGiveIfStop(GetEnnemy(i)->sprite, &(GetEnnemy(i)->animation[GetAnimation(GetEnnemy(i))]), _dt);
	}
}

unsigned GetAnimation(Ennemy* _ennemy)
{
	if (_ennemy->life)
	{
		if (_ennemy->typeEnnemy == 3)
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
	sfVector2f positionF = sfSprite_getPosition(GetEnnemy(_index)->sprite);
	sfVector2i positionI = (sfVector2i){ positionF.x, positionF.y };
	return positionI;
}
