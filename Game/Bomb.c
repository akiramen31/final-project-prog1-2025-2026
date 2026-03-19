#include "Bomb.h"

sfTexture* bombTexture;
Bomb bombList[NUM_MAX_BOMB];

sfTexture* ExplosionTexture;

unsigned int bombCount = 0;

void LoadBomb(void)
{
	bombCount = 0;

	for (int i = 0; i < NUM_MAX_BOMB; i++)
	{
		bombList[i] = (Bomb){ 0 };
	}

	bombTexture = GetAsset("Assets/Sprites/Bomb/Bomb.png");

	ExplosionTexture = GetAsset("Assets/Sprites/Bomb/Explosion.png");
}

void SpawnBomb(sfVector2i _bombPos)
{
	bombCount++;
	if (bombCount > GetIntToSave(bombCount))
	{
		bombCount--;
		return;
	}
	for (int i = 0; i < bombCount; i++)
	{
		if (bombList[i].placed == sfFalse)
		{
			printf("spawn bomb num %d\n", i + 1);
			bombList[i].bombPosition = _bombPos;

			bombList[i].bombSprite = CreateSprite(bombTexture, (sfVector2f) { 200, 200 }, 4.f, 41);

			sfSprite_setTextureRect(bombList[i].bombSprite, (sfIntRect) { 0, 0, 16, 16 });

			sfSprite_setOrigin(bombList[i].bombSprite, (sfVector2f) { 8, 16 });
			sfSprite_setPosition(bombList[i].bombSprite, TransformVector2iToVector2f(_bombPos));
			bombList[i].placed = sfTrue;
			return;
		}
		else
		{
			if (_bombPos.x == bombList[i].bombPosition.x && _bombPos.y == bombList[i].bombPosition.y)
			{
				bombCount--;
				return;
			}
		}
	}
}

void BlowBomb(void)
{
}

void UpdateBomb(float _dt)
{
	for (int i = 0; i < bombCount; i++)
	{

	}
}

sfBool CheckAtLocationBomb(sfVector2i _pos)
{
	for (int i = 0; i < bombCount; i++)
	{
		if (_pos.x == bombList[i].bombPosition.x && _pos.y == bombList[i].bombPosition.y)
		{
			return sfTrue;
		}
	}
}