#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
	sfSprite* background = LoadBackground(GetAsset("Assets/Maps/Level1.png"), 1.f);
	LoadPlayer();
	
	LoadMap(background);
	

	//LoadHUD();
	//LoadGUI();
	LoadPlayer();
	LoadAim();
}

void PollEventGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGame(&_event->key);
		break;
	case sfEvtMouseMoved:
		break;
	case sfEvtMouseButtonPressed:
		if (DEV_ENNEMY)
		{
			sfVector2f viewPosition = GetViewPosition();
			sfColor tempColor = GetColorsPixelMap((sfVector2f) { _event->mouseButton.x+viewPosition.x, _event->mouseButton.y+viewPosition.y });
			printf("color is a = %d \n", tempColor.a);
            sfVector2u pos = RealPositionConvertTableauPosition((sfVector2f) { _event->mouseButton.x + viewPosition.x, _event->mouseButton.y + viewPosition.y });
            printf("position x:%d y:%d\n", pos.x, pos.y);
		}
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
		break;
	default:
		break;
	}
	if (DEV_MODE)
	{
		switch (_keyEvent->code)
		{
		case sfKeyF1:
			SetMap(LEVEL1);
			break;
		case sfKeyF2:
			SetMap(LEVEL2);
			break;
		case sfKeyF3:
			SetMap(LEVEL3);
			break;
		case sfKeyF4:
			SetMap(LEVEL_TEST);
			break;
		default:
			break;
		}
	}
}

void UpdateGame(float _dt)
{
	UpdatePlayer(_dt);
	if (DEV_ENNEMY)
	{
		UpdateEnnemy(_dt, 0);
	}
	//UpdateHUD(_dt);
	//UpdateGUI(_dt);
	UpdateCollider();
	UpdateAim(_dt);
}

void UpdateCollider(void)
{

}