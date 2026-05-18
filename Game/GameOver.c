#include "GameOver.h"
#include "Player.h"

GameOver gameOver;

int score = 5000;
int tempScore = 0;
int highScore = 10000;
int tempHighScore = 0;

void KeyPressedGameOver(sfEvent* _event);
void MouseButtonPressedGameOver(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedGameOver(sfMouseMoveEvent* _mouseMovedEvent);

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	gameOver.backgroundAnim = CreateSprite(GetAsset("Assets/Sprites/game_over_background.png"), (sfVector2f) { 0 }, 8.f, 1000);
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

	sfFont* font = GetAsset(FONT);

	int live = GetPlayerLife();

	score = GetIntFromSave(CURRENT_SCORE);
	MapState map = GetCurrentMap();
	GameData* gameData = GetGameData();
	if (highScore < gameData->score[map])
	{
		highScore = gameData->score[map] = score;
	}
	else
	{
		highScore = gameData->score[map];
	}
	if (gameData->levelUnlock == map + 1)
	{
		gameData->levelUnlock++;
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

	if (GetPlayerLife() > 0)
	{
		sfText_setColor(gameOver.text, (sfColor) { 54, 80, 52, 255 });
		sfText_setString(gameOver.text, "YOU PASSED TO THE NEXT LEVEL");
	}
	else
	{
		sfText_setColor(gameOver.text, (sfColor) { 114, 28, 29, 255 });
		sfText_setString(gameOver.text, "YOU DIED");
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
		for (int i = 0; i < 2; i++)
		{
			if (CompareColor(sfText_getColor(gameOver.button[i]), sfWhite))
			{
				if (i)
				{
					SetGameState(GAME);
				}
				else
				{
					SetGameState(MENU);
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

	for (int i = 0; i < 2; i++)
	{
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.button[i]);
		if (IsColidingPointHitbox(&rect, mouse))
		{
			sfText_setColor(gameOver.button[i], sfWhite);
		}
		else
		{
			sfText_setColor(gameOver.button[i], (sfColor) { 255, 165, 0, 255 });
		}
	}
}

void UpdateGameOver(float _dt)
{
	tempScore += RAND_RANGE(1, 100);
	tempScore = (tempScore > score) ? score : tempScore;

	tempHighScore += RAND_RANGE(1, 100);
	tempHighScore = (tempHighScore > highScore) ? highScore : tempHighScore;

	char* text[50];
	sprintf_s(text, sizeof(text), "%07d", tempScore);
	sfText_setString(gameOver.score[2], text);
	sprintf_s(text, sizeof(text), "%07d", tempHighScore);
	sfText_setString(gameOver.score[3], text);

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
		sfSprite_setTextureRect(gameOver.backgroundAnim, gameOver.frameRect[gameOver.currentFrame]);
	}
}
