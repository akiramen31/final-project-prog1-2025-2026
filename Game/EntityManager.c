#include "EntityManager.h"

char* GetFormatAsset(char* _file);
void LoadGeneralAsset(void);
sfBool CompareString(char* _string1, char* _string2);
void* InitPointer(void* _ptr);

EntityManager entityManager;

void LoadEntityManager(void)
{
	entityManager = (EntityManager){ 0 };
	entityManager.assetCount = 0;
	entityManager.soundCount = 0;
	entityManager.generalAssetCount = 0;
	entityManager.listListCount = 0;
	entityManager.callocListCount = 0;
	entityManager.asset = calloc(1, sizeof(AssetEntity));
	entityManager.sound = calloc(1, sizeof(SoundEntity));
	entityManager.visual = NULL;
	entityManager.listList = calloc(1, sizeof(List*));
	entityManager.callocList = calloc(1, sizeof(void*));
	LoadGeneralAsset();
}

void LoadGeneralAsset(void)
{
	entityManager.generalAssetCount = entityManager.assetCount;
}

void Draw(sfRenderWindow* _renderWindow)
{
	sfRenderWindow_clear(_renderWindow, sfBlue);
	VisualEntity* elementActual = entityManager.visual;
	while (elementActual)
	{
		if (elementActual->type == SPRITE)
		{
			sfRenderWindow_drawSprite(_renderWindow, elementActual->ptr, NULL);
		}
		else if (elementActual->type == TEXT)
		{
			sfRenderWindow_drawSprite(_renderWindow, elementActual->ptr, NULL);
		}
		elementActual = elementActual->next;
	}
	sfRenderWindow_display(_renderWindow);
}

void CleanupGlobal(void)
{
	for (int i = 0; i < entityManager.assetCount; i++)
	{
		char* buffer = GetFormatAsset(entityManager.asset[i].file);
		if (CompareString(buffer, ".png"))
		{
			sfTexture_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".wav") || CompareString(buffer, ".ogg"))
		{
			sfSoundBuffer_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".ttf"))
		{
			sfFont_destroy(entityManager.asset[i].ptr);
		}
	}
	free(entityManager.asset);

	while (entityManager.visual)
	{
		entityManager.visual = InitPointer(entityManager.visual);
		if (entityManager.visual->type == SPRITE)
		{
			sfSprite_destroy(entityManager.visual->ptr);
		}
		else if (entityManager.visual->type == TEXT)
		{
			sfText_destroy(entityManager.visual->ptr);
		}
		VisualEntity* temp = entityManager.visual;
		entityManager.visual = (VisualEntity*)entityManager.visual->next;
		free(temp);
	}

	for (int i = 0; i < entityManager.soundCount; i++)
	{
		if (entityManager.sound[i].type == SOUND)
		{
			sfSound_destroy(entityManager.sound[i].ptr);
		}
		else if (entityManager.sound[i].type == MUSIC)
		{
			sfMusic_destroy(entityManager.sound[i].ptr);
		}
	}
	free(entityManager.sound);

	for (int i = 0; i < entityManager.listListCount; i++)
	{
		RemoveList(entityManager.listList[i]);
	}
	free(entityManager.listList);

	for (int i = 0; i < entityManager.callocListCount; i++)
	{
		free(entityManager.callocList[i]);
	}
	free(entityManager.callocList);
}

void CleanupLocal(void)
{
	for (int i = entityManager.generalAssetCount; i < entityManager.assetCount; i++)
	{
		char* buffer = GetFormatAsset(entityManager.asset[i].file);
		if (CompareString(buffer, ".png"))
		{
			sfTexture_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".wav") || CompareString(buffer, ".ogg"))
		{
			sfSoundBuffer_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".ttf"))
		{
			sfFont_destroy(entityManager.asset[i].ptr);
		}
	}
	entityManager.assetCount = entityManager.generalAssetCount;

	if (entityManager.visual)
	{
		while (entityManager.visual->next)
		{
			entityManager.visual->next = InitPointer(entityManager.visual->next);
			if (entityManager.visual->next->type == SPRITE)
			{
				sfSprite_destroy(entityManager.visual->next->ptr);
			}
			else if (entityManager.visual->next->type == TEXT)
			{
				sfText_destroy(entityManager.visual->next->ptr);
			}
			VisualEntity* temp = entityManager.visual->next;
			entityManager.visual->next = (VisualEntity*)entityManager.visual->next->next;
			free(temp);
		}
	}

	for (int i = 0; i < entityManager.soundCount; i++)
	{
		if (entityManager.sound[i].type == SOUND)
		{
			sfSound_destroy(entityManager.sound[i].ptr);
		}
		else if (entityManager.sound[i].type == MUSIC)
		{
			sfMusic_destroy(entityManager.sound[i].ptr);
		}
	}
	entityManager.soundCount = 0;

	for (int i = 0; i < entityManager.listListCount; i++)
	{
		RemoveList(entityManager.listList[i]);
	}
	entityManager.listListCount = 0;

	for (int i = 0; i < entityManager.callocListCount; i++)
	{
		free(entityManager.callocList[i]);
	}
	entityManager.callocListCount = 0;
}

void* GetAsset(char* _file)
{
	for (int i = 0; i < entityManager.assetCount; i++)
	{
		if (CompareString(entityManager.asset[i].file, _file))
		{
			return entityManager.asset[i].ptr;
		}
	}

	int bufferCalcule = entityManager.assetCount + 1;
	AssetEntity* temp = realloc(entityManager.asset, bufferCalcule * sizeof(AssetEntity));
	if (!temp)
	{
		return NULL;
	}

	entityManager.asset = temp;

	char* buffer = GetFormatAsset(_file);
	if (CompareString(buffer, ".png"))
	{
		entityManager.asset[entityManager.assetCount].ptr = sfTexture_createFromFile(_file, NULL);
	}
	else if (CompareString(buffer, ".wav") || CompareString(buffer, ".ogg"))
	{
		entityManager.asset[entityManager.assetCount].ptr = sfSoundBuffer_createFromFile(_file);
	}
	else if (CompareString(buffer, ".ttf"))
	{
		entityManager.asset[entityManager.assetCount].ptr = sfFont_createFromFile(_file);
	}

	int i = 0;
	while (_file[i])
	{
		i++;
	}
	i++;
	buffer = calloc(i, sizeof(char));
	if (!buffer)
	{
		return NULL;
	}
	for (int j = 0; j < i; j++)
	{
		buffer[j] = _file[j];
	}

	entityManager.asset[entityManager.assetCount].file = _file;
	entityManager.assetCount++;
	return entityManager.asset[entityManager.assetCount - 1].ptr;

}

sfSprite* LoadBackground(sfTexture* _texture, float _scale)
{
	if (!entityManager.visual)
	{
		entityManager.visual = calloc(1, sizeof(VisualEntity));
		if (!entityManager.visual)
		{
			return NULL;
		}
		entityManager.visual->ptr = sfSprite_create();
		entityManager.visual->type = SPRITE;
		entityManager.visual->drawPlan = 100000000000000000000000000000000000000.f;
	}
	sfSprite_setTexture(entityManager.visual->ptr, _texture, sfTrue);
	sfSprite_setScale(entityManager.visual->ptr, (sfVector2f) { _scale* GAME_SCALE, _scale* GAME_SCALE });
	return entityManager.visual->ptr;
}

sfSprite* CreateSprite(sfTexture* _texture, sfVector2f _position, float _scale, float _drawPlan)
{
	VisualEntity* newElement = calloc(1, sizeof(VisualEntity));
	if (!newElement)
	{
		return NULL;
	}
	newElement->type = SPRITE;
	newElement->drawPlan = _drawPlan;
	newElement->ptr = sfSprite_create();
	sfSprite_setTexture(newElement->ptr, _texture, sfTrue);
	sfSprite_setPosition(newElement->ptr, _position);
	sfSprite_setScale(newElement->ptr, (sfVector2f) { _scale* GAME_SCALE, _scale* GAME_SCALE });

	VisualEntity* elementPrevious = entityManager.visual;
	while (elementPrevious->next && elementPrevious->next->drawPlan >= _drawPlan)
	{
		elementPrevious = elementPrevious->next;
	}
	newElement->next = elementPrevious->next;
	elementPrevious->next = newElement;
	return newElement->ptr;
}

sfText* CreateText(sfFont* _font, sfVector2f _position, float _scale, float _drawPlan)
{
	VisualEntity* newElement = calloc(1, sizeof(VisualEntity));
	if (!newElement)
	{
		return NULL;
	}
	newElement->type = TEXT;
	newElement->drawPlan = _drawPlan;
	newElement->ptr = sfText_create();
	sfText_setFont(newElement->ptr, _font);
	sfText_setPosition(newElement->ptr, _position);
	sfText_setScale(newElement->ptr, (sfVector2f) { _scale* GAME_SCALE, _scale* GAME_SCALE });

	if (!entityManager.visual)
	{
		entityManager.visual = newElement;
		return newElement->ptr;
	}
	VisualEntity* elementPrevious = entityManager.visual;
	while (elementPrevious->next && elementPrevious->drawPlan >= _drawPlan)
	{
		elementPrevious = elementPrevious->next;
	}
	newElement->next = elementPrevious->next;
	elementPrevious->next = newElement;
	return newElement->ptr;
}

sfSound* CreateSound(sfSoundBuffer* _buffer, float _volume, sfBool _play)
{
	int bufferCalcule = entityManager.soundCount + 1;
	SoundEntity* temp = realloc(entityManager.sound, bufferCalcule * sizeof(SoundEntity));
	if (!temp)
	{
		return NULL;
	}
	entityManager.sound = temp;

	entityManager.sound[entityManager.soundCount].ptr = sfSound_create();
	sfSound_setBuffer(entityManager.sound[entityManager.soundCount].ptr, _buffer);
	sfSound_setVolume(entityManager.sound[entityManager.soundCount].ptr, _volume);
	if (_play)
	{
		sfSound_play(entityManager.sound[entityManager.soundCount].ptr);
	}
	entityManager.sound[entityManager.soundCount].type = SOUND;
	entityManager.soundCount++;
	return entityManager.sound[entityManager.soundCount - 1].ptr;
}

sfMusic* CreateMusic(char* _fileMusic, float _volume, sfBool _play)
{
	int bufferCalcule = entityManager.soundCount + 1;
	SoundEntity* temp = realloc(entityManager.sound, bufferCalcule * sizeof(SoundEntity));
	if (!temp)
	{
		return NULL;
	}
	entityManager.sound = temp;

	entityManager.sound[entityManager.soundCount].ptr = sfMusic_createFromFile(_fileMusic);
	sfSound_setVolume(entityManager.sound[entityManager.soundCount].ptr, _volume);
	if (_play)
	{
		sfMusic_play(entityManager.sound[entityManager.soundCount].ptr);
	}
	entityManager.sound[entityManager.soundCount].type = MUSIC;
	entityManager.soundCount++;
	return entityManager.sound[entityManager.soundCount - 1].ptr;
}

void DestroyVisualEntity(void* _entity)
{
	VisualEntity* elementActual = entityManager.visual;
	VisualEntity* elementNext = (VisualEntity*)elementActual->next;
	while (elementNext)
	{
		if (elementNext->ptr == _entity)
		{
			if (elementNext->type == SPRITE)
			{
				sfSprite_destroy(elementNext->ptr);
			}
			else if (elementNext->type == TEXT)
			{
				sfText_destroy(elementNext->ptr);
			}
			elementActual->next = elementNext->next;
			free(elementNext);
			return;
		}
		elementActual = elementNext;
		elementNext = (VisualEntity*)elementActual->next;
	}
	return;
}

void DestroySoundEntity(void* _entity)
{
	for (int i = 0; i < entityManager.soundCount; i++)
	{
		if (entityManager.sound[i].ptr == _entity)
		{

			if (entityManager.sound[i].type == SOUND)
			{
				sfSound_destroy(entityManager.sound[i].ptr);
			}
			else if (entityManager.sound[i].type == MUSIC)
			{
				sfMusic_destroy(entityManager.sound[i].ptr);
			}
			return;
		}
	}
}

void DestroyAssetEntity(void* _entity)
{
	for (int i = 0; i < entityManager.assetCount; i++)
	{
		if (entityManager.asset[i].ptr == _entity)
		{
			char* buffer = GetFormatAsset(entityManager.asset[i].file);
			if (CompareString(buffer, ".png"))
			{
				sfTexture_destroy(entityManager.asset[i].ptr);
			}
			else if (CompareString(buffer, ".wav") || CompareString(buffer, ".ogg"))
			{
				sfSoundBuffer_destroy(entityManager.asset[i].ptr);
			}
			else if (CompareString(buffer, ".ttf"))
			{
				sfFont_destroy(entityManager.asset[i].ptr);
			}
			return;
		}
	}
}

char* GetFormatAsset(char* _file)
{
	static char buffer[5] = { 0 };

	int i = 0;
	while (_file[i] != '.')
	{
		i++;
	}

	for (int j = 0; j < 5; j++)
	{
		buffer[j] = _file[i + j];
	}

	return buffer;
}

sfBool CompareString(char* _string1, char* _string2)
{
	int i = 0;
	while (_string1[i] == _string2[i])
	{
		if (_string1[i] == '\0')
		{
			return sfTrue;
		}
		else
		{
			i++;
		}
	}
	return sfFalse;
}

void* InitPointer(void* _ptr)
{
	return _ptr;
}



void* Calloc(size_t _count, size_t _size)
{
	entityManager.callocListCount++;
	void** temp = realloc(entityManager.callocList, entityManager.callocListCount * sizeof(SoundEntity));
	if (!temp)
	{
		return NULL;
	}
	entityManager.callocList = temp;
	entityManager.callocList[entityManager.callocListCount - 1];
	return calloc(_count, _size);
}

void* Realloc(void* _block, size_t _size)
{
	void* temp = realloc(_block, _size);
	if (!temp)
	{
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < entityManager.callocListCount; i++)
	{
		if (entityManager.callocList[i] == _block)
		{
			entityManager.callocList[i] = temp;
			return temp;
		}
	}
	return temp;
}

void Free(void* _ptr)
{
	for (int i = 0; i < entityManager.callocListCount; i++)
	{
		if (_ptr == entityManager.callocList[i])
		{
			free(_ptr);

			entityManager.callocListCount--;
			entityManager.callocList[i] = entityManager.callocList[entityManager.callocListCount];
			void** temp = realloc(entityManager.callocList, entityManager.callocListCount * sizeof(SoundEntity));
			if (!temp)
			{
				return;
			}
			entityManager.callocList = temp;
			return;
		}
	}
}

List* CreateList(void)
{
	entityManager.listListCount++;
	List** temp = realloc(entityManager.listList, entityManager.listListCount * sizeof(SoundEntity));
	if (!temp)
	{
		return NULL;
	}
	entityManager.listList = temp;
	entityManager.listList[entityManager.listListCount - 1];

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
	for (int i = GetListSize(_list) - 1; i >= 0; i--)
	{
		RemoveElement(_list, 0);
	}

	free(_list);
	_list = NULL;
}

unsigned int GetListSize(List* _list)
{
	if (!_list)
	{
		return 0;
	}
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
		_element->next = _list->first;
		_list->first = _element;
	}
	else
	{
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement != NULL)
		{
			_element->next = previousElement->next;
			previousElement->next = _element;
		}
	}
}

void RemoveElement(List* _list, unsigned int _index)
{
	if (_index == 0)
	{
		Element* elementToRemove = _list->first;
		_list->first = elementToRemove->next;
		free(elementToRemove);
	}
	else
	{
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement != NULL)
		{
			Element* elementToRemove = previousElement->next;
			if (elementToRemove != NULL)
			{
				previousElement->next = elementToRemove->next;
				free(elementToRemove);
			}
		}
	}
}
