#include "JetSteam.h"

JetSteam jetSteam;

void LoadJetSteam(void)
{
	MapData* map = GetMapData();
	jetSteam.count = map->jetCount;
	jetSteam.entity = Calloc(jetSteam.count, sizeof(JetSteamEntity));

	sfFloatRect* rectlistAnimation = Calloc(4, sizeof(sfFloatRect));
	rectlistAnimation[0] = (sfFloatRect){ 0.f,24.f, 16.f,8.f };
	rectlistAnimation[1] = (sfFloatRect){ 0.f,16.f, 16.f,16.f };
	rectlistAnimation[2] = (sfFloatRect){ 0.f,8.f, 16.f,24.f };
	rectlistAnimation[3] = (sfFloatRect){ 0.f,0.f, 16.f,32.f };

	for (unsigned i = 0; i < jetSteam.count; i++)
	{
		jetSteam.entity[i] = (JetSteamEntity){ CreateSprite("Assets/Sprites/jetStream.png",(sfVector2f) { map->jet->hitbox.left + map->jet->hitbox.width / 2, map->jet->hitbox.top + map->jet->hitbox.height }, 1.f, 50.f),& map->jet->hitbox , (AdvencedAnimation) { rectlistAnimation , sfTrue, 4, 1.f, 0.f} };
		SetSpriteOriginFoot(jetSteam.entity[i].sprite);
	}
}

void UpdateJetSteam(float _dt)
{
	for (unsigned i = 0; i < jetSteam.count; i++)
	{
		UpdateAdvencedAnimationAndGiveIfStop(jetSteam.entity[i].sprite, &jetSteam.entity[i].animation, _dt);
		sfFloatRect rect = sfSprite_getGlobalBounds(jetSteam.entity[i].sprite);
		if (rect.height <= 8.f)
		{
			rect.height = 0.f;
		}
		*jetSteam.entity[i].rect = rect;
	}
}