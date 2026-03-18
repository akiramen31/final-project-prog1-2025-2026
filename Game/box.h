#ifndef BOX_H
#define BOX_H

#include "Common.h"

#define NB_BOX 40

typedef struct
{
	sfSprite* sprite;
	Animation animation;
}BoxPart;

typedef struct
{
	BoxPart up;
	BoxPart down;
	sfVector2u position;
	sfBool destroy;
}Box;

void LoadBox(void);
void UpdateBox(float _dt);
void SetBoxPosition(sfVector2i _position, int _index);
void DestroyBox(sfVector2i _position);
#endif // !BOX_H
