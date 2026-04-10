#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"

#include "Player.h"
#include "Aim.h"
#include "Missile.h"
#include "Enemy.h"

void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(sfRenderWindow* _renderWindow, float _dt);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);
sfBool CheckCollisionAxeEnemy(sfRectangleShape* _axe, sfCircleShape* _enemy);
sfBool IsEnemyInMeleeRange(sfRectangleShape* _axe, sfCircleShape* _enemy);

#endif // !GAME_H
