#ifndef BOX_H
#define BOX_H

#include "Common.h"

typedef struct BoxEntity
{
	sfSprite* sprite;
	sfFloatRect hitbox;
}BoxEntity;

typedef struct Box
{
	BoxEntity* entity;
	unsigned count;
	sfTexture* texture;
}Box;

void LoadBox(void);
void AddBox(sfVector2f _position);
sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy);
void ReloadBox(void);
#endif //BOX_H