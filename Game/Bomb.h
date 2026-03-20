#ifndef BOMB_h
#define BOMB_H

#include "Common.h"

#define BLOW_TIMER_BOMB 3.f
#define NUM_MAX_BOMB 8
#define NUM_MAX_DEFLAGRATION 255

typedef enum BlowDirection
{
	BLOW_DOWN = 1,
	BLOW_RIGHT = 2,
	BLOW_UP = 4,
	BLOW_LEFT = 8
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
	
typedef struct Deflagration
{
	sfVector2i position;

	sfSprite* sprite;
	BlowDirection direction;
	Animation animation;

	sfBool placed;

}Deflagration;

typedef struct Explosion
{
	sfBool state;
	int index;
}Explosion;

void LoadBomb(void);
void SpawnBomb(sfVector2i _bombPos);
Explosion UpdateBomb(CasePosibility _Colision[], float _dt);
void BlowBomb(int _num, CasePosibility _colision);
void BlowBombByDeflagration(int _num, CasePosibility _colision, BlowDirection _direction);
sfBool CheckAtLocationBomb(sfVector2i _pos);
int GetBombCount(void);
sfVector2i GetBombPositionGrid(int _num);
void CreateDeflagration(BlowDirection _direction, int _length, sfVector2i _position);
int GetDeflagrationCount(void);
sfVector2i GetDeflagrationPosition(int _index);
#endif // BOMB_H