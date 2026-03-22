#include "HUD.h"

HUD hud;

void LoadHUD()
{
	CreateSprite(GetAsset("Assets/Sprites/HUD/HUD.png"), (sfVector2f) { 0 }, 4.f, 10.f);
	//Score
	for (int i = 0; i < 6; i++)
	{
		hud.score[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 776.f+(36 * i), 16.f }, 4.f, 0.f);
		sfSprite_setTextureRect(hud.score[i], (sfIntRect) { 0, 0, 10, 12 });
	}
	//PowerUps
	for (int i = 0; i < 3; i++)
	{
		hud.powerUps[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 152.f + ( 208 * i), 832.f }, 4.f, 0.f);
		sfSprite_setTextureRect(hud.powerUps[i], (sfIntRect) { 10, 0, 10, 12 });
	}
	//Life
	hud.playerLife = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 136.f, 30.f }, 3.f, 0.f);
	sfSprite_setTextureRect(hud.playerLife, (sfIntRect) { 10, 0, 10, 12 });
	//Time
	for (int i = 0; i <2 ; i++)
	{
		hud.time[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 384.f + (36 * i), 16.f }, 4.f, 0.f);
		sfSprite_setTextureRect(hud.time[i], (sfIntRect) { 0, 0, 10, 12 });
		hud.time[3 + i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 476.f + (36* i), 16.f }, 4.f, 0.f);
		sfSprite_setTextureRect(hud.time[3+i], (sfIntRect) { 0, 0, 10, 12 });
	}
	hud.time[2] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 448.f, 16.f }, 4.f, 0.f);
	sfSprite_setTextureRect(hud.time[2], (sfIntRect) { 100, 0, 10, 12 });
}

void UpdateHUD(float _dt, float _timer)
{
	//score
	int scoreTemp = GetIntFromSave(SCORE);
	for (int i = 5; i >= 0; i--)
	{
		sfSprite_setTextureRect(hud.score[i], (sfIntRect) { 10 * (scoreTemp % 10), 0, 10, 12 });
		scoreTemp /= 10;
	}
	//PowerUps
	sfSprite_setTextureRect(hud.powerUps[0], (sfIntRect) { 10 * GetIntFromSave(BOMB), 0, 10, 12 });
	sfSprite_setTextureRect(hud.powerUps[1], (sfIntRect) { 10 * GetIntFromSave(SPEED), 0, 10, 12 });
	sfSprite_setTextureRect(hud.powerUps[2], (sfIntRect) { 10 * GetIntFromSave(FIRE), 0, 10, 12 });

	//PlayerLife
	int lifeTemp = GetIntFromSave(1);
	sfSprite_setTextureRect(hud.playerLife, (sfIntRect) { 10 * lifeTemp, 0, 10, 12 });
	//Time
	int minutesTemp = (int)_timer / 60;
	int secondsUnitTemp = (int)_timer % 10;
	int secondsTenTemp = ((int)_timer % 60 - secondsUnitTemp)/10;
	sfSprite_setTextureRect(hud.time[1], (sfIntRect){10*minutesTemp, 0, 10, 12});
	sfSprite_setTextureRect(hud.time[3], (sfIntRect){10*secondsTenTemp, 0, 10, 12});
	sfSprite_setTextureRect(hud.time[4], (sfIntRect){10*secondsUnitTemp, 0, 10, 12});
}