#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
CasePosibility GetMovePosibility(sfVector2i _position);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"), (sfVector2f) { 0 }, 4.f, 2.f);

	sfMusic* gameMusic = CreateMusic("Assets/Musics/Game-Music.ogg", 10.f, sfFalse);
	sfMusic_setLoop(gameMusic, sfTrue);
	sfMusic_play(gameMusic);
	LoadHUD();
	LoadPlayer();
	LoadBomb();
	timer = 180.f;
	LoadBox();
	LoadEnnemy();

	sfVector2i positionRandom = { 0 , 0 };
	for (int i = 0; i < NB_BOX; i++)
	{
		do
		{
			positionRandom = (sfVector2i){ rand() % NB_GRID_COLUMN, rand() % NB_GRID_ROW };
		} while (positionRandom.x % 2 && positionRandom.y % 2 || GetIfBoxIsHere(positionRandom) || (positionRandom.y + positionRandom.x) < 2);
		SetBoxPosition(positionRandom, i);
	}

	CasePosibility casePosibility = { 0 };
	for (int i = 0; i < GetIntToSave(ENNEMY_COUNT); i++)
	{
		do
		{
			positionRandom = (sfVector2i){ rand() % NB_GRID_COLUMN, rand() % NB_GRID_ROW };
			casePosibility = GetMovePosibility(positionRandom);
		} while (positionRandom.x % 2 && positionRandom.y % 2 || GetIfBoxIsHere(positionRandom) || (positionRandom.y + positionRandom.x) < 7 || !(casePosibility.left + casePosibility.down + casePosibility.right + casePosibility.up));
		AddEnnemy(positionRandom, casePosibility);
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

	for (int i = 0; i < GetEnnemyCount(); i++)
	{
		UpdateEnnemy(_dt, GetMovePosibility(GetEnnemyNextPosition(i)), i);
	}

	UpdateBox(_dt);

	CasePosibility casePosibilityBomb[NUM_MAX_BOMB] = { 0 };
	for (int i = 0; i < GetBombCount(); i++)
	{
		casePosibilityBomb[i] = GetMovePosibility(GetBombPositionGrid(i));

	}
	UpdateBomb(casePosibilityBomb, _dt);

	UpdateCollider();
	timer -= _dt;
	UpdateHUD(_dt, timer);
}

void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeySpace:
		if (AskPlayerIdle())
		{
			SpawnBomb(GetPlayerPositionGrid());
		}
		break;
	default:
		break;
	}

	if (DEV_MODE)
	{
		switch (_keyEvent->code)
		{
		case sfKeyEnter:
			SetGameState(GAME_OVER);
			break;
		case sfKeyE:
			DestroyBox((sfVector2i) { 2, 2 });
			break;
		default:
			break;
		}
	}
}

CasePosibility GetMovePosibility(sfVector2i _position)
{
	CasePosibility posibility = { 0 };
	int radiusExplosion = GetIntToSave(FIRE);

	sfVector2i positionTemp = _position;
	if (!(_position.y % 2))
	{
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			positionTemp.x--;
			if (_position.x - i >= 0 && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
			{
				posibility.left++;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
		positionTemp = _position;
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			positionTemp.x++;
			if (_position.x + i < NB_GRID_COLUMN && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
			{
				posibility.right++;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
	}

	if (!(_position.x % 2))
	{
		positionTemp = _position;
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			positionTemp.y--;
			if (_position.y - i >= 0 && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
			{
				posibility.up = i;
			}
			else
			{
				i = radiusExplosion + 1;
			}
		}
		positionTemp = _position;
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			positionTemp.y++;
			if (_position.y + i < NB_GRID_ROW && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
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
	for (int i = 0; i < GetEnnemyCount(); i++)
	{
		sfVector2i ennemyPosition = GetEnnemyPosition(i);
		sfVector2i ennemyPositionNext = GetEnnemyNextPosition(i);
		for (int j = 0; j < GetDeflagrationCount(); j++)
		{
			sfVector2i deflagrationPosition = GetDeflagrationPosition(j);
			if (deflagrationPosition.x == ennemyPosition.x && deflagrationPosition.y == ennemyPosition.y || deflagrationPosition.x == ennemyPositionNext.x && deflagrationPosition.y == ennemyPositionNext.y)
			{
				printf("ok");
				HitEnnemy(i);
				j = GetDeflagrationCount();
			}
		}
	}

	if (!AskPlayerInvincible())
	{
		sfVector2i playerPosition = GetPlayerPositionGrid();
		for (int i = GetEnnemyCount() - 1; i >= 0; i--)
		{
			sfVector2i ennemyPosition = GetEnnemyPosition(i);
			sfVector2i ennemyPositionNext = GetEnnemyNextPosition(i);
			if (playerPosition.x == ennemyPosition.x && playerPosition.y == ennemyPosition.y || playerPosition.x == ennemyPositionNext.x && playerPosition.y == ennemyPositionNext.y)
			{
				KillPlayer();
				if (GetIntToSave(LIFE) <= 0)
				{
					SetGameState(GAME_OVER);
				}
				return;
			}
		}

		for (int i = 0; i < GetDeflagrationCount(); i++)
		{
			sfVector2i deflagrationPosition = GetDeflagrationPosition(i);
			if (playerPosition.x == deflagrationPosition.x && playerPosition.y == deflagrationPosition.y)
			{
				KillPlayer();
				if (GetIntToSave(LIFE) <= 0)
				{
					SetGameState(GAME_OVER);
				}
				return;
			}
		}
	}
}