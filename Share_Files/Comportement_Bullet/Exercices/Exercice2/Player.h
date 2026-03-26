#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include"Bullet.h"
#include "Enemy.h"

#define PLAYER_RADIUS 20
#define FIRE_RATE 5

typedef struct Player
{
	sfCircleShape* circle;
	sfRectangleShape* rect;
	float speed;
	sfVertexArray* lineOfSight;
	sfBool canShoot;
	float cooldown;
}Player;


void LoadPlayer(void);
void UpdatePlayer(float _dt, sfVector2f _start, sfVector2f _target);
void DrawPlayer(sfRenderWindow* const _renderWindow);
void CleanupPlayer(void);

sfVector2f GetPlayerPosition(void);
void UpdatePlayerLineOfSight(sfVector2f _targetPosition);

#endif // !PLAYER_H
