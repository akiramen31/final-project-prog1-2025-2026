#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"

#include "Player.h"
#include "Enemy.h"

void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(sfRenderWindow* _renderWindow, float _dt);
void DrawGame(sfRenderWindow* _renderWindow);
void CleanupGame(void);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !GAME_H
