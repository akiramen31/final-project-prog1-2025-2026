#include "Game.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Ennemy.h"
#include "Map.h"
#include "Camera.h"
#include "Boss.h"
#include "Parallax.h"
#include "Elevator.h"
#include "Projectiles.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	game.timerRoomPause = PAUSE_ROOM_DURATION;

	LoadCamera();
	LoadParallax();
	LoadPlayer();
	LoadBoss();
	LoadMap();
	SetIntToSave(DEV_MODE_FLY, 0);
	LoadProjectiles(GetBossPositionY());
#if !DEV_PIERRE_ENEMY
	LoadEnemy();
#endif

	LoadHUD();
	SetHpFocus(GetBossHpAdr());
	//LoadGUI();
	LoadWeapons();

	TpPlayerToSpawn();

	game.timerstartLevel = 0;

	game.startIntroCircle = CreateCircleShape((sfFloatRect) { 0, 0, 1, 1 }, sfTransparent, sfBlack, 1.f);
	sfCircleShape_setOutlineThickness(game.startIntroCircle, 500.f);
	game.startIntroCircleHiden = sfFalse;

	switch (GetIntFromSave(CURRENT_MUSIC))
	{
	case 0:
		CreateMusic("Assets/Musics/1914_Its_A_Long_Way_To_Tipperary.ogg", 0.1f, sfTrue);
		break;
	case 1:
		CreateMusic("Assets/Musics/1914_United_Forces_March.ogg", 0.1f, sfTrue);
		break;
	case 2:
		CreateMusic("Assets/Musics/1915_Dont_Bite_The_Hand_Thats_Feeding_You.ogg", 0.1f, sfTrue);
		break;
	case 3:
		CreateMusic("Assets/Musics/1917_Oh_Johnny,_Oh_Johnny,_Oh.ogg", 0.1f, sfTrue);
		break;
	case 4:
		CreateMusic("Assets/Musics/1917_Over_There.ogg", 0.1f, sfTrue);
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
		return;
	default:
		break;
	}
	if (DEV_MODE)
	{
		switch (_keyEvent->code)
		{
		case sfKeyF1:
			SetCurrentMap(LEVEL1);
			break;
		case sfKeyF2:
			SetCurrentMap(LEVEL2);
			break;
		case sfKeyF3:
			SetCurrentMap(LEVEL3);
			break;
		case sfKeyF4:
			SetCurrentMap(LEVEL_TEST);
			break;
#if DEV_PIERRE_ENEMY
		case sfKeyP:
			HitEnemyI(0, (sfVector2f) { 7, 7 }, 5, HEAVY);
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
		if (!PauseGameCameraMoveRoom() || game.timerstartLevel <= START_GAME_CAM_DURATION)
		{
			if (game.timerRoomPause >= PAUSE_ROOM_DURATION)
			{
				if (GetPlayerLife() <= 0)
				{
					SetCurrentMap(GetCurrentMap());
					AddPlayerLife(PLAYER_MAX_HEALTH);
				}

				if (GetCurrentMap() == LEVEL1)
				{
					UpdateBoss(GetPlayerPosition(), _dt);
				}

				UpdatePlayer(_dt);
				UpdateEnemy(_dt);

				UpdateProjectiles(GetMousePositionToOrigin(), _dt);
				UpdateSecondary(GetMousePositionToOrigin(), _dt);
				MovePlayer(UpdateElevator(GetPlayerRect(), GetPlayerPosition(), _dt));

				VisibilityBossBar(IsBossActive());

			}
			else
			{
				if (game.timerRoomPause <= PAUSE_ROOM_DURATION)
				{
					game.timerRoomPause += _dt;
				}
			}

			if (game.timerstartLevel < START_GAME_CAM_DURATION)
			{
				sfFloatRect hitbox = GetPlayerRect();
				SetViewCenter((sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });

				sfCircleShape_setPosition(game.startIntroCircle, (sfVector2f) { hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 });
				sfCircleShape_setRadius(game.startIntroCircle, sfCircleShape_getRadius(game.startIntroCircle) + 125 * _dt);
				hitbox = sfCircleShape_getGlobalBounds(game.startIntroCircle);
				sfCircleShape_setOrigin(game.startIntroCircle, (sfVector2f) { sfCircleShape_getRadius(game.startIntroCircle), sfCircleShape_getRadius(game.startIntroCircle) });

				game.timerstartLevel += _dt;
			}
			else
			{
				if (!game.startIntroCircleHiden)
				{
					game.startIntroCircleHiden = sfTrue;
					sfCircleShape_setScale(game.startIntroCircle, (sfVector2f) { 0 });
				}
			}
		}
		else
		{
			game.timerRoomPause = 0;
		}

		UpdateHUD(_dt);
		UpdateCamera(_dt);
		UpdateParallax(_dt);
	}
	else
	{
	}
}