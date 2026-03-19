#include "HUD.h"

sfSprite* score[6];
sfSprite* powerUps[3];
sfSprite* playerLife;



void LoadHUD()
{
	CreateSprite(GetAsset("Assets/Sprites/HUD/HUD.png"), (sfVector2f) { 0 }, 4.f, 10);
	for (int i = 0; i < 6; i++)
	{
		score[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 776+(36 * i), 16 }, 4.f, 0);
		sfSprite_setTextureRect(score[i], (sfIntRect) { 0, 0, 10, 12 });
	}
	for (int i = 0; i < 3; i++)
	{
		powerUps[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 152 + ( 208 * i), 832 }, 4.f, 0);
		sfSprite_setTextureRect(powerUps[i], (sfIntRect) { 10, 0, 10, 12 });
	}
	playerLife = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 136, 30 }, 3.f, 0);
	sfSprite_setTextureRect(playerLife, (sfIntRect) { 10, 0, 10, 12 });
}

void UpdateHUD(float _dt)
{
	//score
	int scoreTemp = GetIntToSave(0);
	for (int i = 0; i < 6; i++)
	{
		int number = (scoreTemp % (int)(pow(10, i + 1))) - (scoreTemp % (int)(pow(10, i)));
		sfSprite_setTextureRect(score[i], (sfIntRect) { 10 * number, 0, 10, 12 });
	}
	//PowerUps
	for (int i = 0; i < 3; i++)
	{
		int number = GetIntToSave(2+i);
		sfSprite_setTextureRect(powerUps[i], (sfIntRect){10 * number, 0, 10, 12});
	}
	//PlayerLife
	int lifeTemp = GetIntToSave(1);
	sfSprite_setTextureRect(playerLife, (sfIntRect) { 10 * lifeTemp, 0, 10, 12 });
}