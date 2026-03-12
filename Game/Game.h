#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "GameState.h"

void LoadGame(void);
void PollEventGame(sfRenderWindow* _renderWindow);
void UpdateGame(float _dt);

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent);

#endif // !GAME_H
