#ifndef LOAD_GAME_SAVE_H
#define LOAD_GAME_SAVE_H

#include "Common.h"

#define NB_SAVE 5
#define NB_MAP 3

typedef struct LoadGameSave
{
	sfText* save[NB_SAVE];
	sfText* back;
	sfText* next;
	int pageI;

	sfText* destroy;
	sfText* load;
	sfText* newGame;

	sfText* score[NB_MAP];
	sfText* unlocker;
	sfText* name;
	char nameTrigger;
	sfRectangleShape* rect;
}LoadGameSave;

void LoadLoadGameSave(sfFont* _font);
void KeyPressedLoadGameSave(sfEvent* _event);
int MouseButtonPressedLoadGameSave(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedLoadGameSave(sfMouseMoveEvent* _mouseMovedEvent);
void SetVisibleLoadGameSave(sfBool _state);



#endif //LOAD_GAME_SAVE_H
