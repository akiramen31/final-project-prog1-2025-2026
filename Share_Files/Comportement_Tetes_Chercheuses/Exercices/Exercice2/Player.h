#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"

#define PLAYER_RADIUS 25

typedef struct Player
{
	sfCircleShape* circle;
	float speed;
	sfVertexArray* lineOfSight;
}Player;

void LoadPlayer(void);
void UpdatePlayer(sfRenderWindow* const _renderWindow, float _dt);
void DrawPlayer(sfRenderWindow* const _renderWindow);
void CleanupPlayer(void);

sfVector2f GetPlayerPosition(void);
void UpdatePlayerLineOfSight(sfVector2f _targetPosition);

#endif // !PLAYER_H
