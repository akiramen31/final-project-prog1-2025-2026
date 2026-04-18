#include "Box.h"

Box box;

void LoadBox(void)
{
	box.count = 0;
	box.entity = Calloc(1, sizeof(BoxEntity));
	box.texture = GetAsset("Assets/Sprites/Box.png");
}

void AddBox(sfVector2f _position)
{
	box.entity = Realloc(box.entity, (size_t)(box.count + 1) * sizeof(BoxEntity));
	box.entity[box.count].hitbox = (sfFloatRect){ _position.x, _position.y, 16.f, 16.f };
	box.entity[box.count].sprite = CreateSprite(box.texture, _position, 1.f, 50.f);
	box.entity[box.count].breakSound = CreateSound(GetAsset("Assets/Musics/box_break.ogg"), 5.f, sfFalse);
	box.count++;
}

sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy, int _axis)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };
	sfFloatRect hitbox = { 0 };

	for (unsigned i = 0; i < box.count; i++)
	{
		hitbox = box.entity[i].hitbox;
		if (sfFloatRect_intersects(&_hitbox, &hitbox, &reaction))
		{
			if (_destroy)
			{
				sfSound_play(box.entity[i].breakSound);
				box.count--;
				DestroyVisualEntity(box.entity[i].sprite);
				if (box.count)
				{
					box.entity[i] = box.entity[box.count];
					box.entity = Realloc(box.entity, box.count * sizeof(BoxEntity));
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
					float coliderCenterX = box.entity[i].hitbox.left + (box.entity[i].hitbox.width / 2.0f);

					float push = (hitboxCenterX < coliderCenterX) ? -reaction.width : reaction.width;
					vectorMove.x += push;
					_hitbox.left += push; // Instantly update hitbox for the next loop iteration
				}
				else
				{
					float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.0f);
					float coliderCenterY = box.entity[i].hitbox.top + (box.entity[i].hitbox.height / 2.0f);

					float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : reaction.height;
					vectorMove.y += push;
					_hitbox.top += push; // Instantly update hitbox for the next loop iteration
				}
			}
		}
	}
	return vectorMove;
}

void ReloadBox(void)
{
	for (unsigned i = 0; i < box.count; i++)
	{
		DestroyVisualEntity(box.entity[i].sprite);
	}
	box.count = 0;
}