#include "EntityManager.h"
#include "Menu.h"
#include "Game.h"
#include "GameOver.h"

void LoadEntityManager(void);
void LoadGeneralAsset(void);
void LoadMainData(void);
void CleanupGlobal(void);
void CleanupLocal(void);

char* GetFormatAsset(char* _file);
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
	entityManager.listList = calloc(1, sizeof(List*));
	entityManager.callocList = calloc(1, sizeof(void*));
	entityManager.visual = NULL;
	LoadGeneralAsset();

}

void LoadGeneralAsset(void)
{
	void* ptr = GetAsset("Assets/Fonts/Daydream.otf");
	entityManager.generalAssetCount = entityManager.assetCount;
}

void Draw(void)
{
	sfRenderWindow_clear(entityManager.renderWindow, sfColor_fromRGBA(248, 171, 63, 255));
	//sfRenderWindow_clear(entityManager.renderWindow, sfBlack);

	//background non draw entityManager.visual->next  -> entityManager.visual
	VisualEntity* elementActual = entityManager.visual;
	float lightlevel = GetFloatFromSave(LIGHT_LEVEL);
	sfColor temp = { 0 };
	sfRenderWindow_setVerticalSyncEnabled(GetRenderWindow(), sfTrue);

	while (elementActual)
	{
		if (elementActual->type == SPRITE)
		{
			temp = sfSprite_getColor(elementActual->ptr);
			sfSprite_setColor(elementActual->ptr, (sfColor) { (sfUint8)(temp.r * lightlevel), (sfUint8)(temp.g * lightlevel), (sfUint8)(temp.b * lightlevel), (sfUint8)(temp.a) });
			sfRenderWindow_drawSprite(entityManager.renderWindow, elementActual->ptr, NULL);
			sfSprite_setColor(elementActual->ptr, temp);
		}
		else if (elementActual->type == TEXT)
		{
			temp = sfText_getColor(elementActual->ptr);
			sfText_setColor(elementActual->ptr, (sfColor) { (sfUint8)(temp.r * lightlevel), (sfUint8)(temp.g * lightlevel), (sfUint8)(temp.b * lightlevel), (sfUint8)(temp.a) });
			sfRenderWindow_drawText(entityManager.renderWindow, elementActual->ptr, NULL);
			sfText_setColor(elementActual->ptr, temp);
		}
		elementActual = elementActual->next;
	}

	if (entityManager.gameState == GAME)
	{
		if (DEV_MODE)
		{
			DrawDev(entityManager.renderWindow);
		}
	}

	sfRenderWindow_setView(entityManager.renderWindow, entityManager.view);
	sfRenderWindow_display(entityManager.renderWindow);
}

void CleanupGlobal(void)
{
	CleanupLocal();
	for (int i = 0; i < entityManager.assetCount; i++)
	{
		char* buffer = GetFormatAsset(entityManager.asset[i].file);
		free(entityManager.asset[i].file);
		if (CompareString(buffer, ".png"))
		{
			sfTexture_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".wav") || CompareString(buffer, ".ogg"))
		{
			sfSoundBuffer_destroy(entityManager.asset[i].ptr);
		}
		else if (CompareString(buffer, ".ttf") || CompareString(buffer, ".otf"))
		{
			sfFont_destroy(entityManager.asset[i].ptr);
		}
	}
	free(entityManager.asset);
	free(entityManager.sound);
	free(entityManager.listList);
	free(entityManager.callocList);

	sfRenderWindow_destroy(entityManager.renderWindow);
	sfClock_destroy(entityManager.clock);
	sfView_destroy(entityManager.view);
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
		else if (CompareString(buffer, ".ttf") || CompareString(buffer, ".otf"))
		{
			sfFont_destroy(entityManager.asset[i].ptr);
		}
		free(entityManager.asset[i].file);
	}
	entityManager.assetCount = entityManager.generalAssetCount;

	while (entityManager.visual)
	{
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
	entityManager.soundCount = 0;

	for (int i = 0; i < entityManager.listListCount; i++)
	{
		RemoveList(entityManager.listList[i]);
		entityManager.listList[i] = NULL;
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
	else if (CompareString(buffer, ".ttf") || CompareString(buffer, ".otf"))
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

	entityManager.asset[entityManager.assetCount].file = buffer;
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

sfText* CreateText(sfFont* _font, sfVector2f _position, unsigned _scale, float _drawPlan)
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
	sfText_setCharacterSize(newElement->ptr, _scale * GAME_SCALE);

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
	if (_entity)
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
					elementNext->ptr = NULL;
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
	}
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
	for (int i = entityManager.generalAssetCount; i < entityManager.assetCount; i++)
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
			else if (CompareString(buffer, ".ttf") || CompareString(buffer, ".otf"))
			{
				sfFont_destroy(entityManager.asset[i].ptr);
			}
			entityManager.assetCount--;
			free(entityManager.asset[i].file);
			entityManager.asset[i].ptr = entityManager.asset[entityManager.assetCount].ptr;
			entityManager.asset[i].file = entityManager.asset[entityManager.assetCount].file;
			AssetEntity* temp = realloc(entityManager.asset, entityManager.assetCount * sizeof(AssetEntity));
			if (temp == NULL)
			{
				return;
			}
			entityManager.asset = temp;
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
	entityManager.callocList[entityManager.callocListCount - 1] = calloc(_count, _size);
	return entityManager.callocList[entityManager.callocListCount - 1];
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
	List** temp = realloc(entityManager.listList, (size_t)(entityManager.listListCount + 1) * sizeof(List*));
	if (temp)
	{
		entityManager.listList = temp;
		entityManager.listList[entityManager.listListCount] = calloc(1, sizeof(List));
		if (entityManager.listList[entityManager.listListCount])
		{
			entityManager.listList[entityManager.listListCount]->first = NULL;
		}
		entityManager.listListCount++;
		return entityManager.listList[entityManager.listListCount - 1];
	}
	return NULL;
}

void RemoveList(List* _list)
{
	if (_list)
	{
		Element* elementActualy = _list->first;
		if (elementActualy)
		{
			Element* elementNext = elementActualy->next;

			while (elementNext)
			{
				free(elementActualy);
				elementActualy = elementNext;
				elementNext = elementNext->next;
			}
		}

		for (int i = 0; i < entityManager.listListCount; i++)
		{
			if (_list == entityManager.listList[i])
			{
				free(_list);
				entityManager.listListCount--;
				if (entityManager.listListCount)
				{
					entityManager.listList[i] = entityManager.listList[entityManager.listListCount];
					void** temp = realloc(entityManager.listList, entityManager.listListCount * sizeof(SoundEntity));
					if (!temp)
					{
						return;
					}
					entityManager.listList = (List**)temp;
				}
				return;
			}
		}
	}

}

unsigned GetListSize(List* _list)
{
	unsigned listSize = 0;
	if (_list)
	{
		Element* actualElement = _list->first;
		while (actualElement)
		{
			listSize++;
			actualElement = actualElement->next;
		}
	}
	return listSize;
}

Element* CreateElement(void* _value)
{
	Element* newElement = calloc(1, sizeof(Element));
	if (newElement)
	{
		*newElement = (Element){ _value, NULL };
	}
	return newElement;
}

Element* GetElement(List* _list, unsigned _index)
{
	if (_list)
	{
		Element* actualElement = _list->first;
		for (unsigned i = 0; i < _index && actualElement; i++)
		{
			actualElement = actualElement->next;
		}
		return actualElement;
	}
	return NULL;
}

void InsertElement(List* _list, Element* _element, unsigned _index)
{
	if (_index == 0)
	{
		_element->next = _list->first;
		_list->first = _element;
	}
	else
	{
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement)
		{
			_element->next = previousElement->next;
			previousElement->next = _element;
		}
	}
}

void RemoveElement(List* _list, unsigned _index)
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
		if (previousElement && previousElement->next)
		{
			Element* elementToRemove = previousElement->next;
			previousElement->next = elementToRemove->next;
			free(elementToRemove);
		}
	}
}



void Load(void)
{
	LoadBackup();
	LoadEntityManager();
	LoadMainData();
	SetGameState(MENU);
}

void PollEvent(void)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(entityManager.renderWindow, &event))
	{
		if (event.type == sfEvtClosed)
		{
			sfRenderWindow_close(entityManager.renderWindow);
		}
		else
		{
			switch (entityManager.gameState)
			{
			case MENU:
				PollEventMenu(&event);
				break;
			case GAME:
				PollEventGame(&event);
				break;
			case GAME_OVER:
				PollEventGameOver(&event);
				break;
			default:
				break;
			}
		}
	}
}

void Update(void)
{
	float dt = sfTime_asSeconds(sfClock_restart(entityManager.clock));

	if (dt > 0.05f)
	{
		dt = 0.05f;
	}

	switch (entityManager.gameState)
	{
	case MENU:
		UpdateMenu(dt);
		break;
	case GAME:
		UpdateGame(dt);
		break;
	case GAME_OVER:
		UpdateGameOver(dt);
		break;
	default:
		break;
	}
}

void Cleanup(void)
{
	SaveBackup();
	CleanupGlobal();
}

void LoadMainData(void)
{
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, BPP };

	if (GetIntFromSave(FULL_SCREEN))
	{
		entityManager.renderWindow = sfRenderWindow_create(videoMode, "Game", sfDefaultStyle, NULL);
	}
	else
	{
		entityManager.renderWindow = sfRenderWindow_create(videoMode, "Game", sfFullscreen, NULL);
	}

	sfRenderWindow_setVerticalSyncEnabled(entityManager.renderWindow, sfTrue);

	//sfRenderWindow_setFramerateLimit(entityManager.renderWindow, (unsigned int) { 60 });

	entityManager.clock = sfClock_create();

	entityManager.view = sfView_create();
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	SetViewZoom(1.f);
}

void SetViewCenter(sfVector2f _centre)
{
	sfView_setCenter(entityManager.view, _centre);
}

void SetViewZoom(float _zoom)
{
	sfVector2u windowSize = sfRenderWindow_getSize(entityManager.renderWindow);
	entityManager.viewZoom = _zoom;
	sfView_setSize(entityManager.view, (sfVector2f) { windowSize.x* entityManager.viewZoom, windowSize.y* entityManager.viewZoom });
}

sfVector2f GetViewPosition(void)
{
	sfVector2f center = sfView_getCenter(entityManager.view);
	sfVector2f size = sfView_getSize(entityManager.view);

	return (sfVector2f) { center.x - size.x / 2, center.y - size.y / 2 };
}

sfVector2f GetViewSize(void)
{
	sfVector2f size = sfView_getSize(entityManager.view);

	return size;
}

void MoveView(sfVector2f _move)
{
	sfView_move(entityManager.view, _move);
}

void SetGameState(GameState _gameState)
{
	CleanupLocal();
	entityManager.gameState = _gameState;

	switch (entityManager.gameState)
	{
	case MENU:
		LoadMenu();
		break;
	case GAME:
		LoadGame();
		break;
	case GAME_OVER:
		LoadGameOver();
		break;
	default:
		break;
	}
}

sfRenderWindow* GetRenderWindow(void)
{
	return entityManager.renderWindow;
}

void ChangeFullSceen(void)
{
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, BPP };

	if (GetIntFromSave(FULL_SCREEN))
	{
		sfRenderWindow_destroy(entityManager.renderWindow);
		entityManager.renderWindow = sfRenderWindow_create(videoMode, "Game", sfFullscreen, NULL);
		SetIntToSave(FULL_SCREEN, 0);
	}
	else
	{
		sfRenderWindow_destroy(entityManager.renderWindow);
		entityManager.renderWindow = sfRenderWindow_create(videoMode, "Game", sfDefaultStyle, NULL);
		SetIntToSave(FULL_SCREEN, 1);
	}
}