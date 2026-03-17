#include "Common.h"

int GetRandomInRange(int _min, int _max)
{
	if (_min > _max)
	{
		int temp = _max;
		_max = _min;
		_min = temp;
	}
	return _min + rand() % (_max - _min + 1);
}

float GetAngleToVector(sfVector2f _vector)
{
	return RadToDeg(atan2f(_vector.y, _vector.x));
}

float RadToDeg(float _angleRad)
{
	return(float)(_angleRad * 180 / (float)M_PI);
}

float DegToRad(float _angleDeg)
{
	return(float)(_angleDeg * M_PI / 180);
}

void UpdateText(sfText* _text, char* _format, char* _string, int _value)
{
	char buffer[50];
	sprintf_s(buffer, sizeof(buffer), _format, _string, _value);
	sfText_setString(_text, buffer);
}

sfBool IsColidingPionHitbox(sfFloatRect* _hitbox, sfVector2f _position)
{
	if (_hitbox->left < _position.x)
	{
		if (_hitbox->top < _position.y)
		{
			if (_hitbox->left + _hitbox->width > _position.x)
			{
				if (_hitbox->top + _hitbox->height > _position.y)
				{
					return sfTrue;
				}
			}
		}
	}
	return sfFalse;
}

sfBool UpdateAnimationAndGiveIfStop(sfSprite* const _sprite, Animation* const _animation, const float _dt)
{
	_animation->timeActualy += _dt;

	sfSprite_setTextureRect(_sprite, _animation->rectActualy);
	if (_animation->timeActualy > _animation->frameDuration)
	{
		_animation->timeActualy = 0.f;
		_animation->rectActualy.left += _animation->rectActualy.width;
		if (_animation->rectActualy.left > (_animation->rectActualy.width * (_animation->frameCount - 1)))
		{
			_animation->rectActualy.left = 0;
			if (!_animation->isLooping)
			{
				return sfTrue;
			}
		}
	}
	return sfFalse;
}



List* CreateList(void)
{
	List* newList = calloc(1, sizeof(List));
	if (newList == NULL)
	{
		exit(EXIT_FAILURE);
	}
	newList->first = NULL;

	return newList;
}

void RemoveList(List* _list)
{
	// Suppression de tous les éléments
	for (int i = GetListSize(_list) - 1; i >= 0; i--)
	{
		RemoveElement(_list, 0);
	}

	// Suppresion de la liste
	free(_list);
	_list = NULL;
}

unsigned int GetListSize(List* _list)
{
	Element* actualElement = _list->first;
	unsigned int listSize = 0;

	while (actualElement != NULL)
	{
		listSize++;
		actualElement = actualElement->next;
	}

	return listSize;
}

Element* CreateElement(void* _value)
{
	Element* newElement = calloc(1, sizeof(Element));
	if (newElement == NULL)
	{
		exit(EXIT_FAILURE);
	}

	newElement->value = _value;
	newElement->next = NULL;

	return newElement;
}

Element* GetElement(List* _list, unsigned int _index)
{
	Element* actualElement = _list->first;
	unsigned int actualIndex = 0;

	while (actualIndex < _index && actualElement != NULL)
	{
		actualIndex++;
		actualElement = actualElement->next;
	}

	return actualElement;
}

void InsertElement(List* _list, Element* _element, unsigned int _index)
{
	if (_index == 0)
	{
		// Le nouvel élément pointe vers l'élément suivant
		_element->next = _list->first;

		// Le début de liste pointe vers le nouvel élément (les autres éléments sont automatiquement décalés)
		_list->first = _element;
	}
	else
	{
		// On récupère l'élément qui précède celui que l'on souhaite insérer
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement != NULL)
		{
			// Le nouvel élément pointe vers l'élément suivant
			_element->next = previousElement->next;

			// L'élément précédent pointe vers le nouvel élément (les autres éléments sont automatiquement décalés)
			previousElement->next = _element;
		}
	}
}

void RemoveElement(List* _list, unsigned int _index)
{
	if (_index == 0)
	{
		// L'élément à supprimer est le 1er élément de la liste
		Element* elementToRemove = _list->first;

		// Le début de la liste pointe vers l'élément suivant
		_list->first = elementToRemove->next;

		// On suppression l'élément souhaité
		free(elementToRemove);
	}
	else
	{
		// On récupère l'élément précédent celui que l'on souhaite supprimer
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement != NULL)
		{
			// On récupère l'élément que l'on souhaite supprimer
			Element* elementToRemove = previousElement->next;
			if (elementToRemove != NULL)
			{
				// L'élément précédent pointe vers l'élément suivant (les autres éléments sont automatiquement décalés)
				previousElement->next = elementToRemove->next;

				// On suppression l'élément souhaité
				free(elementToRemove);
			}
		}
	}
}
