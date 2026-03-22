#include "PowerUp.h"

sfTexture* powerUpTextureRef;
PowerUpEntity powerUpList[POWER_UP_COUNT];
sfBool exitIsAccessible;
sfSound* booster;

void LoadPowerUp(void)
{
	booster = CreateSound(GetAsset("Assets/Sounds/Booster.wav"), 100.f, sfFalse);
	powerUpTextureRef = GetAsset("Assets/Sprites/Items/Items.png");
	exitIsAccessible = sfFalse;

	powerUpList[0].powerUpType = EXIT;
	powerUpList[0].sprite = CreateSprite(GetAsset("Assets/Sprites/Map/Exit.png"), (sfVector2f) { 0 }, 4.f, 90.f);
	sfSprite_setTextureRect(powerUpList[0].sprite, (sfIntRect) { 0, 0, 16, 16 });
	sfSprite_setOrigin(powerUpList[0].sprite, (sfVector2f) { 8, 16 });

	for (int i = 1; i < POWER_UP_COUNT; i++)
	{
		powerUpList[i].sprite = CreateSprite(powerUpTextureRef, (sfVector2f) { 0 }, 4.f, 90.f);

		powerUpList[i].isVisible = sfFalse;
		powerUpList[i].isCollected = sfFalse;
		if (GetIntFromSave(ROUND) == 1)
		{
			if (i >= 1 && i <= 2)
			{
				powerUpList[i].powerUpType = FIRE_UP;
			}
			else if (i >= 3 && i <= 4)
			{
				powerUpList[i].powerUpType = BOMB_UP;
			}
			else if (i == 5)
			{
				powerUpList[i].powerUpType = SPEED_UP;
			}
			else if (i >= 6 && i <= 10)
			{
				int malusTypes[3] = { 2, 4, 6 };
				powerUpList[i].powerUpType = (PowerUpType)malusTypes[rand() % 3];
			}
		}
		else
		{
			if (i >= 1 && i <= 5)
			{
				int bonusTypes[4] = { 0, 1, 3, 5 };
				powerUpList[i].powerUpType = (PowerUpType)bonusTypes[rand() % 4];
			}
			else if (i >= 6 && i <= 10)
			{
				int malusTypes[3] = { 2, 4, 6 };
				powerUpList[i].powerUpType = (PowerUpType)malusTypes[rand() % 3];
			}
		}
		int typeIndex = (int)powerUpList[i].powerUpType;
		sfSprite_setTextureRect(powerUpList[i].sprite, (sfIntRect) { 0, 16 * typeIndex, 16, 16 });
		sfSprite_setOrigin(powerUpList[i].sprite, (sfVector2f) { 8, 16 });
	}
}

void GeneratePowerUpPositions(sfVector2i _gridPos, int _index)
{
	powerUpList[_index].gridPos = _gridPos;
	sfSprite_setPosition(powerUpList[_index].sprite, TransformVector2iToVector2f(_gridPos));
}

void DestroyPowerUp(sfVector2i _gridPos)
{
	for (int i = 1; i < POWER_UP_COUNT; i++)
	{
		if (powerUpList[i].gridPos.x == _gridPos.x && powerUpList[i].gridPos.y == _gridPos.y)
		{
			powerUpList[i].gridPos = (sfVector2i){ -1,-1 };
			sfSprite_setPosition(powerUpList[i].sprite, (sfVector2f) { SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 });
			return;
		}
	}
}

sfBool LootPowerUpAndReturnIfExit(sfVector2i _gridPos)
{
	for (int i = 0; i < POWER_UP_COUNT; i++)
	{
		if (powerUpList[i].gridPos.x == _gridPos.x && powerUpList[i].gridPos.y == _gridPos.y)
		{
			sfSound_stop(booster);
			sfSound_play(booster);
			switch (powerUpList[i].powerUpType)
			{
			case FIRE_UP:
				if (GetIntFromSave(FIRE) < 8)
				{
					AddIntToSave(FIRE, 1);
				}
				break;
			case FULL_FIRE:
				SetIntToSave(FIRE, 8);
				break;
			case FIRE_DOWN:
				if (GetIntFromSave(FIRE) > 1)
				{
					AddIntToSave(FIRE, -1);
				}
				break;
			case SPEED_UP:
				if (GetIntFromSave(SPEED) < 8)
				{
					AddIntToSave(SPEED, 1);
				}
				break;
			case SPEED_DOWN:
				if (GetIntFromSave(SPEED) > 1)
				{
					AddIntToSave(SPEED, -1);
				}
				break;
			case BOMB_UP:
				if (GetIntFromSave(BOMB) < 8)
				{
					AddIntToSave(BOMB, 1);
				}
				break;
			case BOMB_DOWN:
				if (GetIntFromSave(BOMB) > 1)
				{
					AddIntToSave(BOMB, -1);
				}
				break;
			case EXIT:
				return sfTrue;
				break;
			default:
				break;
			}
			powerUpList[i].gridPos = (sfVector2i){ -1,-1 };
			sfSprite_setPosition(powerUpList[i].sprite, (sfVector2f) { SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 });
			return sfFalse;
		}
	}
	return sfFalse;
}

void SetExitIsAccessible(void)
{
	sfSprite_setTextureRect(powerUpList[0].sprite, (sfIntRect) { 16, 0, 16, 16 });
}