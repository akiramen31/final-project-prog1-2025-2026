#include "Player.h"
#include "Entity.h"

Entity entity;

void LoadEntity(void)
{
	entity = (Entity){ 0 };
	entity.conveyorData.animation.frameCount = 4;
	entity.conveyorData.animation.frameDuration = 0.1f;
	entity.conveyorData.animation.isLooping = sfTrue;
	entity.conveyorData.animation.rectActualy = (sfIntRect){ 0,0,16,16 };
	entity.conveyorData.animation.timeActualy = 0;
	entity.boxData.breakSound = CreateSound(GetAsset("Assets/Musics/box_break.ogg"), 0.1f, sfFalse);
}

void UpdateEntity(float _dt)
{
	sfFloatRect rect = { 0 };
	sfFloatRect playerRect = GetPlayerRect();
	sfFloatRect reaction = { 0 };

	if (entity.conveyorData.count > 0)
	{
		UpdateAnimationAndGiveIfStop(entity.conveyorData.entity[0], &entity.conveyorData.animation, _dt);
		sfIntRect rect = sfSprite_getTextureRect(entity.conveyorData.entity[0]);
		for (int i = 1; i < entity.conveyorData.count; i++)
		{
			sfSprite_setTextureRect(entity.conveyorData.entity[i], rect);
		}
	}

	for (int i = 0; i < entity.bluePrint.count; i++)
	{
		rect = sfSprite_getGlobalBounds(entity.bluePrint.entity[i].sprite);
		if (sfFloatRect_intersects(&playerRect, &rect, NULL))
		{
			MapState map = GetCurrentMap();
			GameData* data = GetGameData();
			if (map == LEVEL1)
			{
				if (entity.bluePrint.entity[i].type == 1)
				{
					if (data->secondaryUnlock & 1)
					{
						AddIntToSave(CURRENT_SCORE, 2500);
					}
					else
					{
						data->secondaryUnlock++;
					}
				}
			}
			else if (map == LEVEL2)
			{
				if (entity.bluePrint.entity[i].type == 1 || entity.bluePrint.entity[i].type == 5)
				{
					AddIntToSave(CURRENT_SCORE, 2500);
				}
				else if (entity.bluePrint.entity[i].type == 2)
				{
					AddIntToSave(CURRENT_SCORE, 5000);
				}
				else if (entity.bluePrint.entity[i].type == 3)
				{
					if (data->weaponUnlock & 2)
					{
						AddIntToSave(CURRENT_SCORE, 2500);
					}
					else
					{
						data->weaponUnlock += 2;
					}
				}
				else if (entity.bluePrint.entity[i].type == 4)
				{
					AddIntToSave(CURRENT_SCORE, 1000);
				}
				else if (entity.bluePrint.entity[i].type == 6)
				{
					if (data->weaponUnlock & 4)
					{
						AddIntToSave(CURRENT_SCORE, 2500);
					}
					else
					{
						data->weaponUnlock += 4;
					}
				}
				else if (entity.bluePrint.entity[i].type == 7)
				{
					if (data->secondaryUnlock & 2)
					{
						AddIntToSave(CURRENT_SCORE, 2500);
					}
					else
					{
						data->secondaryUnlock += 2;
					}
				}
			}

			DestroyVisualEntity(entity.bluePrint.entity[i].sprite);
			entity.bluePrint.count--;
			entity.bluePrint.entity[i] = entity.bluePrint.entity[entity.bluePrint.count];
			entity.bluePrint.entity = Realloc(entity.bluePrint.entity, entity.bluePrint.count * sizeof(BluePrintEntity));
		}
	}

	for (int i = 0; i < entity.jetSteam.count; i++)
	{
		if (entity.jetSteam.entity[i].cooldown < 0.f)
		{
			if (UpdateAnimationAndGiveIfStop(entity.jetSteam.entity[i].sprite, &entity.jetSteam.entity[i].animation, _dt))
			{
				sfSprite_setTextureRect(entity.jetSteam.entity[i].sprite, entity.jetSteam.entity[i].animation.rectActualy);
				entity.jetSteam.entity[i].cooldown = 5.f;
			}
			else
			{
				rect = sfSprite_getGlobalBounds(entity.jetSteam.entity[i].sprite);
				if (sfFloatRect_intersects(&playerRect, &rect, NULL))
				{
					SetPlayerVelocity((sfVector2f) { GetPlayerVelocity().x, -1.0f * (int)(entity.jetSteam.entity[i].animation.timeActualy / entity.jetSteam.entity[i].animation.frameDuration) });
				}
			}
		}
		else
		{
			entity.jetSteam.entity[i].cooldown -= _dt;
		}
	}

	for (int i = 0; i < entity.press.count; i++)
	{
		entity.press.entity[i].timer += _dt;
		if (entity.press.entity[i].timer > 6.f)
		{
			entity.press.entity[i].timer = -0.5f;
		}
		rect = sfSprite_getGlobalBounds(entity.press.entity[i].sprite);
		sfVector2u size = sfTexture_getSize(sfSprite_getTexture(entity.press.entity[i].sprite));
		if (entity.press.entity[i].timer < 0.f)
		{
			sfSprite_setTextureRect(entity.press.entity[i].sprite, (sfIntRect) { 0, (int)(size.y - (1.f + entity.press.entity[i].timer) * size.y), size.x, (int)((1.f + entity.press.entity[i].timer) * size.y) });
			if (rect.left < playerRect.left && rect.left + rect.width > playerRect.left + playerRect.width && rect.top + rect.height > playerRect.top && rect.top < playerRect.top + playerRect.height)
			{
				DamagePlayer(1);
				continue;
			}
		}
		else if (entity.press.entity[i].timer < 3.f)
		{
			sfSprite_setTextureRect(entity.press.entity[i].sprite, (sfIntRect) { 0, (int)(size.y - (6.f - entity.press.entity[i].timer) / 6 * size.y), size.x, (int)((6.f - entity.press.entity[i].timer) / 6 * size.y) });
		}

		if (sfFloatRect_intersects(&rect, &playerRect, &reaction))
		{
			if (reaction.left + reaction.width / 2 < playerRect.left + playerRect.width / 2)
			{
				MovePlayer((sfVector2f) { rect.left + rect.width - playerRect.left, 0.f });
			}
			else
			{
				MovePlayer((sfVector2f) { rect.left - playerRect.left - playerRect.width, 0.f });
			}
		}
	}

	for (int i = 0; i < entity.shadow.count; i++)
	{
		sfCircleShape_setPosition(entity.shadow.entity[i].circle, sfSprite_getPosition(entity.shadow.entity[i].follow));
		sfCircleShape_setRadius(entity.shadow.entity[i].circle, sfSprite_getGlobalBounds(entity.shadow.entity[i].follow).width / 4);
	}
}

void AddBox(sfVector2f _position)
{
	entity.boxData.entity = Realloc(entity.boxData.entity, (size_t)(entity.boxData.count + 1) * sizeof(BoxEntity));
	entity.boxData.entity[entity.boxData.count] = (BoxEntity){ CreateSprite(GetAsset("Assets/Sprites/Box.png"), _position, 1.f, 50.f), (sfFloatRect) { _position.x, _position.y, 16.f, 16.f } };
	entity.boxData.count++;
}

void AddConveyor(sfVector2f _position)
{
	entity.conveyorData.entity = Realloc(entity.conveyorData.entity, (size_t)(entity.conveyorData.count + 1) * sizeof(sfSprite*));
	entity.conveyorData.entity[entity.conveyorData.count] = (sfSprite*){ CreateSprite(GetAsset("Assets/Sprites/conveyor.png"), _position, 1.f, 50.f) };
	entity.conveyorData.count++;
}

void AddBluePrint(sfVector2f _position, int _type)
{
	entity.bluePrint.entity = Realloc(entity.bluePrint.entity, (size_t)(entity.bluePrint.count + 1) * sizeof(BluePrintEntity));
	entity.bluePrint.entity[entity.bluePrint.count] = (BluePrintEntity){ CreateSprite(GetAsset("Assets/Sprites/BluePrint.png"), _position, 1.f, 50.f), _type };
	entity.bluePrint.count++;
}

void AddJetSteam(InfoZone* _infoZone)
{
	entity.jetSteam.entity = Realloc(entity.jetSteam.entity, (size_t)(entity.jetSteam.count + 1) * sizeof(JetSteamEntity));
	entity.jetSteam.entity[entity.jetSteam.count] = (JetSteamEntity){ CreateSprite(GetAsset("Assets/Sprites/jetStream.png"),(sfVector2f) { _infoZone->hitbox.left,_infoZone->hitbox.top }, 1.f, 50.f), (Animation) { (sfIntRect) { 0, 0,16, 32 } , sfFalse, 4, 1.f, 0.f }, 0.f };
	entity.jetSteam.count++;
}

void AddPress(sfVector2f _position)
{
	entity.press.entity = Realloc(entity.press.entity, (size_t)(entity.press.count + 1) * sizeof(PressEntity));
	entity.press.entity[entity.press.count] = (PressEntity){ CreateSprite(GetAsset("Assets/Sprites/press.png"),_position, 1.f, 1.f), (rand() % 30000) / 30000.f * 6.f };
	entity.press.count++;
}

void AddShadow(sfSprite* _entityFollow)
{
	entity.shadow.entity = Realloc(entity.shadow.entity, (size_t)(entity.shadow.count + 1) * sizeof(ShadowEntity));
	sfVector2f pos = sfSprite_getPosition(_entityFollow);
	entity.shadow.entity[entity.shadow.count] = (ShadowEntity){ _entityFollow, CreateCircleShape((sfFloatRect) { pos.x, pos.y, sfSprite_getGlobalBounds(_entityFollow).width, 0 }, (sfColor) { 0, 0, 0, 50 }, sfTransparent, 49.f) };
	entity.shadow.count++;
}

sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy, int _axis)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };
	sfFloatRect hitbox = { 0 };

	for (unsigned i = 0; i < entity.boxData.count; i++)
	{
		hitbox = entity.boxData.entity[i].hitbox;
		if (sfFloatRect_intersects(&_hitbox, &hitbox, &reaction))
		{
			if (_destroy)
			{
				AddIntToSave(CURRENT_SCORE, 10);
				PlaySound(GENERAL_BOX);
				entity.boxData.count--;
				DestroyVisualEntity(entity.boxData.entity[i].sprite);
				if (entity.boxData.count)
				{
					entity.boxData.entity[i] = entity.boxData.entity[entity.boxData.count];
					entity.boxData.entity = Realloc(entity.boxData.entity, entity.boxData.count * sizeof(BoxEntity));
				}

				return (sfVector2f) { 1.f, 1.f };
			}
			else
			{
				int resolveX = 0;

				// Determine which axis to resolve based on the passed parameter
				if (_axis == AXIS_X) {
					resolveX = 1;
				}
				else if (_axis == AXIS_Y) {
					resolveX = 0;
				}
				else {
					// Fallback for AXIS_BOTH (old behavior)
					resolveX = (reaction.width < reaction.height);
				}

				if (resolveX)
				{
					float hitboxCenterX = _hitbox.left + (_hitbox.width / 2.0f);
					float coliderCenterX = entity.boxData.entity[i].hitbox.left + (entity.boxData.entity[i].hitbox.width / 2.0f);

					float push = (hitboxCenterX < coliderCenterX) ? -reaction.width : reaction.width;
					vectorMove.x += push;
					_hitbox.left += push; // Instantly update hitbox for the next loop iteration
				}
				else
				{
					float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.0f);
					float coliderCenterY = entity.boxData.entity[i].hitbox.top + (entity.boxData.entity[i].hitbox.height / 2.0f);

					float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : reaction.height;
					vectorMove.y += push;
					_hitbox.top += push; // Instantly update hitbox for the next loop iteration
				}
			}
		}
	}
	return vectorMove;
}