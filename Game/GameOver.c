#include "GameOver.h"
#include "Player.h"
#include "Weapons.h"

GameOver gameOver;

int score;
int tempScore;
int highScore;
int tempHighScore;

void KeyPressedGameOver(sfEvent* _event);
void MouseButtonPressedGameOver(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedGameOver(sfMouseMoveEvent* _mouseMovedEvent);

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	score = 0;
	tempScore = 0;
	highScore = 0;
	tempHighScore = 0;
	if (GetPlayerLife() > 0)
	{
		gameOver.didPlayerLoose = 0;
	}
	else
	{
		gameOver.didPlayerLoose = 1;
	}
	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	if (gameOver.didPlayerLoose)
	{
		gameOver.background = LoadBackground(GetAsset("Assets/Sprites/game_over_loose.png"), 8.f);
		gameOver.frameTime[0] = 1.f;
		gameOver.frameTime[1] = 0.2f;
		gameOver.frameTime[2] = 0.1f;
		gameOver.frameTime[3] = 0.15f;
		gameOver.frameTime[4] = 0.5f;
		gameOver.frameTime[5] = 0.15f;
		gameOver.frameTime[6] = 0.1f;
		gameOver.frameTime[7] = 0.15f;
		gameOver.frameTime[8] = 0.1f;
		gameOver.frameTime[9] = 0.15f;
		gameOver.frameTime[10] = 0.7f;
		gameOver.frameTime[11] = 0.2f;
		gameOver.frameTime[12] = 0.15f;
		gameOver.frameTime[13] = 0.1f;
		gameOver.frameTime[14] = 0.4f;
		gameOver.frameTime[15] = 0.1f;
		gameOver.frameTime[16] = 0.6f;
		gameOver.frameTime[17] = 0.1f;
		gameOver.frameTime[18] = 0.15f;
		gameOver.frameTime[19] = 0.2f;
		for (int i = 0; i < ANIM_FRAME_COUNT; i++)
		{
			gameOver.frameRect[i] = (sfIntRect){ 0,135 * i,240,135 };
		}
		gameOver.currentFrame = 0;
	}
	else
	{
		gameOver.background = LoadBackground(GetAsset("Assets/Sprites/game_over_win.png"), 8.f);
	}
	sfFont* font = GetAsset(FONT);

	float multiplier = 1.f;
	switch (GetSecondaryType())
	{
	case NO_SECONDARY:
		multiplier += 0.3f;
		break;
	case DRONE:
		break;
	case COLDBREATH:
		multiplier += 0.1f;
		break;
	}
	switch (GetWeapon().weaponType)
	{
	case NO_WEAPON:
		multiplier += 0.7f;
		break;
	case RAILGUN:
		multiplier += 0.2;
		break;
	case STEAMAXE:
		multiplier += 0.4f;
		break;
	case MISTEEL:
		break;
	}

	score = GetIntFromSave(CURRENT_SCORE)*multiplier;

	MapState map = GetCurrentMap();
	GameData* gameData = GetGameData();

	if (gameData->score[map] < score)
	{
		gameData->score[map] = score;
		highScore = score;
	}
	else
	{
		highScore = gameData->score[map];
	}

	gameOver.button[2] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 13 }, 1.f, 10);
	sfText_setColor(gameOver.button[2], COLOR_ORANGE);

	if (!gameOver.didPlayerLoose)
	{
		sfText_setCharacterSize(gameOver.button[2], 60);
		sfText_setString(gameOver.button[2], "CONTINUE");
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.button[2]);
		sfText_setOrigin(gameOver.button[2], (sfVector2f) { rect.width / 2, rect.height });

		if (gameData->levelUnlock == map + 1)
		{
			gameData->levelUnlock++;
		}
	}
	SaveGameData();

#pragma region button
	for (int i = 0; i < 2; i++)
	{
		gameOver.button[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 8 * ((i * 6) + 1), (SCREEN_HEIGHT / 8) * 3 }, 1.f, 10);
		sfText_setCharacterSize(gameOver.button[i], 60);
		sfText_setColor(gameOver.button[i], (sfColor) { 255, 165, 0, 255 });
	}

	sfText_setString(gameOver.button[0], "MENU");
	sfText_setString(gameOver.button[1], "RETRY");

	for (int i = 0; i < 2; i++)
	{
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.button[i]);
		sfText_setOrigin(gameOver.button[i], (sfVector2f) { rect.width / 2, rect.height });
	}
#pragma endregion
	gameOver.text = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 8) * 5 }, 1.f, 10);
	sfText_setCharacterSize(gameOver.text, 60);
	if (gameOver.didPlayerLoose)
	{
		sfText_setColor(gameOver.text, (sfColor) { 114, 28, 29, 255 });
		sfText_setString(gameOver.text, "YOU DIED");
	}
	else
	{
		sfText_setColor(gameOver.text, (sfColor) { 60, 255, 120, 255 });
		sfText_setString(gameOver.text, "YOU PASSED TO THE NEXT LEVEL");
	}
	sfFloatRect rect = sfText_getGlobalBounds(gameOver.text);
	sfText_setOrigin(gameOver.text, (sfVector2f) { rect.width / 2, rect.height });


#pragma region score
	int decal = 60;

	for (int i = 0; i < 2; i++)
	{
		gameOver.score[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2 - decal, SCREEN_HEIGHT / 10 * (1 + i) }, 1.f, 10);
		sfText_setCharacterSize(gameOver.score[i], 60);
	}

	sfText_setString(gameOver.score[0], "SCORE");
	sfText_setString(gameOver.score[1], "HIGH SCORE");

	for (int i = 0; i < 2; i++)
	{
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.score[i]);
		sfText_setOrigin(gameOver.score[i], (sfVector2f) { rect.width, rect.height });
	}


	for (int i = 2; i < 4; i++)
	{
		gameOver.score[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2 + decal, SCREEN_HEIGHT / 10 * (i - 1) }, 1.f, 10);
		sfText_setCharacterSize(gameOver.score[i], 60);
	}

	char* text[50];
	sprintf_s(text, sizeof(text), "%07d", 0);
	sfText_setString(gameOver.score[2], text);
	sprintf_s(text, sizeof(text), "%07d", 0);
	sfText_setString(gameOver.score[3], text);

	for (int i = 2; i < 4; i++)
	{
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.score[i]);
		sfText_setOrigin(gameOver.score[i], (sfVector2f) { 0, rect.height });
	}
#pragma endregion
}

void PollEventGameOver(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGameOver(_event);
		break;
	case sfEvtMouseButtonPressed:
		MouseButtonPressedGameOver(&_event->mouseButton);
		break;
	case sfEvtMouseMoved:
		MouseMovedGameOver(&_event->mouseMove);
		break;
	default:
		break;
	}
}

void KeyPressedGameOver(sfEvent* _event)
{
	if (tempHighScore == highScore && tempScore == score)
	{
		switch (_event->key.code)
		{
		case sfKeyEscape:
			SetGameState(MENU);
			return;
		case sfKeyEnter:
			SetGameState(GAME);
			return;
		default:
			break;
		}
	}
	switch (_event->key.code)
	{
	case sfKeySpace:
		tempHighScore = highScore;
		tempScore = score;
		return;
	default:
		break;
	}
}

void MouseButtonPressedGameOver(sfMouseButtonEvent* _mouseButtonEvent)
{
	switch (_mouseButtonEvent->button)
	{
	case sfMouseLeft:
		for (int i = 0; i < NB_BOTTON_GAMEOVER; i++)
		{
			if (CompareColor(sfText_getColor(gameOver.button[i]), sfWhite))
			{
				if (i == 0)
				{
					SetGameState(MENU);
				}
				else if (i == 1)
				{
					SetGameState(MENU_SELECTION_GAME);
				}
				else if (i == 2)
				{
					int temp = GetCurrentMap();
					if (temp != 3)
					{
						SetCurrentMap(temp + 1);
					}
					SetGameState(MENU_SELECTION_GAME);
				}
				return;
			}
		}
		break;
	default:
		break;
	}
}

void MouseMovedGameOver(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfVector2f mouse = { _mouseMovedEvent->x,_mouseMovedEvent->y };

	for (int i = 0; i < NB_BOTTON_GAMEOVER; i++)
	{
		UpdateTextHighlightTextColor(gameOver.button[i], COLOR_ORANGE, sfWhite, mouse);
	}
}

void UpdateGameOver(float _dt)
{
	tempScore += RAND_RANGE(1, 100);
	if (tempScore > score)
	{
		tempScore = score;
	}

	tempHighScore += RAND_RANGE(1, 100);
	if (tempHighScore > highScore)
	{
		tempHighScore = highScore;
	}

	char* text[50];
	sprintf_s(text, sizeof(text), "%07d", tempScore);
	sfText_setString(gameOver.score[2], text);
	sprintf_s(text, sizeof(text), "%07d", tempHighScore);
	sfText_setString(gameOver.score[3], text);

	if (gameOver.didPlayerLoose)
	{
		gameOver.timerAnim += _dt;
		if (gameOver.timerAnim >= gameOver.frameTime[gameOver.currentFrame])
		{
			gameOver.timerAnim = 0;
			if (gameOver.currentFrame == 19)
			{
				gameOver.currentFrame = 0;
			}
			else
			{
				gameOver.currentFrame++;
			}
			sfSprite_setTextureRect(gameOver.background, gameOver.frameRect[gameOver.currentFrame]);
		}
	}
}
