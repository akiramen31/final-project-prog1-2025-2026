#include "Box.h"

Box box;

void LoadBox(void)
{
	box.count = 0;
	box.entity = Calloc(1, sizeof(BoxEntity));
	box.texture = GetAsset("Assets/Sprites/Aim_V2.png");
}

void AddBox(sfVector2f _position)
{
	box.entity = Realloc(box.entity, (size_t)(box.count + 1) * sizeof(BoxEntity));
	box.entity[box.count].hitbox = (sfFloatRect){ _position.x, _position.y, 16.f, 16.f };
	box.entity[box.count].sprite = CreateSprite(box.texture, _position, 1.f, 1.f);
	box.count++;
}

sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };

	for (unsigned i = 0; i < box.count; i++)
	{		
		if (sfFloatRect_intersects(&_hitbox, &box.entity[i].hitbox, &reaction))
		{
			if (_destroy)
			{
				box.count--;
				DestroyVisualEntity(box.entity[i].sprite);
				box.entity[i] = box.entity[box.count];
				box.entity = Realloc(box.entity, box.count);
			}
			else
			{
				if (reaction.width < reaction.height)
				{
					if (_hitbox.left + vectorMove.x - box.entity[i].hitbox.left < (box.entity[i].hitbox.width - _hitbox.width) / 2.0f)
					{
						vectorMove.x -= reaction.width;
					}
					else
					{
						vectorMove.x += reaction.width;
					}
				}
				else
				{
					if (_hitbox.top + vectorMove.y - box.entity[i].hitbox.top < (box.entity[i].hitbox.height - _hitbox.height) / 2.0f)
					{
						vectorMove.y -= reaction.height;
					}
					else
					{
						vectorMove.y += reaction.height;
					}
				}
				_hitbox.left += vectorMove.x;
				_hitbox.top += vectorMove.y;
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