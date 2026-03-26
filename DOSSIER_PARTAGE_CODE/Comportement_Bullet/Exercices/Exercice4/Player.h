#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "Bullet.h"

#define PLAYER_SPEED 400
#define FIRE_RATE 100

typedef struct Player
{
	sfTexture* texture;
	sfSprite* sprite;
	sfBool canShoot;
	float cooldown;
}Player;

void LoadPlayer(void);
void UpdatePlayer(sfRenderWindow* _renderWindow, float _dt);
void DrawPlayer(sfRenderWindow* const _renderWindow);
void CleanupPlayer(void);

#endif // !PLAYER_H
