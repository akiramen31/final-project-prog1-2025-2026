#include "Elevator.h"

#define DECAL 0

Elevator elevatorList[2] = { 0 };
int numElevator;

void LoadElevatorLevel1(void);
void LoadElevatorLevel2(void);
void LoadElevatorLevel3(void);
void CreateElevator(void);
void SecurityElevator(sfVector2f _posPlayer);

int val = 0;

void LoadElevator(void)
{
	MapState map = GetCurrentMap();

	GetAsset("Assets/Sprites/Elevator.png");

	switch (map)
	{
	case LEVEL1:
		LoadElevatorLevel1();
		break;
	case LEVEL2:
		LoadElevatorLevel2();
		break;
	case LEVEL3:
		LoadElevatorLevel3();
		break;
	default:
		break;
	}
}

void UpdateElevator(sfVector2f _posPlayer, float _dt)
{
	for (int i = 0; i < numElevator; i++)
	{
		sfVector2f pos = sfRectangleShape_getPosition(elevatorList[i].hitbox[0]);

		if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] != pos.y)
		{
			sfVector2f move = { 0 };
			float vectorYLength = elevatorList[i].posYlevel[elevatorList[i].actualLevel] - pos.y;

			if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] < pos.y)
			{
				move = (sfVector2f){ 0, -SPEED_ELEVATOR * _dt };
			}
			else
			{
				move = (sfVector2f){ 0, SPEED_ELEVATOR * _dt };
			}

			if (POW2(move.y) > POW2(vectorYLength))
			{
				move.y = vectorYLength;
			}

			sfRectangleShape_move(elevatorList[i].hitbox[0], move);
		}
		pos = sfRectangleShape_getPosition(elevatorList[i].hitbox[0]);

		sfRectangleShape_setPosition(elevatorList[i].hitbox[1], (sfVector2f) { pos.x, pos.y - 24 });
		sfRectangleShape_setPosition(elevatorList[i].hitbox[2], (sfVector2f) { pos.x + 48, pos.y - 24 });

		sfSprite_setPosition(elevatorList[i].sprite, (sfVector2f) { pos.x, pos.y - 24 });
	}

	SecurityElevator(_posPlayer);
}

sfVector2f ColisionElevator(sfFloatRect _hitbox, int _axis)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };
	sfFloatRect hitbox = { 0 };

	for (int i = 0; i < numElevator; i++)
	{
		hitbox = sfRectangleShape_getGlobalBounds(elevatorList[i].hitbox[0]);

		if (sfFloatRect_intersects(&_hitbox, &hitbox, &reaction))
		{
			int resolveX = 0;

			if (_axis == AXIS_X)
			{
				resolveX = 1;
			}
			else if (_axis == AXIS_Y)
			{
				resolveX = 0;
			}
			else
			{
				resolveX = (reaction.width < reaction.height);
			}

			if (resolveX)
			{
				float hitboxCenterX = _hitbox.left + (_hitbox.width / 2.0f);
				float coliderCenterX = hitbox.left + (hitbox.width / 2.0f);

				float push = (hitboxCenterX < coliderCenterX) ? -reaction.width : reaction.width;
				vectorMove.x += push;
				_hitbox.left += push;
			}
			else
			{
				float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.0f);
				float coliderCenterY = hitbox.top + (hitbox.height / 2.0f);

				float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : reaction.height;
				vectorMove.y += push;
				_hitbox.top += push;
			}
		}

		for (int j = 1; j < 3; j++)
		{
			hitbox = sfRectangleShape_getGlobalBounds(elevatorList[i].hitbox[j]);
		}
	}
	return vectorMove;
}

sfBool ColisionElevatorButon(sfFloatRect _hitbox)
{
	sfFloatRect hitbox = { 0 };

	for (int i = 0; i < numElevator; i++)
	{
		for (int j = 1; j < 3; j++)
		{
			hitbox = sfRectangleShape_getGlobalBounds(elevatorList[i].hitbox[j]);

			if (sfFloatRect_intersects(&_hitbox, &hitbox, NULL))
			{
				if (j == 1)
				{
					elevatorList[i].actualLevel++;
				}
				else if (j == 2)
				{
					elevatorList[i].actualLevel--;
				}

				return sfTrue;
			}
		}
	}
	return sfFalse;
}

void LoadElevatorLevel1(void)
{
	numElevator = 1;

	CreateElevator();

	elevatorList[0].numberLevel = 2;

	elevatorList[0].posYlevel[0] = 752.f + DECAL;
	elevatorList[0].posYlevel[1] = 272.f + DECAL;

	elevatorList[0].actualLevel = 1;

	sfRectangleShape_setPosition(elevatorList[0].hitbox[0], (sfVector2f) { 1696, elevatorList[0].posYlevel[1] });

	for (int i = 0; i < numElevator; i++)
	{

		sfVector2f pos = sfRectangleShape_getPosition(elevatorList[i].hitbox[0]);

		sfRectangleShape_setPosition(elevatorList[i].hitbox[1], (sfVector2f) { pos.x, pos.y - 24 });
		sfRectangleShape_setPosition(elevatorList[i].hitbox[2], (sfVector2f) { pos.x + 48, pos.y - 24 });

		sfSprite_setPosition(elevatorList[i].sprite, (sfVector2f) { pos.x, pos.y - 24 });
	}
}

void LoadElevatorLevel2(void)
{
	numElevator = 2;

	CreateElevator();

	for (int i = 0; i < numElevator; i++)
	{
		elevatorList[i].numberLevel = 3;
		elevatorList[i].posYlevel[0] = 2400.f + DECAL;
		elevatorList[i].posYlevel[1] = 1600.f + DECAL;
		elevatorList[i].posYlevel[2] = 416.f + DECAL;
		elevatorList[i].actualLevel = 1;
	}
	sfRectangleShape_setPosition(elevatorList[0].hitbox[0], (sfVector2f) { 2624, elevatorList[0].posYlevel[1] });
	sfRectangleShape_setPosition(elevatorList[1].hitbox[0], (sfVector2f) { 4608, elevatorList[0].posYlevel[1] });

	for (int i = 0; i < numElevator; i++)
	{

		sfVector2f pos = sfRectangleShape_getPosition(elevatorList[i].hitbox[0]);

		sfRectangleShape_setPosition(elevatorList[i].hitbox[1], (sfVector2f) { pos.x, pos.y - 24 });
		sfRectangleShape_setPosition(elevatorList[i].hitbox[2], (sfVector2f) { pos.x + 48, pos.y - 24 });

		sfSprite_setPosition(elevatorList[i].sprite, (sfVector2f) { pos.x, pos.y - 24 });
	}
}

void LoadElevatorLevel3(void)
{

}

void CreateElevator(void)
{
	for (int i = 0; i < numElevator; i++)
	{
		if (elevatorList[i].hitbox[0] == NULL)
		{
			sfFloatRect rect = { 0,0,64,16 };
			elevatorList[i].hitbox[0] = CreateRectangleShape(rect, sfTransparent, sfRed, 70.f);
		}

		for (int j = 1; j < 3; j++)
		{
			if (elevatorList[i].hitbox[j] == NULL)
			{
				sfFloatRect rect = { 0,0,16,16 };
				elevatorList[i].hitbox[j] = CreateRectangleShape(rect, sfTransparent, sfBlue, 70.f);
			}
		}

		if (elevatorList[i].sprite == NULL)
		{
			elevatorList[i].sprite = CreateSprite(GetAsset("Assets/Sprites/Elevator.png"), (sfVector2f) { 0, 0 }, 1.f, 70.f);
		}
	}
}

void SecurityElevator(sfVector2f _posPlayer)
{
	for (int i = 0; i < numElevator; i++)
	{
		float length = 0;
		sfFloatRect rect = sfRectangleShape_getGlobalBounds(elevatorList[i].hitbox[0]);
		rect.left -= rect.width * 4;
		rect.width *= 9;

		if (_posPlayer.x < rect.left || _posPlayer.x > rect.left + rect.width)
		{
			if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] < _posPlayer.y)
			{
				if (elevatorList[i].actualLevel > 0)
				{
					length = elevatorList[i].posYlevel[elevatorList[i].actualLevel - 1] - elevatorList[i].posYlevel[elevatorList[i].actualLevel];

					if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] + length / 2 < _posPlayer.y)
					{
						//printf("elevator %d DOWN %d\n", i, val);
						//val++;
						elevatorList[i].actualLevel--;
						sfRectangleShape_setPosition(elevatorList[i].hitbox[0], (sfVector2f) { sfRectangleShape_getPosition(elevatorList[i].hitbox[0]).x, elevatorList[i].posYlevel[elevatorList[i].actualLevel] });
					}
				}
			}
			else
			{
				if (elevatorList[i].actualLevel < elevatorList[i].numberLevel - 1)
				{
					length = elevatorList[i].posYlevel[elevatorList[i].actualLevel + 1] - elevatorList[i].posYlevel[elevatorList[i].actualLevel];

					if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] + length / 2 > _posPlayer.y)
					{
						//printf("elevator %d UP %d\n", i, val);
						//val++;
						elevatorList[i].actualLevel++;
						sfRectangleShape_setPosition(elevatorList[i].hitbox[0], (sfVector2f) { sfRectangleShape_getPosition(elevatorList[i].hitbox[0]).x, elevatorList[i].posYlevel[elevatorList[i].actualLevel] });
					}
				}
			}
		}

		if (elevatorList[i].actualLevel >= elevatorList[i].numberLevel)
		{
			elevatorList[i].actualLevel = elevatorList[i].numberLevel - 1;
		}
		if (elevatorList[i].actualLevel < 0)
		{
			elevatorList[i].actualLevel = 0;
		}
	}
}