#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
CasePosibility GetMovePosibility(sfVector2i _position);
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
	sfVector2i positionRandom = { 0 , 0 };
	for (int i = 0; i < NB_BOX; i++)
	{
		do
		{
			positionRandom = (sfVector2i){ rand() % NB_GRID_COLUMN, rand() % NB_GRID_ROW };
		} while (game.caseState[positionRandom.y][positionRandom.x] != 0 || (positionRandom.y + positionRandom.x) < 2);

		SetBoxPosition(positionRandom, i);
		game.caseState[positionRandom.y][positionRandom.x] = BOX;
	}

	LoadEnnemy();
	AddEnnemy((sfVector2i){100, 100});
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
	UpdateBox(_dt);
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
	case sfKeyE:
		if (DEV_MODE)
		{
			game.caseState[2][2] = VOID;
			DestroyBox((sfVector2i) {2, 2});
		}
		break;
	default:
		break;
	}
}

CasePosibility GetMovePosibility(sfVector2i _position)
{
	CasePosibility posibility = { 0 };
	int radiusExplosion = GetIntToSave(FIRE);

	if (!(_position.y % 2))
	{
		if (_position.x > 0)
		{
			for (int i = 1; i < radiusExplosion + 1; i++)
			{
				if (game.caseState[_position.y][_position.x - i] != BOX)
				{
					posibility.left = i;
				}
			}
		}
		if (_position.x < NB_GRID_COLUMN - 1)
		{
			for (int i = 1; i < radiusExplosion + 1; i++)
			{
				if (game.caseState[_position.y][_position.x + i] != BOX)
				{
					posibility.right++;
				}
			}
		}
	}

	if (!(_position.x % 2))
	{
		if (_position.y > 1)
		{
			for (int i = 1; i < radiusExplosion + 1; i++)
			{
				if (game.caseState[_position.y - i][_position.x] != BOX)
				{
					posibility.up++;
				}
			}
		}
		if (_position.y < NB_GRID_ROW - 1)
		{
			for (int i = 1; i < radiusExplosion + 1; i++)
			{
				if (game.caseState[_position.y + i][_position.x] != BOX)
				{
					posibility.down++;
				}
			}
		}
	}
	printf("left %d right %d up %d down %d\n", posibility.left, posibility.right, posibility.up, posibility.down);
	return posibility;
}

void UpdateCollider(void)
{

}
