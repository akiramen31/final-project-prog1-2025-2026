#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
CasePosibility GetMovePosibility(sfVector2i _position);
void UpdateCollider(void);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"), (sfVector2f) { 0 }, 4.f, 2.f);
	LoadHUD();
	LoadPlayer();
	LoadBomb();

	sfMusic* gameMusic = CreateMusic("Assets/Musics/Game-Music.ogg", 10.f, sfFalse);
	sfMusic_setLoop(gameMusic, sfTrue);
	sfMusic_play(gameMusic);

	for (int row = 1; row < NB_GRID_ROW; row += 2)
	{
		for (int column = 1; column < NB_GRID_COLUMN; column += 2)
		{
			game.caseState[row][column] = WALL;
		}
	}

	LoadBox();
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
	CasePosibility casePosibility = { 0 };
	for (int i = 0; i < GetIntToSave(ENNEMY_COUNT); i++)
	{
		do
		{
			positionRandom = (sfVector2i){ rand() % NB_GRID_COLUMN, rand() % NB_GRID_ROW };
			casePosibility = GetMovePosibility(positionRandom);
		} while (game.caseState[positionRandom.y][positionRandom.x] != 0 || (positionRandom.y + positionRandom.x) < 7 || !(casePosibility.left + casePosibility.down + casePosibility.right + casePosibility.up));
		//AddEnnemy(positionRandom, casePosibility);
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
	if (sfKeyboard_isKeyPressed(sfKeySpace))
	{
		if (AskPlayerIdle())
		{
			SpawnBomb(GetPlayerPositionGrid());
		}
	}

	UpdatePlayer(GetMovePosibility(GetPlayerPositionGrid()), _dt);
	for (int i = 0; i < GetNumberEnnemy(); i++)
	{
		UpdateEnnemy(_dt, GetMovePosibility(GetPositionEnnemy(i)), i);
	}
	UpdateBox(_dt);

	CasePosibility casePosibilityBomb[NUM_MAX_BOMB] = { 0 };
	for (int i = 0; i < GetBombCount(); i++)
	{
		casePosibilityBomb[i] = GetMovePosibility(GetBombPositionGrid(i));
	}
	UpdateBomb(&casePosibilityBomb[0], _dt);
}

void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeyEnter:
		if (DEV_MODE)
		{
			SetGameState(GAME_OVER);
		}
		break;
	case sfKeyE:
		if (DEV_MODE)
		{
			game.caseState[2][2] = VOID;
			DestroyBox((sfVector2i) { 2, 2 });
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
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			if (_position.x - i >= 0 && game.caseState[_position.y][_position.x - i] != BOX)
			{
				posibility.left = i;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			if (_position.x + i < NB_GRID_COLUMN && game.caseState[_position.y][_position.x + i] != BOX)
			{
				posibility.right = i;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
	}

	if (!(_position.x % 2))
	{
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			if (_position.y - i >= 0 && game.caseState[_position.y - i][_position.x] != BOX)
			{
				posibility.up = i;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			if (_position.y + i < NB_GRID_ROW && game.caseState[_position.y + i][_position.x] != BOX)
			{
				posibility.down = i;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
	}
	return posibility;
}

void UpdateCollider(void)
{

}
