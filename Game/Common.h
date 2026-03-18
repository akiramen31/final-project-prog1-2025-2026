#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "SFML/Graphics.h"
#include "SFML/Audio.h"

#include "Backup.h"
#include "EntityManager.h"

#define GAME_SCALE 1
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 904
#define BPP 32

# define DEV_MODE 1

#define FIRST_CASE (sfIntRect) {32, 160, 64, 64}
#define NB_GRID_ROW 9
#define NB_GRID_COLUMN 15 

typedef struct Animation
{
	sfIntRect rectActualy;
	sfBool isLooping;
	int frameCount;
	float frameDuration;
	float timeActualy;
}Animation;

typedef enum Direction
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
}Direction;

typedef struct
{
	sfBool down;
	sfBool left;
	sfBool right;
	sfBool up;
}MovePosibility;


typedef struct Element
{
	void* value;
	struct Element* next;
}Element;

typedef struct List
{
	Element* first;
}List;



int GetRandomInRange(int _min, int _max);
float GetAngleToVector(sfVector2f _vector);
float RadToDeg(float _angleRad);
float DegToRad(float _angleDeg);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPionHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);



List* CreateList(void);
void RemoveList(List* _list);
unsigned int GetListSize(List* _list);

Element* CreateElement(void* _value);
Element* GetElement(List* _list, unsigned int _index);
void InsertElement(List* _list, Element* _element, unsigned int _index);
void RemoveElement(List* _list, unsigned int _index);

#endif // !COMMON_H
