#include "HUD.h"

HUD hud;

void LoadHUD()
{
	hud.gauge = CreateSprite(GetAsset("Assets/Sprites/gauge.png"), (sfVector2f) { 0 }, 1.f, 10.f);
	for (int i = 0; i < PLAYER_MAX_HEALTH; i++) {
		hud.life[i] = CreateSprite(GetAsset("Assets/Sprites/health_cog.png"), (sfVector2f) { 0 }, 1.f, 10.f);
	}
	
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 0, 32, 32 });
}

void UpdateHUD(float _dt)
{
	//scaling to viewsize
	sfVector2f viewPos = GetViewPosition();
	float xScale = GetViewSize().x / SCREEN_WIDTH;
	float yScale= GetViewSize().y / SCREEN_HEIGHT;
	//Gauge
	float max = GetPlayerEnergyInfo(ENERGY_MAX);
	float current = GetPlayerEnergyInfo(ENERGY);
	int row = (int)((current / max) * 15);
	if (row < 0)
	{
		row = 0;
	} 
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 32 * row, 32, 32 });
	sfVector2f gaugePos = { viewPos.x +(xScale * 1710), viewPos.y + (yScale * -10) };
	sfSprite_setPosition(hud.gauge, gaugePos);
	sfSprite_setScale(hud.gauge, (sfVector2f) { xScale * 5, yScale * 5 });
	//Life
	int life = GetPlayerLife();
	for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
	{
		sfSprite_setOrigin(hud.life[i], (sfVector2f) {16,16});
		sfVector2f lifePos = { viewPos.x + (xScale * (90 +(32 * i))), viewPos.y + (yScale * 90) };
		if (life <= i)
		{
			sfSprite_setScale(hud.life[i], (sfVector2f){0});
		}
		else
		{
			sfSprite_setPosition(hud.life[i], lifePos);
			sfSprite_setScale(hud.life[i], (sfVector2f) { xScale*5, yScale*5 });
			sfSprite_setRotation(hud.life[i], 45.f * i);
		}		
	}
}