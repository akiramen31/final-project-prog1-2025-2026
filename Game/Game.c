#include "Game.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Aim.h"
#include "Ennemy.h"
#include "Map.h"
#include "Camera.h"
#include "Boss.h"
#include "Missile.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
	SetIntToSave(DEV_MODE_FLY, 0);
	sfSprite* background = LoadBackground(GetAsset("Assets/Maps/Level1.png"), 1.f);
	LoadBullet();
	LoadWeapon();
	LoadPlayer();
#if !DEV_PIERRE_ENEMY
	LoadEnemy();
#endif
	if (GetActualyMap() == LEVEL1)
	{
		LoadBoss();
	}
	LoadMap(background);

	LoadHUD();
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
			MapData* mapData = GetMapData();
			sfVector2i mousePosI = sfMouse_getPositionRenderWindow(GetRenderWindow());
			sfVector2f viewPos = GetViewPosition();
			sfVector2f mousePos = { (float)mousePosI.x * GetCameraZoom() + viewPos.x, (float)mousePosI.y * GetCameraZoom() + viewPos.y };
			printf("x = %d y = %d a= %d\n",
				(int) {mousePos.x / mapData->caseSize.x},
				(int){mousePos.y / mapData->caseSize.y},
                sfImage_getPixel(
                    mapData->image,
                    (unsigned int)(mousePos.x / mapData->caseSize.x),
                    (unsigned int)(mousePos.y / mapData->caseSize.y)
                ).a
            );
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
#if DEV_PIERRE_ENEMY
		case sfKeyP:
			HitEnemyI(0, (sfVector2f) { 7, 7 }, 5);
			break;
#endif
		case sfKeyF5:
			SetIntToSave(DEV_MODE_FLY, 1);
			break;
		case sfKeyF6:
			SetIntToSave(DEV_MODE_FLY, 0);
			break;
		case sfKeyF7:
			TpPlayerBoss();
			break;
		case sfKeyF12:
			KillPlayer();
			break;
		default:
			break;
		}
	}
}

void UpdateGame(float _dt)
{
	if (GetPlayerLife() <= 0)
	{
		SetMap(GetActualyMap());
		AddPlayerLife(99);
	}

	UpdatePlayer(_dt);
	UpdateEnemy(_dt);

	UpdateHUD(_dt);
	//UpdateGUI(_dt);
	UpdateCollider();
	UpdateAim(_dt);
	if (GetActualyMap() == LEVEL1)
	{
		UpdateBoss(_dt);
	}

	UpdateBullet(_dt);
	UpdateMissile(GetAimPosition(), _dt);
	UpdateCamera(_dt);


}

void UpdateCollider(void)
{

}