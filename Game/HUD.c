#include "HUD.h"
#include "Camera.h"

HUD hud;

void LoadHUD()
{
	hud.gauge = CreateSprite(GetAsset("Assets/Sprites/gauge.png"), (sfVector2f) { 1710, -10 }, 5.f, -10.f);
	sfSprite_setTextureRect(hud.gauge, (sfIntRect) { 0, 0, 32, 32 });

	for (int i = 0; i < PLAYER_MAX_HEALTH; i++)
	{
		hud.life[i] = CreateSprite(GetAsset("Assets/Sprites/health_cog.png"), (sfVector2f) { 90 + (32 * i), 90 }, 5.f, -10.f);
	}
	LoadBossBar();
}

void LoadBossBar(void)
{
	hud.bossBar.bossLifeContainer = CreateSprite(GetAsset("Assets/Boss/1/Boss_Lifebar_Placeholder.png"), (sfVector2f) { 768 , 50 }, 0.f, -10.f);
	hud.bossBar.bossLifeBar = CreateSprite(GetAsset("Assets/Boss/1/Boss_Life_Placeholder.png"), (sfVector2f) { 768, 50 }, 0.f, -10.f);
}


void UpdateHUD(float _dt)
{
	//Gauge
	int row = (GetPlayerEnergyInfo(ENERGY) / GetPlayerEnergyInfo(ENERGY_MAX)) * 15;
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
	float lifePercentage = *hud.bossBar.bossLife / hud.bossBar.maxBossLife;
	sfSprite_setTextureRect(hud.bossBar.bossLifeBar, (sfIntRect) { 0, 0, (int) { 113 * lifePercentage }, 8 });
}

void SetHpFocus(float* _bossHpAdr)
{
	hud.bossBar.bossLife = _bossHpAdr;
	hud.bossBar.maxBossLife = *_bossHpAdr;
}

void VisibilityBossBar(sfBool _visibility)
{
	if (_visibility)
	{
		sfSprite_setScale(hud.bossBar.bossLifeContainer, (sfVector2f) { 5, 5 });
		sfSprite_setScale(hud.bossBar.bossLifeBar, (sfVector2f) { 5, 5 });
	}
	else
	{
		sfSprite_setScale(hud.bossBar.bossLifeContainer, (sfVector2f) { 0, 0 });
		sfSprite_setScale(hud.bossBar.bossLifeBar, (sfVector2f) { 0, 0 });
	}
}

 