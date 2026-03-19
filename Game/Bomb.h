#ifndef BOMB_h
#define BOMB_H

#include "Common.h"

#define BLOW_TIMER_BOMB 3.f
#define NUM_MAX_BOMB 8

typedef enum BlowDirection
{
	BLOWDOWN = 1,
	BLOWRIGHT = 8,
	BLOWUP = 1,
	BLOWLEFT = 2
}BlowDirection;

typedef struct Bomb
{
	sfVector2i position;
	sfSprite* sprite;
	Animation animation;

	int blowDirectionCode;
	sfBool placed;

	float duration;
}Bomb;

void LoadBomb(void);
void SpawnBomb(sfVector2i _bombPos);
void UpdateBomb(CasePosibility _Colision[], float _dt);
void BlowBomb(int i, CasePosibility _colision);
sfBool CheckAtLocationBomb(sfVector2i _pos);
int GetBombCount(void);
sfVector2i GetBombPositionGrid(int _num);
void CreateDeflagration(Direction _direction, int length);
#endif // BOMB_H