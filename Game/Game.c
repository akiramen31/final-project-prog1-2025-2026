#include "Game.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h"
#include "Map.h"
#include "Camera.h"
#include "Boss.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
	sfSprite* background = LoadBackground(GetAsset("Assets/Maps/Level1.png"), 1.f);
	LoadBullet();
	LoadWeapon();
	LoadPlayer();
	LoadBoss();
	LoadMap(background);

	//LoadHUD();
	//LoadGUI();
	LoadMissile();
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
			sfVector2i mousePosI = sfMouse_getPositionRenderWindow(GetRenderWindow());
			sfVector2f viewPos = GetViewPosition();
			sfVector2f mousePos = { (float)mousePosI.x * GetCameraZoom() + viewPos.x, (float)mousePosI.y * GetCameraZoom() + viewPos.y};
			sfColor pixelColor = sfImage_getPixel(GetMapData()->image, RealPositionConvertTableauPosition(mousePos).x, RealPositionConvertTableauPosition(mousePos).y);
			printf("a = %d\n", pixelColor.a);
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
		case sfKeyP:
			HitEnemy(0, (sfVector2f) { 7, 7 }, 5);
			break;
		default:
			break;
		}
	}
}

void UpdateGame(float _dt)
{
	UpdatePlayer(_dt);
	UpdateTotalEnemy(_dt);

	//UpdateHUD(_dt);
	//UpdateGUI(_dt);
	UpdateCollider();
	UpdateAim(_dt);
	UpdateBullet(_dt);
	UpdateMissile(GetAimPosition(), _dt);
	UpdateCamera(_dt);
}

void UpdateCollider(void)
{

}