#include "GameOver.h"
#include "Player.h"

GameOver gameOver;

int live;

void KeyPressedGameOver(sfEvent* _event);
void MouseButtonPressedGameOver(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedGameOver(sfMouseMoveEvent* _mouseMovedEvent);

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	int highScore = 50;
	//int score = GetIntFromSave(CURRENT_SCORE);
	int score = 0;
	sfFont* font = GetAsset("Assets/Fonts/Daydream.otf");

	switch (GetCurrentMap())
	{
	case LEVEL1:
		highScore = GetIntFromSave(HIGH_SCORE_1);
		break;
	case LEVEL2:
		highScore = GetIntFromSave(HIGH_SCORE_2);
		break;
	case LEVEL3:
		highScore = GetIntFromSave(HIGH_SCORE_3);
		break;
	default:
		break;
	}

	highScore = (score > highScore) ? score : highScore;

	live = GetPlayerLife();

#pragma region button
	for (int i = 0; i < 2; i++)
	{
		gameOver.button[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 3 * (i + 1), SCREEN_HEIGHT / 3 * 2 }, 1.f, 10);
		sfText_setCharacterSize(gameOver.button[i], 60);
	}

	sfText_setString(gameOver.button[0], "MENU");
	sfText_setString(gameOver.button[1], "RETRY");

	for (int i = 0; i < 2; i++)
	{
		sfFloatRect rect = sfText_getGlobalBounds(gameOver.button[i]);
		sfText_setOrigin(gameOver.button[i], (sfVector2f) { rect.width / 2, rect.height });
	}
#pragma endregion

#pragma region score
	int decal = 50;

	for (int i = 0; i < 2; i++)
	{
		gameOver.score[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2 - decal, SCREEN_HEIGHT / 6 * (1 + i) }, 1.f, 10);
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
		gameOver.score[i] = CreateText(font, (sfVector2f) { SCREEN_WIDTH / 2 + decal, SCREEN_HEIGHT / 6 * (i - 1) }, 1.f, 10);
		sfText_setCharacterSize(gameOver.score[i], 60);
	}

	char* text[50];
	sprintf_s(text, sizeof(text), "%07d", score);
	sfText_setString(gameOver.score[2], text);
	sprintf_s(text, sizeof(text), "%07d", highScore);
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

void MouseButtonPressedGameOver(sfMouseButtonEvent* _mouseButtonEvent)
{
	switch (_mouseButtonEvent->button)
	{
	default:
		break;
	}
}

void MouseMovedGameOver(sfMouseMoveEvent* _mouseMovedEvent)
{
	float mouseX = (float)_mouseMovedEvent->x;
	float mouseY = (float)_mouseMovedEvent->y;
}


void UpdateGameOver(float _dt)
{

}
