#include "GameOver.h"

void KeyPressedGameOver(sfKeyEvent* _keyEvent);
sfSprite* highScore[6];

void LoadGameOver(void)
{
	//Score
	int scoreTemp = GetIntFromSave(SCORE);
	for (int i = 5; i >= 0; i--)
	{
		sfSprite* sprite = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { SCREEN_WIDTH / 2.0f + (36 * (i - 3)), 200.f }, 4.f, 0.f);
		sfSprite_setTextureRect(sprite, (sfIntRect) { 10 * (scoreTemp % 10), 0, 10, 12 });
		scoreTemp /= 10;
	}

	SetIntToSave(SCORE, 0);
	SetIntToSave(BOMB, 1);
	SetIntToSave(SPEED, 1);
	SetIntToSave(ENNEMY_COUNT, 3);
	SetIntToSave(FIRE, 1);
	SetIntToSave(LIFE, 4);

	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);

	sfText* text = CreateText(GetAsset("Assets/Fonts/Bomberman.ttf"), (sfVector2f) { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 1.5f }, 1.f, 5.f);
	sfText_setString(text, "Press <R> to reload game \n press <Echap> to quit");
	sfText_setOrigin(text, (sfVector2f) { sfText_getGlobalBounds(text).width / 2.f, 0 });

	sfMusic* gameOverMusic = CreateMusic("Assets/Musics/Game-Over.ogg", GAME_VOLUME, sfFalse);
	sfMusic_setLoop(gameOverMusic, sfTrue);
	sfMusic_play(gameOverMusic);
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