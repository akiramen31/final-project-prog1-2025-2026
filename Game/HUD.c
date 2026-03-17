#include "HUD.h"

sfSprite* symbols[12];



void LoadHUD()
{
	CreateSprite(GetAsset("Assets/Sprites/HUD/HUD.png"), (sfVector2f){0}, 4.f,  10);
	for (int i = 0; i < 12; i++)
	{
		symbols[i] = CreateSprite(GetAsset("Assets/Sprites/HUD/Numbers.png"), (sfVector2f) { 0 }, 0.f, 0);
		if(i != 11)
		{
			sfSprite_setTextureRect(symbols[i], (sfIntRect) { 10 * i, 0, 10 * (i+1), 12 });
		}
		else
		{
			sfSprite_setTextureRect(symbols[i], (sfIntRect) {110, 0, 134, 12});
		}
	}
}

void UpdateHUD(float _dt)
{

}