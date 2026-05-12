#include "StartGame.h"
void KeyPressedStartGame(sfKeyEvent* _keyEvent);
void LoadLogoStartGame(sfSprite* _sp1, sfSprite* _sp2, sfVector2f _pos);

StartGame startGame;

void LoadStartGame(void)
{
	startGame = (StartGame){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/fondNoir.png"), 8.f);
	startGame.textureCrea[0] = GetAsset("Assets/Sprites/Creajeux_Logo.png");
	startGame.textureCrea[1] = GetAsset("Assets/Sprites/Creajeux_Logo2.png");
	sfIntRect rectI = { 0 };
	if (rand() % 2)
	{
		startGame.crea1 = CreateSprite(startGame.textureCrea[1], CREA_POS, 0.65f, 1.f);
	}
	else
	{
		startGame.crea1 = CreateSprite(startGame.textureCrea[0], CREA_POS, 0.25f, 1.f);
		rectI = sfSprite_getTextureRect(startGame.crea1);
		sfSprite_setTextureRect(startGame.crea1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 500 });
	}
	startGame.crea2 = CreateSprite(startGame.textureCrea[0], CREA_POS, 0.25f, 1.f);
	rectI = sfSprite_getTextureRect(startGame.crea2);
	sfSprite_setTextureRect(startGame.crea2, (sfIntRect) { rectI.left, 500, rectI.width, rectI.height - 500 });
	LoadLogoStartGame(startGame.crea1, startGame.crea2, CREA_POS);

	sfTexture* texture = GetAsset("Assets/Sprites/studio.png");
	startGame.rose1 = CreateSprite(texture, ROSE_POS, 3.0f, 1.f);
	rectI = sfSprite_getTextureRect(startGame.rose1);
	sfSprite_setTextureRect(startGame.rose1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 45 });

	startGame.rose2 = CreateSprite(texture, ROSE_POS, 3.0f, 1.f);
	rectI = sfSprite_getTextureRect(startGame.rose2);
	sfSprite_setTextureRect(startGame.rose2, (sfIntRect) { rectI.left, 45, rectI.width, rectI.height - 45});
	LoadLogoStartGame(startGame.rose1, startGame.rose2, ROSE_POS);

	startGame.update = 1;
}

void LoadLogoStartGame(sfSprite* _sp1, sfSprite* _sp2, sfVector2f _pos)
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

void PollEventStartGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedStartGame(&_event->key);
		break;
	default:
		break;
	}
}

void UpdateStartGame(float _dt)
{
	if (startGame.update)
	{
		if (sfSprite_getRotation(startGame.crea1) > 10)
		{
			sfSprite_rotate(startGame.crea1, 9 * _dt);
			sfSprite_move(startGame.crea2, (sfVector2f) { (CREA_POS.x - sfSprite_getPosition(startGame.crea2).x)* _dt * 4, 0.f });

			sfSprite_rotate(startGame.rose1, 9 * _dt);
			sfSprite_move(startGame.rose2, (sfVector2f) { (ROSE_POS.x - sfSprite_getPosition(startGame.rose2).x)* _dt * 4, 0.f });
		}
		else
		{
			sfSprite_setRotation(startGame.crea1, 0.f);
			sfSprite_setTexture(startGame.crea1, startGame.textureCrea[0], sfTrue);
			sfIntRect rectI = sfSprite_getTextureRect(startGame.crea1);
			sfSprite_setTextureRect(startGame.crea1, (sfIntRect) { rectI.left, rectI.top, rectI.width, 500 });
			SetSpriteOriginFoot(startGame.crea1);
			sfSprite_setPosition(startGame.crea1, CREA_POS);
			sfSprite_setScale(startGame.crea1, (sfVector2f) { 0.25f, 0.25f });

			sfSprite_setPosition(startGame.crea2, CREA_POS);

			sfSprite_setRotation(startGame.rose1, 0.f);
			SetSpriteOriginFoot(startGame.rose1);
			sfSprite_setPosition(startGame.rose1, ROSE_POS);

			sfSprite_setPosition(startGame.rose2, ROSE_POS);
			startGame.update = 0;
		}
	}
	else if (startGame.timer < 2.f)
	{
		startGame.timer += _dt;
	}
	else
	{
		SetGameState(MENU);
	}
}

void KeyPressedStartGame(sfKeyEvent* _keyEvent)
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