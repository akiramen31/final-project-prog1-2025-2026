#include "HUD.h"
#include "Camera.h"

HUD hud;

void LoadHUD()
{
	hud = (HUD){ 0 };
	hud.gauge = CreateSprite(GetAsset("Assets/Sprites/gauge.png"), (sfVector2f) { 1710, -10 }, 5.f, -10.f);
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 0, 32, 32 });

	for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
	{
		hud.life[i] = CreateSprite(GetAsset("Assets/Sprites/health_cog.png"), (sfVector2f) { 90 + (32 * i), 90 }, 5.f, -10.f);
	}
}

void LoadBossBar(float* _bossHpAdr, float _maxHealth)
{
	hud.bossBar.bossLifeContainer = CreateSprite(GetAsset("Assets/Sprites/boss_lifebar.png"), (sfVector2f) { 768, 50 }, 0.f, -10.f);
	hud.bossBar.bossLifeBar = CreateSprite(GetAsset("Assets/Sprites/boss_life.png"), (sfVector2f) { 768, 50 }, 0.f, -10.f);
	hud.bossBar.bossLife = _bossHpAdr;
	hud.bossBar.maxBossLife = _maxHealth;
}


void UpdateHUD(float _dt)
{
	//Gauge
	int row = (GetPlayerEnergyInfo(ENERGY) / GetPlayerEnergyInfo(ENERGY_MAX)) * 16;
	if (row < 0)
	{
		row = 0;
	}
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 32 * row, 32, 32 });
	//Life
	int life = GetPlayerLife();
	for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
	{
		sfSprite_setOrigin(hud.life[i], (sfVector2f) { 16, 16 });

		if (life <= i)
		{
			sfSprite_setScale(hud.life[i], (sfVector2f) { 0 });
		}
		else
		{
			sfSprite_setRotation(hud.life[i], 45.f * i);
			sfSprite_setRotation(hud.life[i], 45.f * i);
		}
	}
	//Boss
	if (*hud.bossBar.bossLife != -1)
	{
		sfVector2f visibleScale = (sfVector2f){ 5,5 };
		sfSprite_setScale(hud.bossBar.bossLifeContainer, visibleScale);
		sfSprite_setScale(hud.bossBar.bossLifeBar, visibleScale);
		float lifePercentage = *hud.bossBar.bossLife / hud.bossBar.maxBossLife;
		sfSprite_setTextureRect(hud.bossBar.bossLifeBar, (sfIntRect) { 0, 0, (int) { 113 * lifePercentage }, 8 });
	}
	else
	{
		sfVector2f invisibleScale = (sfVector2f){ 0 };
		sfSprite_setScale(hud.bossBar.bossLifeContainer, invisibleScale);
		sfSprite_setScale(hud.bossBar.bossLifeBar, invisibleScale);
	}
}


void ToggleVisibilityHUD(sfBool _visible)
{
	if (_visible)
	{
		sfVector2f visibleScale = (sfVector2f){ 5,5 };
		for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
		{
			sfSprite_setScale(hud.life[i], visibleScale);
		}
		sfSprite_setScale(hud.gauge, visibleScale);
		if (*hud.bossBar.bossLife !=-1)
		{
			sfSprite_setScale(hud.bossBar.bossLifeBar, visibleScale);
			sfSprite_setScale(hud.bossBar.bossLifeContainer, visibleScale);
		}
	}
	else
	{
		sfVector2f invisibleScale = (sfVector2f){ 0 };
		for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
		{
			sfSprite_setScale(hud.life[i], invisibleScale);
		}
		sfSprite_setScale(hud.gauge, invisibleScale);
		sfSprite_setScale(hud.bossBar.bossLifeBar, invisibleScale);
		sfSprite_setScale(hud.bossBar.bossLifeContainer, invisibleScale);
	}
}
