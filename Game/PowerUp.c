#include "PowerUp.h"

sfTexture* powerUpTextureRef;

PowerUpEntity powerUpList[11];

void LoadPowerUp(void)
{ 
	powerUpTextureRef = GetAsset("Assets/Sprites/Items/Items.png");

}

void GeneratePowerUpPositions(sfVector2i _gridPos, int _index)
{
    powerUpList[_index].gridPos = _gridPos;
}

PowerUpType CheckPowerUpType(int _index)
{
    return powerUpList[_index].powerUpType;
}


void CreatePowerUp(void)
{
    CleanupPowerUp();

    if (!powerUpTextureRef)
    {
        powerUpTextureRef = sfTexture_createFromFile("Assets/Sprites/Items/Items.png", NULL);
    }

    int round = GetIntFromSave(ROUND);

    for (int i = 0; i < POWER_UP_COUNT; i++)
    {
        powerUpList[i].sprite = sfSprite_create();
        sfSprite_setTexture(powerUpList[i].sprite, powerUpTextureRef, sfTrue);
        sfSprite_setScale(powerUpList[i].sprite, (sfVector2f) { 4.f, 4.f });

        powerUpList[i].isVisible = sfFalse;
        powerUpList[i].isCollected = sfFalse;
        if (round == 1)
        {
            if (i == 0) 
            {
                powerUpList[i].powerUpType = CROWN;
            }
         else if (i >= 1 && i <= 2) 
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
            if (i == 0)
            {
                powerUpList[i].powerUpType = CROWN;
            }
            else if (i >= 1 && i <= 5)
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
    }
}

void DropPowerUp(sfVector2i _gridPos)
{
    int index = -1;
    for (int i = 0; i <= 10; i++)
    {
        if (powerUpList[i].gridPos.x == _gridPos.x && powerUpList[i].gridPos.y == _gridPos.y)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        powerUpList[index].isVisible = sfTrue;
        return;
    }
    else
    {
        printf("Error Power Up not found\n");
        return;
    }
}

void DrawPowerUp(sfRenderWindow* _renderWindow)
{
    for (int i = 0; i < POWER_UP_COUNT; i++)
    {
        if (powerUpList[i].isVisible == sfTrue && powerUpList[i].isCollected == sfFalse)
        {
            sfRenderWindow_drawSprite(_renderWindow, powerUpList[i].sprite, NULL);
        }
    }
}

void DestructionPowerUp(sfVector2i _gridPos)
{
    for (int i = 0; i <= POWER_UP_COUNT; i++)
    {
        if (powerUpList[i].gridPos.x == _gridPos.x && powerUpList[i].gridPos.y == _gridPos.y)
        {
            if (powerUpList[i].isVisible == sfTrue)
            {
            powerUpList[i].isVisible = sfFalse;
            // Become unaccesible
            powerUpList[i].isCollected = sfTrue; 
            break;
            }
        }
    }
}

void CleanupPowerUp(void)
{
    if (powerUpList != NULL) {
        for (int i = 0; i < POWER_UP_COUNT; i++) {
            if (powerUpList[i].sprite != NULL) {
                sfSprite_destroy(powerUpList[i].sprite);
            }
        }
        
    }
}

PowerUpType IsPowerUpType(sfVector2i _gridPos)
{
    for (int i = 0; i <= POWER_UP_COUNT; i++)
    {
        if (powerUpList[i].gridPos.x == _gridPos.x && powerUpList[i].gridPos.y == _gridPos.y)
        {
            return powerUpList[i].powerUpType;
        }
    }
}