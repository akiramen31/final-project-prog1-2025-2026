#include "GameOver.h"

void KeyPressedGameOver(sfKeyEvent* _keyEvent);
sfSprite* highScore[6];

void LoadGameOver(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
	sfMusic* gameOverMusic = CreateMusic("Assets/Musics/Game-Over.ogg", GAME_VOLUME, sfFalse);
	sfMusic_setLoop(gameOverMusic, sfTrue);
	sfMusic_play(gameOverMusic);

	//Score
	int highScoreNum = GetIntFromSave(0);
	for (int i = 0; i < 6; i++)
	{
		highScore[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 360 + (36 * i), 200 }, 4.f, 0.f);
		int number = (highScoreNum % (int)(pow(10, i + 1))) - (highScoreNum % (int)(pow(10, i)));
		printf("%d", number);
		sfSprite_setTextureRect(highScore[i], (sfIntRect) { 10 * number, 0, 10, 12 });
	}
	printf("\n");
}

void PollEventGameOver(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGameOver(&_event->key);
		break;
	default:
		break;
	}
}

void UpdateGameOver(float _dt)
{

}

void KeyPressedGameOver(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeyR:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}