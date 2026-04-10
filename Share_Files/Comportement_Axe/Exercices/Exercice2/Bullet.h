#ifndef BULLET_H
#define BULLET_H

#include "Common.h"

#define BULLET_MAX 5
#define BULLET_SPEED 700

typedef struct Bullet
{
	sfSprite* sprite;
	sfVector2f velocity;
}Bullet;

void LoadBullet(void);
void UpdateBullet(sfRenderWindow* _renderWindow, float _dt);
void DrawBullet(sfRenderWindow* const _renderWindow);
void CleanupBullet(void);

unsigned int GetBulletCount(void);
void AddBullet(sfVector2f _position, sfVector2f _bulletDirection);

#endif // !BULLET_H
