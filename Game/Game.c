#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
MovePosibility GetMovePosibility(sfVector2i _position);
void UpdateCollider(void);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"),(sfVector2f) {0}, 4.f, 2.f);
	LoadHUD();
	LoadPlayer();
	sfMusic* gameMusic = CreateMusic("Assets/Musics/Game-Music.ogg", 10.f, sfFalse);
	sfMusic_setLoop(gameMusic, sfTrue);
	sfMusic_play(gameMusic);

	LoadBox();
	for (int row = 1; row < NB_GRID_ROW; row += 2)
	{
		for (int column = 1; column < NB_GRID_COLUMN; column += 2)
		{
			game.caseState[row][column] = WALL;
		}
	}

	for (int i = 0; i < NB_BOX; i++)
	{
		
		SetBoxPosition((sfVector2i) {rand() % 4, rand() % 4 }, i);
	}

	LoadEnnemy();
	AddEnnemy();
	AddEnnemy();
	AddEnnemy();
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
	UpdatePlayer(GetMovePosibility(GetPlayerPositionGrid()), _dt);
	UpdateEnnemy(_dt);
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

MovePosibility GetMovePosibility(sfVector2i _position)
{
	MovePosibility posibility = { 0 };

	if (!(_position.y % 2))
	{
		if (_position.x > 0)
		{
			if (game.caseState[_position.y][_position.x - 1] != BOX)
			{
				posibility.left = sfTrue;
			}
		}

		if (_position.x < (NB_GRID_COLUMN - 1))
		{
			if (game.caseState[_position.y][_position.x + 1] != BOX)
			{
				posibility.right = sfTrue;
			}
		}
	}

	if (!(_position.x % 2))
	{
		if (_position.y > 0)
		{
			if (game.caseState[_position.y - 1][_position.x] != BOX)
			{
				posibility.up = sfTrue;
			}
		}

		if (_position.y < (NB_GRID_ROW - 1))
		{
			if (game.caseState[_position.y + 1][_position.x] != BOX)
			{
				posibility.down = sfTrue;
			}
		}
	}
	return posibility;
}

void UpdateCollider(void)
{

}
