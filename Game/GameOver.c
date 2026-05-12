#include "GameOver.h"
void KeyPressedGameOver(sfKeyEvent* _keyEvent);
void LoadLogoGameOver(sfSprite* _sp1, sfSprite* _sp2, sfVector2f _pos);

GameOver gameOver;

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/fondNoir.png"), 8.f);
	gameOver.textureCrea[0] = GetAsset("Assets/Sprites/Creajeux_Logo.png");
	gameOver.textureCrea[1] = GetAsset("Assets/Sprites/Creajeux_Logo2.png");
	sfIntRect rectI = { 0 };
	if (rand() % 2)
	{
		gameOver.crea1 = CreateSprite(gameOver.textureCrea[1], CREA_POS, 0.65f, 1.f);
	}
	else
	{
		gameOver.crea1 = CreateSprite(gameOver.textureCrea[0], CREA_POS, 0.25f, 1.f);
		rectI = sfSprite_getTextureRect(gameOver.crea1);
		sfSprite_setTextureRect(gameOver.crea1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 500 });
	}
	gameOver.crea2 = CreateSprite(gameOver.textureCrea[0], CREA_POS, 0.25f, 1.f);
	rectI = sfSprite_getTextureRect(gameOver.crea2);
	sfSprite_setTextureRect(gameOver.crea2, (sfIntRect) { rectI.left, 500, rectI.width, rectI.height - 500 });
	LoadLogoGameOver(gameOver.crea1, gameOver.crea2, CREA_POS);

	sfTexture* texture = GetAsset("Assets/Sprites/studio.png");
	gameOver.rose1 = CreateSprite(texture, ROSE_POS, 3.0f, 1.f);
	rectI = sfSprite_getTextureRect(gameOver.rose1);
	sfSprite_setTextureRect(gameOver.rose1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 45 });

	gameOver.rose2 = CreateSprite(texture, ROSE_POS, 3.0f, 1.f);
	rectI = sfSprite_getTextureRect(gameOver.rose2);
	sfSprite_setTextureRect(gameOver.rose2, (sfIntRect) { rectI.left, 45, rectI.width, rectI.height - 45});
	LoadLogoGameOver(gameOver.rose1, gameOver.rose2, ROSE_POS);

	gameOver.update = 1;
}

void LoadLogoGameOver(sfSprite* _sp1, sfSprite* _sp2, sfVector2f _pos)
{
	sfFloatRect rect = sfSprite_getLocalBounds(_sp1);
	sfSprite_setOrigin(_sp1, (sfVector2f) { 0.f, rect.height });
	rect = sfSprite_getGlobalBounds(_sp1);
	sfSprite_setPosition(_sp1, (sfVector2f) { _pos.x - (rect.width / 2.f), _pos.y });
	sfSprite_setRotation(_sp1, -10.f);

	rect = sfSprite_getLocalBounds(_sp2);
	sfSprite_setOrigin(_sp2, (sfVector2f) { rect.width / 2.f, 0.f });
	rect = sfSprite_getGlobalBounds(_sp2);
	sfSprite_move(_sp2, (sfVector2f) { 100.f, 0.f });
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
	if (gameOver.update)
	{
		if (sfSprite_getRotation(gameOver.crea1) > 10)
		{
			sfSprite_rotate(gameOver.crea1, 9 * _dt);
			sfSprite_move(gameOver.crea2, (sfVector2f) { (CREA_POS.x - sfSprite_getPosition(gameOver.crea2).x)* _dt * 4, 0.f });

			sfSprite_rotate(gameOver.rose1, 9 * _dt);
			sfSprite_move(gameOver.rose2, (sfVector2f) { (ROSE_POS.x - sfSprite_getPosition(gameOver.rose2).x)* _dt * 4, 0.f });
		}
		else
		{
			sfSprite_setRotation(gameOver.crea1, 0.f);
			sfSprite_setTexture(gameOver.crea1, gameOver.textureCrea[0], sfTrue);
			sfIntRect rectI = sfSprite_getTextureRect(gameOver.crea1);
			sfSprite_setTextureRect(gameOver.crea1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 500 });
			SetSpriteOriginFoot(gameOver.crea1);
			sfSprite_setPosition(gameOver.crea1, CREA_POS);
			sfSprite_setScale(gameOver.crea1, (sfVector2f) { 0.25f, 0.25f });

			sfSprite_setPosition(gameOver.crea2, CREA_POS);

			sfSprite_setRotation(gameOver.rose1, 0.f);
			SetSpriteOriginFoot(gameOver.rose1);
			sfSprite_setPosition(gameOver.rose1, ROSE_POS);

			sfSprite_setPosition(gameOver.rose2, ROSE_POS);
			gameOver.update = 0;
		}
	}
	else if (gameOver.timer < 5.f)
	{
		gameOver.timer += _dt;
	}
	else
	{
		SetGameState(MENU);
	}
}

void KeyPressedGameOver(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	default:
		break;
	}
}