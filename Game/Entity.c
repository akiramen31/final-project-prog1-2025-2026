#include "Entity.h"

Entity entity;

void LoadEntity(void)
{
	entity = (Entity){ 0 };
	entity.boxData.texture = GetAsset("Assets/Sprites/Box.png");
	entity.boxData.breakSound = CreateSound(GetAsset("Assets/Musics/box_break.ogg"), 0.1f, sfFalse);
	entity.conveyorData.texture = GetAsset("Assets/Sprites/conveyor.png");
}

void UpdateEntity(float _dt)
{
	for (int i = 0; i < entity.conveyorData.count; i++)
	{
		UpdateAnimationAndGiveIfStop(entity.conveyorData.entity[i].sprite, &entity.conveyorData.entity[i].animation, _dt);
	}
}

void AddBox(sfVector2f _position)
{
	entity.boxData.entity = Realloc(entity.boxData.entity, (size_t)(entity.boxData.count + 1) * sizeof(BoxEntity));
	entity.boxData.entity[entity.boxData.count].hitbox = (sfFloatRect){ _position.x, _position.y, 16.f, 16.f };
	entity.boxData.entity[entity.boxData.count].sprite = CreateSprite(entity.boxData.texture, _position, 1.f, 50.f);
	entity.boxData.count++;
}

void AddConveyor(sfVector2f _position)
{
	entity.conveyorData.entity = Realloc(entity.conveyorData.entity, (size_t)(entity.conveyorData.count + 1) * sizeof(ConveyorEntity));
	entity.conveyorData.entity[entity.conveyorData.count].sprite = CreateSprite(entity.conveyorData.texture, _position, 1.f, 50.f);

	entity.conveyorData.entity[entity.conveyorData.count].animation.frameCount = 4;
	entity.conveyorData.entity[entity.conveyorData.count].animation.frameDuration = 0.1f;
	entity.conveyorData.entity[entity.conveyorData.count].animation.isLooping = sfTrue;
	entity.conveyorData.entity[entity.conveyorData.count].animation.rectActualy = (sfIntRect){ 0,0,16,16 };
	entity.conveyorData.entity[entity.conveyorData.count].animation.timeActualy = 0;

	entity.conveyorData.count++;
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
				sfSound_play(entity.boxData.breakSound);
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

void ReloadEntity(void)
{
	for (unsigned i = 0; i < entity.boxData.count; i++)
	{
		DestroyVisualEntity(entity.boxData.entity[i].sprite);
	}
	entity.boxData.count = 0;

	for (unsigned i = 0; i < entity.conveyorData.count; i++)
	{
		DestroyVisualEntity(entity.conveyorData.entity[i].sprite);
	}
	entity.conveyorData.count = 0;
}