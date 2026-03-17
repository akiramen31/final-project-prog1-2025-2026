#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
sfVector2u GetObjectUnstableCase(ObjectUnstable _object);
sfVector2f GetObjectUnstablePosition(ObjectUnstable _object);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"),(sfVector2f) {0}, 4.f, 2.f);
	LoadHUD();

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
	if (sfKeyboard_isKeyPressed(sfKeySpace)) {
		
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

sfVector2u GetObjectUnstableCase(ObjectUnstable _object)
{
	for (int row = 0; row < NB_GRID_ROW; row++)
	{
		for (int column = 0; column < NB_GRID_COLUMN; column++)
		{
			if (game.caseState[row][column].unstable = _object)
			{
				return (sfVector2u) {row, column};
			}
		}
	}
}

sfVector2f GetObjectUnstablePosition(ObjectUnstable _object)
{
	sfVector2u objectCase = GetObjectUnstableCase(_object);

	return (sfVector2f) { GRID_BOX.left + (float)objectCase.x / NB_GRID_COLUMN * GRID_BOX.width, GRID_BOX.top + (float)objectCase.y / NB_GRID_ROW * GRID_BOX.height};
}