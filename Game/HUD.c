#include "HUD.h"

sfSprite* score[6];
sfSprite* powerUps[3];
sfSprite* playerLife;
sfSprite* time[5];



void LoadHUD()
{
	//Score
	CreateSprite(GetAsset("Assets/Sprites/HUD/HUD.png"), (sfVector2f) { 0 }, 4.f, 10);
	for (int i = 0; i < 6; i++)
	{
		score[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 776+(36 * i), 16 }, 4.f, 0);
		sfSprite_setTextureRect(score[i], (sfIntRect) { 0, 0, 10, 12 });
	}
	//PowerUps
	for (int i = 0; i < 3; i++)
	{
		powerUps[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 152 + ( 208 * i), 832 }, 4.f, 0);
		sfSprite_setTextureRect(powerUps[i], (sfIntRect) { 10, 0, 10, 12 });
	}
	//Life
	playerLife = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 136, 30 }, 3.f, 0);
	sfSprite_setTextureRect(playerLife, (sfIntRect) { 10, 0, 10, 12 });
	//Time
	for (int i = 0; i <2 ; i++)
	{
		time[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 384 + (36 * i), 16 }, 4.f, 0);
		sfSprite_setTextureRect(time[i], (sfIntRect) { 0, 0, 10, 12 });
		time[3 + i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 476 + (36* i), 16 }, 4.f, 0);
		sfSprite_setTextureRect(time[3+i], (sfIntRect) { 0, 0, 10, 12 });
	}
	time[2] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 448, 16 }, 4.f, 0);
	sfSprite_setTextureRect(time[2], (sfIntRect) { 100, 0, 10, 12 });
}

void UpdateHUD(float _dt, float _timer)
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
		printf("%d, %d\n", number, i);
		sfSprite_setTextureRect(powerUps[i], (sfIntRect){10 * number, 0, 10, 12});
	}
	//PlayerLife
	int lifeTemp = GetIntToSave(1);
	sfSprite_setTextureRect(playerLife, (sfIntRect) { 10 * lifeTemp, 0, 10, 12 });

	//Time


}