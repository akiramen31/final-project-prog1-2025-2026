#include "Game.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h"
#include "Map.h"
#include "Camera.h"
#include "Boss.h"
#include "Missile.h"
#include "Parallax.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;
float timerRoomPause = PAUSE_ROOM_DURATION;
float timerstartLevel = 0;

void LoadGame(void)
{
	LoadParallax();
	game = (Game){ 0 };
	LoadMap();
	SetIntToSave(DEV_MODE_FLY, 0);
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

	LoadMap();
	LoadHUD();
	//LoadGUI();
	LoadMissile();

	timerstartLevel = 0;

	SetMap(LEVEL1);
	switch (GetIntFromSave(MUSIC_ACTUALY))
	{
	case 0:
		CreateMusic("Assets/Musics/1914_Its_A_Long_Way_To_Tipperary.ogg", 10.f, sfTrue);
		break;
	case 1:
		CreateMusic("Assets/Musics/1914_United_Forces_March.ogg", 10.f, sfTrue);
		break;
	case 2:
		CreateMusic("Assets/Musics/1915_Dont_Bite_The_Hand_Thats_Feeding_You.ogg", 10.f, sfTrue);
		break;
	case 3:
		CreateMusic("Assets/Musics/1917_Oh_Johnny,_Oh_Johnny,_Oh.ogg", 10.f, sfTrue);
		break;
	case 4:
		CreateMusic("Assets/Musics/1917_Over_There.ogg", 10.f, sfTrue);
		break;
	default:
		break;
	}
}

void PollEventGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGame(&_event->key);
		break;
	case sfEvtKeyReleased:
		switch (_event->key.code)
		{
		case sfKeyO:
			ChangePlayerInvicibility();
			break;
		default:
			break;
		}
		break;
	case sfEvtMouseMoved:
		break;
	case sfEvtMouseButtonPressed:
		if (DEV_ENNEMY)
		{
			//MapData* mapData = GetMapData();
			//sfVector2i mousePosI = sfMouse_getPositionRenderWindow(GetRenderWindow());
			//sfVector2f viewPos = GetViewPosition();
			//sfVector2f mousePos = { (float)mousePosI.x * GetCameraZoom() + viewPos.x, (float)mousePosI.y * GetCameraZoom() + viewPos.y };
			//printf("x = %d y = %d a= %d\n", (int) { mousePos.x / mapData->caseSize.x }, (int) { mousePos.y / mapData->caseSize.y }, sfImage_getPixel(mapData->image, (unsigned int)(mousePos.x / mapData->caseSize.x), (unsigned int)(mousePos.y / mapData->caseSize.y)).a);
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
	if (sfTrue /*PauseGame*/)
	{
		if (!PauseGameCameraMoveRoom() || timerstartLevel <= START_GAME_CAM_DURATION)
		{
			if (timerRoomPause >= PAUSE_ROOM_DURATION)
			{
				if (GetPlayerLife() <= 0)
				{
					SetMap(GetActualyMap());
					AddPlayerLife(PLAYER_MAX_HEALTH);
				}

				UpdatePlayer(_dt);
				UpdateEnemy(_dt);

				//UpdateGUI(_dt);
				UpdateCollider();
				if (GetActualyMap() == LEVEL1)
				{
					UpdateBoss(_dt);
				}

				UpdateBullet(_dt);
				UpdateMisteal(_dt);
				UpdateSecondary(GetMousePositionToOrigin(), _dt);
			}
		}
		else
		{
			timerRoomPause = 0;
		}
		UpdateBullet(_dt);
		UpdateMisteal(_dt);
		UpdateMissile(GetMousePositionToOrigin(), _dt);
	}

	if (timerRoomPause <= PAUSE_ROOM_DURATION)
	{
		timerRoomPause += _dt;
	}

	if (timerstartLevel <= START_GAME_CAM_DURATION)
	{
		timerstartLevel += _dt;
	}

	UpdateHUD(_dt);
	UpdateCamera(_dt);
	UpdateCamera(_dt);
	UpdateParallax(_dt);
}

void UpdateCollider(void)
{

}