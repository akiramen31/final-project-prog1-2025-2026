#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"),(sfVector2f) {0}, 4.f, 2.f);
	LoadHUD();
	LoadPlayer();
	game.caseState[0][0].unstable = PLAYER;

	for (int row = 0; row < NB_GRID_ROW; row++)
	{
		for (int column = 0; column < NB_GRID_COLUMN; column++)
		{
			if (row % 2 && column % 2)
			{
				game.caseState[row][column].stable = WALL;
			}
		}
	}
}

void PollEventGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGame(&_event->key);
		break;
	default:
		break;
	}
}

void UpdateGame(float _dt)
{


	if (sfKeyboard_isKeyPressed(sfKeyUp) && !sfKeyboard_isKeyPressed(sfKeyDown))
	{
		
	}
	if (!sfKeyboard_isKeyPressed(sfKeyUp) && sfKeyboard_isKeyPressed(sfKeyDown))
	{

	}
	if (sfKeyboard_isKeyPressed(sfKeyUp) && !sfKeyboard_isKeyPressed(sfKeyDown))
	{

	}
	if (!sfKeyboard_isKeyPressed(sfKeyUp) && sfKeyboard_isKeyPressed(sfKeyDown))
	{

	}
}



void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeySpace:
		if (DEV_MODE)
		{
			SetGameState(GAME_OVER);
		}
		break;
	default:
		break;
	}
}