#include "GameState.h"

GameState gameState = -1;

GameState GetGameState(void)
{
    return gameState;
}

void SetGameState(GameState _gameState)
{
    CleanupTempEntity();

    gameState = _gameState;

    switch (gameState)
    {
    case MENU:
        LoadMenu();
        break;
    case GAME:
        LoadGame();
        break;
    case GAME_OVER:
        LoadGameOver();
        break;
    default:
        break;
    }
}
