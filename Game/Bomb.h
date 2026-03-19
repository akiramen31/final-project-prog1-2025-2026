#ifndef BOMB_h
#define BOMB_H

#include "Common.h"

#define BLOW_TIMER_BOMB 3.f
#define NUM_MAX_BOMB 8

typedef enum BlowDirection
{
	BLOWDOWN = 1,
	BLOWRIGHT = 2,
	BLOWTOP = 4,
	BLOWLEFT = 8
}BlowDirection;

typedef struct Bomb
{
	sfVector2i bombPosition;
	sfSprite* bombSprite;
	Animation animation;

	int blowDirectionCode;
	sfBool placed;

	float duration;
}Bomb;

void LoadBomb(void);
void SpawnBomb(sfVector2i _bombPos);
void UpdateBomb(float _dt);
void BlowBomb(void);
sfBool CheckAtLocationBomb(sfVector2i _pos);

#endif // BOMB_H