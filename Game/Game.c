#include "Game.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "Enemy.h"
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
	LoadPlayer();
	LoadMap();
	SetIntToSave(DEV_MODE_FLY, 0);
	LoadProjectiles(GetBossPosition().y);
#if !DEV_PIERRE_ENEMY
	LoadEnemy();
#endif

	LoadHUD();
	LoadBossBar(GetBossLifeAdress());
	//LoadGUI();
	LoadWeapons();

	TpPlayerToSpawn();

	game.startIntroCircle = CreateCircleShape((sfFloatRect) { 0, 0, 1, 1 }, sfTransparent, sfBlack, 1.f);
	sfCircleShape_setOutlineThickness(game.startIntroCircle, 500.f);

	game.startIntoRectangle = CreateRectangleShape((sfFloatRect) { 0, 0, 150, 250 }, sfTransparent, sfBlack, 1.f);
	sfRectangleShape_setOrigin(game.startIntoRectangle, (sfVector2f) { 75, 125 });
	sfRectangleShape_setOutlineThickness(game.startIntoRectangle, 500.f);

	game.startIntroIsFinished = sfFalse;

	switch (GetCurrentMap())
	{
	case 0:
		game.timerDurationStartLevel = 2.5f;
		break;
	case 1:
	case 2:
		game.timerDurationStartLevel = 1.5f;
		game.timerDurationStartLevel = 1.5f;
		break;
	default:
		game.timerDurationStartLevel = 0;
		break;
	}

	if (DEV_MODE_CAMERA)
	{
		game.cameraCenter = CreateCircleShape((sfFloatRect) { 0, 0, 2, 2 }, sfRed, sfBlue, 1.f);
	}

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
		if (!PauseGameCameraMoveRoom() && game.startIntroIsFinished)
		{
			if (game.timerRoomPause >= PAUSE_ROOM_DURATION)
			{
				if (GetPlayerLife() <= 0)
				{
					AddPlayerLife(PLAYER_MAX_HEALTH);
					LoadGame();
				}

				if (GetCurrentMap() == LEVEL1)
				{
					UpdateBoss(GetPlayerPosition(), _dt);
				}

				UpdateMap(_dt);
				UpdatePlayer(sfFalse, _dt);
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

		}
		else
		{
			game.timerRoomPause = 0;
		}

		if (game.timerStartLevel < game.timerDurationStartLevel)
		{
			UpdateMap(_dt);
			UpdatePlayer(sfTrue, _dt);

			sfVector2f pos = GetPlayerCenterPosition();

			SetViewCenter(pos);

			sfCircleShape_setPosition(game.startIntroCircle, pos);
			sfCircleShape_setRadius(game.startIntroCircle, sfCircleShape_getRadius(game.startIntroCircle) + 125 * _dt);

			float radius = sfCircleShape_getRadius(game.startIntroCircle);
			sfCircleShape_setOrigin(game.startIntroCircle, (sfVector2f) { radius, radius });

			sfRectangleShape_setPosition(game.startIntoRectangle, pos);

			game.timerStartLevel += _dt;
			VisibilityHUD(sfFalse);
		}
		else
		{
			if (!game.startIntroIsFinished)
			{
				VisibilityHUD(sfTrue);
				game.startIntroIsFinished = sfTrue;
				sfCircleShape_setScale(game.startIntroCircle, (sfVector2f) { 0 });
				sfRectangleShape_setScale(game.startIntoRectangle, (sfVector2f) { 0 });
			}
		}

		UpdateHUD(_dt);
		UpdateCamera(_dt);

		if (DEV_MODE_CAMERA)
		{
			sfVector2f pos = GetViewCenterPosition();
			sfCircleShape_setPosition(game.cameraCenter, pos);
		}
		UpdateParallax(_dt);
	}
	else
	{
		//UpdateGUI(_dt);
	}
}