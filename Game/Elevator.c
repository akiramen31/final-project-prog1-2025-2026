#include "Elevator.h"
#include "Player.h"


Elevator elevatorList[2] = { 0 };
int numElevator;

void LoadElevatorLevel1(void);
void LoadElevatorLevel2(void);
void LoadElevatorLevel3(void);
void CreateElevator(void);

void LoadElevator(void)
{
	MapState map = GetActualyMap();

	GetAsset("Assets/Sprites/Elevator.png");
	elevatorList[0] = (Elevator){ 0 };
	elevatorList[1] = (Elevator){ 0 };

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

void UpdateElevator(float _dt)
{
	for (int i = 0; i < numElevator; i++)
	{
		sfVector2f pos = sfSprite_getPosition(elevatorList[i].sprite);
		sfVector2f move = { 0 };

		float vectorYLength = elevatorList[i].posYlevel[elevatorList[i].actualLevel] - pos.y;

		if (elevatorList[i].posYlevel[elevatorList[i].actualLevel] != pos.y)
		{
			if (DEV_ELEVATOR)
			{
				printf("elevator %d pos : %f\n", i, pos.y);
			}

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

			sfSprite_move(elevatorList[i].sprite, move);
		}
	}
}

void LoadElevatorLevel1(void)
{
	numElevator = 1;

	CreateElevator();

	elevatorList[0].numberLevel = 2;

	elevatorList[0].posYlevel[0] = 752.f;
	elevatorList[0].posYlevel[1] = 272.f;

	elevatorList[0].actualLevel = 0;

	sfSprite_setPosition(elevatorList[0].sprite, (sfVector2f) { 1696, elevatorList[0].posYlevel[1] });
}

void LoadElevatorLevel2(void)
{
	numElevator = 2;

	CreateElevator();

	elevatorList[0].numberLevel = 3;

	for (int i = 0; i < numElevator; i++)
	{
		elevatorList[i].posYlevel[0] = 2400.f;
		elevatorList[i].posYlevel[1] = 1600.f;
		elevatorList[i].posYlevel[2] = 416.f;
		elevatorList[i].actualLevel = 1;
	}
	sfSprite_setPosition(elevatorList[0].sprite, (sfVector2f) { 2624, elevatorList[0].posYlevel[1] });
	sfSprite_setPosition(elevatorList[1].sprite, (sfVector2f) { 4608, elevatorList[0].posYlevel[1] });
}

void LoadElevatorLevel3(void)
{

}

void CreateElevator(void)
{
	for (int i = 0; i < numElevator; i++)
	{
		elevatorList[i].sprite = CreateSprite(GetAsset("Assets/Sprites/Elevator.png"), (sfVector2f) { 0, 0 }, 1.f, 70.f);
	}
}