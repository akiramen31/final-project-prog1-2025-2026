#ifndef MENU_SELECTION_GAME_H
#define MENU_SELECTION_GAME_H

#include "Common.h"
#include "Map.h"
#include "Weapons.h"

#define NB_INFO_BUTTONS 6
#define CREDIT_COUNT 5
#define KEY_COUNT 8
#define MAX_INFO 6
#define MUSIC_COUNT 3

typedef enum
{
	MAP,
	WEAPON,
	SECONRARY,
	COUND_MENU_SELECTION_GAME_STATE
}MenuSelectionGameState;

typedef enum ButtonRect
{
	MAP_1,
	WEAPON_1,
	UNSELECT,
	MAP_2,
	WEAPON_2,
	HIGHLIGHT,
	MAP_3,
	WEAPON_3,
	SELECT,
	SECONDARY_1,
	SECONDARY_2,
	LOCKED,
	RECT_COUNT
}ButtonRect;

typedef struct
{
	sfSprite* categoryButton[3];
	sfSprite* categoryIcon[3];
	sfSprite* generalButton[WEAPON_COUNT];
	sfSprite* generalIcon[WEAPON_COUNT];
	sfText* bottomText[2];
	sfText* descriptionText;
	char* description[COUND_MENU_SELECTION_GAME_STATE][3];
	MenuSelectionGameState state;
}MenuSelectionGame;

void LoadMenuSelectionGame(void);
void PollEventMenuSelectionGame(sfEvent* _event);

#endif