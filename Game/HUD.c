#include "HUD.h"

HUD hud;

void LoadHUD()
{
	hud.gauge = CreateSprite(GetAsset("Assets/Sprites/gauge.png"), (sfVector2f) { 0 }, 1.f, 10.f);
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 0, 32, 32 });
}

void UpdateHUD(float _dt)
{
	float max = GetPlayerEnergyInfo(ENERGY_MAX);
	float current = GetPlayerEnergyInfo(ENERGY);
	int row = (current / max)*15;
	 sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 32 * row, 32, 32 });
	sfSprite_setPosition(hud.gauge, GetViewPosition());
}