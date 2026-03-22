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
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"), (sfVector2f) { 0 }, 4.f, 40.f);

	sfMusic* gameMusic = CreateMusic("Assets/Musics/Game-Music.ogg", GAME_VOLUME, sfFalse);
	sfMusic_setLoop(gameMusic, sfTrue);
	sfMusic_play(gameMusic);
	LoadHUD();
	LoadPlayer();
	LoadBomb();
	timer = 180.f;
	LoadBox();
	LoadEnnemy();
	LoadPowerUp();

	sfVector2i positionRandom = { 0 , 0 };
	for (int i = 0; i < NB_BOX; i++)
	{
		do
		{
			positionRandom = (sfVector2i){ rand() % NB_GRID_COLUMN, rand() % NB_GRID_ROW };
		} while (positionRandom.x % 2 && positionRandom.y % 2 || GetIfBoxIsHere(positionRandom) || (positionRandom.y + positionRandom.x) < 2);

		if (i < 11)
		{
			GeneratePowerUpPositions(positionRandom, i);
		}
		SetBoxPosition(positionRandom, i);
	}

	CasePosibility casePosibility = { 0 };
	for (int i = 0; i < GetIntFromSave(ENNEMY_COUNT); i++)
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

void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		return;
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
}

void UpdateGame(float _dt)
{
	UpdatePlayer(GetMovePosibility(GetPlayerPositionGrid()), _dt);

	for (int i = 0; i < GetEnnemyCount(); i++)
	{
		UpdateEnnemy(_dt, GetMovePosibility(GetEnnemyNextPosition(i)), i, GetPlayerPositionGrid());
	}

	UpdateBox(_dt);

	CasePosibility casePosibilityBomb[NUM_MAX_BOMB] = { 0 };
	for (int i = 0; i < GetBombCount(); i++)
	{
		casePosibilityBomb[i] = GetMovePosibility(GetBombPositionGrid(i));

	}
	UpdateBomb(casePosibilityBomb, _dt);

	timer -= _dt;
	if (timer < 0.f)
	{
		SetGameState(GAME_OVER);
		return;
	}

	UpdatePlayer(GetMovePosibility(GetPlayerPositionGrid()), _dt);
	UpdateHUD(_dt, timer);

	UpdateCollider();
}

CasePosibility GetMovePosibility(sfVector2i _position)
{
	CasePosibility posibility = { 0 };
	int radiusExplosion = GetIntFromSave(FIRE);

	sfVector2i positionTemp = _position;
	if (!(_position.y % 2))
	{
		for (int i = 1; i < radiusExplosion + 1; i++)
		{
			positionTemp.x--;
			if (GetIfBoxIsHere(positionTemp))
			{
				posibility.leftBox = sfTrue;
				i = radiusExplosion + 1;
			}
			else if (_position.x - i >= 0 && !CheckAtLocationBomb(positionTemp))
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
			if (GetIfBoxIsHere(positionTemp))
			{
				posibility.rightBox = sfTrue;
				i = radiusExplosion + 1;
			}
			else if (_position.x + i < NB_GRID_COLUMN && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
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
			if (GetIfBoxIsHere(positionTemp))
			{
				posibility.upBox = sfTrue;
				i = radiusExplosion + 1;
			}
			else if (_position.y - i >= 0 && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
			{
				posibility.up++;
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
			if (GetIfBoxIsHere(positionTemp))
			{
				posibility.downBox = sfTrue;
				i = radiusExplosion + 1;
			}
			else if (_position.y + i < NB_GRID_ROW && !GetIfBoxIsHere(positionTemp) && !CheckAtLocationBomb(positionTemp))
			{
				posibility.down++;
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
	for (int j = 0; j < GetDeflagrationCount(); j++)
	{
		sfVector2i deflagrationPosition = GetDeflagrationPosition(j);
		if (GetIfBoxIsHere(deflagrationPosition))
		{
			DestroyBox(deflagrationPosition);
		}
		else
		{
			DestroyPowerUp(deflagrationPosition);
		}
	}

	for (int i = 0; i < GetEnnemyCount(); i++)
	{
		sfVector2i ennemyPosition = GetEnnemyPosition(i);
		sfVector2i ennemyPositionNext = GetEnnemyNextPosition(i);
		for (int j = 0; j < GetDeflagrationCount(); j++)
		{
			sfVector2i deflagrationPosition = GetDeflagrationPosition(j);
			if (deflagrationPosition.x == ennemyPosition.x && deflagrationPosition.y == ennemyPosition.y || deflagrationPosition.x == ennemyPositionNext.x && deflagrationPosition.y == ennemyPositionNext.y)
			{
				HitEnnemy(i);
				j = GetDeflagrationCount();
			}
		}
	}

	sfVector2i playerPosition = GetPlayerPositionGrid();
	if (GetEnnemyCount() <= 0)
	{
		SetExitIsAccessible();
	}

	if (LootPowerUpAndReturnIfExit(playerPosition) && GetEnnemyCount() <= 0 )
	{
		SetGameState(GAME);
		return;
	}

	if (!AskPlayerInvincible())
	{
		for (int i = GetEnnemyCount() - 1; i >= 0; i--)
		{
			sfVector2i ennemyPosition = GetEnnemyPosition(i);
			sfVector2i ennemyPositionNext = GetEnnemyNextPosition(i);
			if (playerPosition.x == ennemyPosition.x && playerPosition.y == ennemyPosition.y || playerPosition.x == ennemyPositionNext.x && playerPosition.y == ennemyPositionNext.y)
			{
				KillPlayer();
				if (GetIntFromSave(LIFE) <= 0)
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
				if (GetIntFromSave(LIFE) <= 0)
				{
					SetGameState(GAME_OVER);
				}
				return;
			}
		}
	}
}