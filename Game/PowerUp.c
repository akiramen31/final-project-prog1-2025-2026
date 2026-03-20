#include "PowerUp.h"

sfTexture* powerUpTextureRef;

PowerUpEntity powerUpList[11];

void LoadPowerUp(void)
{ 
	powerUpTextureRef = GetAsset("Assets/Sprites/Items/Items.png");

}

void GeneratePowerUpPositions(int _targetArray[11], sfVector2i _gridPos)
{

    int count = 0;

    while (count < 11)
    {
        int num = (rand() % 40) + 1;
        sfBool alreadyExists = sfFalse;

        for (int i = 0; i < count; i++)
        {
            if (_targetArray[i] == num)
            {
                alreadyExists = sfTrue;
                break;
            }
        }

        if (!alreadyExists)
        {
            _targetArray[count] = num;
            powerUpList[count].gridPos = _gridPos;
            count++;
        }
    }
}

void CreatePowerUp(void)
{
    CleanupPowerUp();

    if (!powerUpTextureRef)
    {
        powerUpTextureRef = sfTexture_createFromFile("Assets/Sprites/Items/Items.png", NULL);
    }

    for (int i = 0; i < POWER_UP_COUNT; i++)
    {
        powerUpList[i].sprite = sfSprite_create();
        sfSprite_setTexture(powerUpList[i].sprite, powerUpTextureRef, sfTrue);
        sfSprite_setScale(powerUpList[i].sprite, (sfVector2f) { 4.f, 4.f });

        powerUpList[i].isVisible = sfFalse;
        powerUpList[i].isCollected = sfFalse;
        if (actualround == first)
        {
            if (i == 0) 
            {
                powerUpList[i].PowerUpType = CROWN;
            }
         else if (i >= 1 && i <= 2) 
            {
                powerUpList[i].PowerUpType = FIRE_UP;
            }
            else if (i >= 3 && i <= 4) 
            {
                powerUpList[i].PowerUpType = BOMB_UP;
            }
            else if (i == 5) 
            {
                powerUpList[i].PowerUpType = SPEED_UP;
            }
            else if (i >= 6 && i <= 10) 
            {
                int malusTypes[3] = { 2, 4, 6 };
                powerUpList[i].PowerUpType = (PowerUpType)malusTypes[rand() % 3];
            }
        }
        else
        {
            if (i == 0)
            {
                powerUpList[i].PowerUpType = CROWN;
            }
            else if (i >= 1 && i <= 5)
            {
                int bonusTypes[4] = { 0, 1, 3, 5 };
                powerUpList[i].PowerUpType = (PowerUpType)bonusTypes[rand() % 4];
            }
            else if (i >= 6 && i <= 10) 
            {
                int malusTypes[3] = { 2, 4, 6 };
                powerUpList[i].PowerUpType = (PowerUpType)malusTypes[rand() % 3];
            }
        }


        int typeIndex = (int)powerUpList[i].PowerUpType;
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
            return powerUpList[i].PowerUpType;
        }
    }
}