#ifndef POWERUP_H
#define POWERUP_H

#include "Common.h"

#define MAX_BOMB 8
#define POWER_UP_COUNT 11

#define TIMER_POWERUP 60

#define CROWNCOUNT 1

typedef enum PowerUpType {
    FIRE_UP,
    FULL_FIRE,
    FIRE_DOWN,
    SPEED_UP,
    SPEED_DOWN,
    BOMB_UP,
    BOMB_DOWN,
    CROWN
}PowerUpType;

typedef struct PowerUpEntity{
    sfSprite* sprite;
    sfVector2i gridPos;
    sfBool isVisible;
    sfBool isCollected;
    PowerUpType PowerUpType;
} PowerUpEntity;

void LoadPowerUp(void);
void CreatePowerUp(void);
void CleanupPowerUp(void);

void DestructionPowerUp(sfVector2i _gridPos);
void GeneratePowerUpPositions(int _targetArray[11], sfVector2i _gridPos);
void DropPowerUp(sfVector2i _gridPos);
void DrawPowerUp(sfRenderWindow* _renderWindow);

#endif