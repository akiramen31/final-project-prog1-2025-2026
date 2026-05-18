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
void MouseMovedGame(sfMouseMoveEvent* _mouseMoveEvent);
void MouseButtonPressedGame(sfMouseButtonEvent* _mouseMoveEvent);
sfBool CheckPlayerEndLevel(void);
void SetGameMenu(char _pause);

Game game;

void LoadGame(void)
{
	game = (Game){ 0 };
	game.timerRoomPause = PAUSE_ROOM_DURATION;

	SetIntToSave(CURRENT_SCORE, 0);

	LoadSounds();
	LoadCamera();
	LoadParallax();
	LoadPlayer();
	LoadMap();
	SetIntToSave(DEV_MODE_FLY, 0);
	LoadProjectiles(GetBossPosition().y);
	LoadHUD();
	LoadBossBar(GetBossLifeAdress(), GetBossMaxLife());
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
		game.timerDurationStartLevel = 3.f;
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

	switch (GetCurrentMap())
	{
	case 0:
		sfMusic_setLoop(CreateMusic("Assets/Musics/Niveau_1_Cog.wav", 0.05f, sfTrue), sfTrue);
		break;
	case 1:
		sfMusic_setLoop(CreateMusic("Assets/Musics/Niveau_2_Cog.wav", 0.05f, sfTrue), sfTrue);
		break;
	case 2:
		CreateMusic("Assets/Musics/1914_Its_A_Long_Way_To_Tipperary.ogg", 0.05f, sfTrue);
		break;
	case 3:
		CreateMusic("Assets/Musics/1917_Oh_Johnny,_Oh_Johnny,_Oh.ogg", 0.05f, sfTrue);
		break;
	case 4:
		CreateMusic("Assets/Musics/1917_Over_There.ogg", 0.1f, sfTrue);
		break;
	default:
		break;
	}

	game.pause.backgound = CreateSprite(GetAsset("Assets/Sprites/starting_menu_text1.png"), (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT }, 6.f, -0.5f);
	sfSprite_setTextureRect(game.pause.backgound, (sfIntRect) {0, 39, 112, 97 });
	SetSpriteOriginFoot(game.pause.backgound);

	char* buffer[PAUSE_BUTTON_COUNT] = { "resume", "menu", "restart","quit" };
	for (int i = 0; i < PAUSE_BUTTON_COUNT; i++)
	{
		game.pause.button[i] = CreateText(GetAsset(FONT), (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + i * 75 }, 50.f, -0.6f);
		sfText_setString(game.pause.button[i], buffer[i]);
		SetTextOriginFoot(game.pause.button[i]);
	}

	SetGameMenu(0);
}

void PollEventGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGame(&_event->key);
		break;
	case sfEvtMouseMoved:
		MouseMovedGame(&_event->mouseMove);
		break;
	case sfEvtMouseButtonPressed:
		MouseButtonPressedGame(&_event->mouseButton);
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
	default:
		break;
	}
}

void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	if (game.pause.actif)
	{
		switch (_keyEvent->code)
		{
		case sfKeyEscape:
			SetGameMenu(0);
			return;
		default:
			break;
		}
	}
	else
	{
		switch (_keyEvent->code)
		{
		case sfKeyEscape:
			SetGameMenu(1);
			return;
		default:
			break;
		}

#if DEV_MODE
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
		case sfKeyF5:
			SetIntToSave(DEV_MODE_FLY, 1);
			break;
		case sfKeyF6:
			SetIntToSave(DEV_MODE_FLY, 0);
			break;
		case sfKeyF7:
			TpPlayerBoss();
			break;
		case sfKeyF11:
			KillPlayer();
			break;
		default:
			break;
		}
#endif
	}

}

void MouseMovedGame(sfMouseMoveEvent* _mouseMoveEvent)
{
	if (game.pause.actif)
	{
		sfFloatRect rect = { 0 };
		for (int i = 0; i < PAUSE_BUTTON_COUNT; i++)
		{
			rect = sfText_getGlobalBounds(game.pause.button[i]);
			if (sfFloatRect_contains(&rect, _mouseMoveEvent->x, _mouseMoveEvent->y))
			{
				sfText_setColor(game.pause.button[i], sfWhite);
			}
			else
			{
				sfText_setColor(game.pause.button[i], COLOR_ORANGE);
			}
		}
	}
}

void MouseButtonPressedGame(sfMouseButtonEvent* _mouseMouseButton)
{
	if(CompareColor(sfText_getColor(game.pause.button[0]), sfWhite))
	{
		SetGameMenu(0);
	}
	else if (CompareColor(sfText_getColor(game.pause.button[1]), sfWhite))
	{
		SetGameState(MENU);
	}
	else if (CompareColor(sfText_getColor(game.pause.button[2]), sfWhite))
	{
		SetGameState(GAME);
	}
	else if (CompareColor(sfText_getColor(game.pause.button[3]), sfWhite))
	{
		sfRenderWindow_close(GetRenderWindow());
	}
}

void UpdateGame(float _dt)
{
	if (game.pause.actif)
	{

	}
	else
	{
		if (!PauseGameCameraMoveRoom() && game.startIntroIsFinished)
		{
			if (game.timerRoomPause >= PAUSE_ROOM_DURATION)
			{
				if (GetPlayerLife() <= 0)
				{
					SetGameState(GAME_OVER);
					return;
				}

				if (GetCurrentMap() == LEVEL1 || GetCurrentMap() == LEVEL2)
				{
					UpdateBoss(GetPlayerPosition(), _dt);
				}

				UpdatePlayer(sfFalse, _dt);
				UpdateEnemy(_dt);

				UpdateProjectiles(GetMousePositionToOrigin(), _dt);
				UpdateSecondary(GetMousePositionToOrigin(), _dt);
				MovePlayer(UpdateElevator(GetPlayerRect(), GetPlayerPosition(), _dt));

				UpdateEntity(_dt);

				//if (!IsBossActive())
				{
					if (CheckPlayerEndLevel())
					{
						SetGameState(GAME_OVER);
						return;
					}
				}
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
		CleanUpFinishedSounds();

		if (game.timerStartLevel < game.timerDurationStartLevel)
		{
			UpdatePlayer(sfTrue, _dt);

			sfVector2f pos = GetPlayerCenterPosition();

			SetViewCenter(pos);

			sfCircleShape_setPosition(game.startIntroCircle, pos);
			sfCircleShape_setRadius(game.startIntroCircle, sfCircleShape_getRadius(game.startIntroCircle) + 125 * _dt);

			float radius = sfCircleShape_getRadius(game.startIntroCircle);
			sfCircleShape_setOrigin(game.startIntroCircle, (sfVector2f) { radius, radius });

			sfRectangleShape_setPosition(game.startIntoRectangle, pos);

			game.timerStartLevel += _dt;
			ToggleVisibilityHUD(sfFalse);
		}
		else
		{
			if (!game.startIntroIsFinished)
			{
				ToggleVisibilityHUD(sfTrue);
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
}

sfBool CheckPlayerEndLevel(void)
{
	InfoZone* zone = GetInfoZoneTriger(GetPlayerRect());
	int num = GetTrigerCount();

	if (zone != 0)
	{
		for (int i = 0; i < num; i++)
		{
			if (IsPointInFloatRect(GetPlayerCenterPosition(), &zone[i].hitbox))
			{
				if (StringCompare(zone[i].type, "EndLevel"))
				{
					return sfTrue;
				}
			}
		}
	}

	return sfFalse;
}

void SetGameMenu(char _pause)
{
	if (_pause)
	{
		sfSprite_setColor(game.pause.backgound, sfWhite);
		for (int i = 0; i < PAUSE_BUTTON_COUNT; i++)
		{
			sfText_setColor(game.pause.button[i], COLOR_ORANGE);
		}
	}
	else
	{
		sfSprite_setColor(game.pause.backgound, (sfColor) {0});
		for (int i = 0; i < PAUSE_BUTTON_COUNT; i++)
		{
			sfText_setColor(game.pause.button[i], (sfColor) { 0 });
		}
	}
	game.pause.actif = _pause;
}