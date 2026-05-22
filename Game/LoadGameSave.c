#include "LoadGameSave.h"

char GetCaractere(sfKeyCode _key);
void ReloadLoadGameSave(void);

LoadGameSave data;

void LoadLoadGameSave(sfFont* _font)
{
	data = (LoadGameSave){ 0 };
	RechargeSaves();
	GameSave* gameSave = GetGameSave();

	for (int i = 0; i < NB_SAVE; i++)
	{
		data.save[i] = CreateText(_font, (sfVector2f) { 500, 400 + i * 50 }, 50.f, 1.f);
		if (i < gameSave->count)
		{
			sfText_setString(data.save[i], gameSave->name[i]);
		}
	}
	for (int i = 0; i < NB_MAP; i++)
	{
		data.score[i] = CreateText(_font, (sfVector2f) { 40, 650 + i * 50 }, 50.f, 1.f);
	}
	data.back = CreateText(_font, (sfVector2f) { 500, 350 }, 50.f, 1.f);
	data.next = CreateText(_font, (sfVector2f) { 700, 350 }, 50.f, 1.f);
	data.newGame = CreateText(_font, (sfVector2f) { 40, 500 }, 50.f, 1.f);
	data.load = CreateText(_font, (sfVector2f) { 40, 450 }, 50.f, 1.f);
	data.destroy = CreateText(_font, (sfVector2f) { 40, 400 }, 50.f, 1.f);
	data.name = CreateText(_font, (sfVector2f) { 40, 330 }, 50.f, 1.f);
	data.rect = CreateRectangleShape((sfFloatRect) { 40, 350, 400, 50 }, sfTransparent, sfWhite, 2.f);
	data.unlocker = CreateText(_font, (sfVector2f) { 40, 600 }, 50.f, 1.f);

	sfText_setString(data.score[0], "Level 1: ");
	sfText_setString(data.score[1], "Level 2: ");
	sfText_setString(data.score[2], "Level 3: ");
	sfText_setString(data.back, "back");
	sfText_setString(data.next, "next");
	sfText_setString(data.newGame, "New");
	sfText_setString(data.load, "Load");
	sfText_setString(data.destroy, "Destroy");
	sfText_setString(data.unlocker, "Unlocked : ");

	SetVisibleLoadGameSave(sfFalse);
}

void KeyPressedLoadGameSave(sfEvent* _event)
{
	if (data.nameTrigger)
	{
		char buffer[11] = { 0 };
		char caractere = GetCaractere(_event->key.code);
		GameSave* gameSave = GetGameSave();
		char* actual = sfText_getString(data.name);
		int size = GetSizeString(actual);
		if (_event->key.code == sfKeyEnter)
		{
			if (GetSizeString(actual) && CompareColor(sfRectangleShape_getFillColor(data.rect), sfGreen))
			{
				RenameSave(actual);
				ReloadLoadGameSave();
			}
			else
			{
				sfText_setString(data.name, gameSave->nameActualy);
				ReloadLoadGameSave();
			}
			data.nameTrigger = 0;
			sfRectangleShape_setFillColor(data.rect, sfTransparent);
		}
		else if (caractere)
		{
			int i = 0;
			while (actual[i])
			{
				buffer[i] = actual[i];
				i++;
			}

			if (caractere > 0 && size < 10)
			{
				buffer[size] = caractere;
				sfText_setString(data.name, buffer);
			}
			else if (caractere == -1 && size > 0)
			{
				buffer[size - 1] = 0;
				sfText_setString(data.name, buffer);
			}
			if ((StringCompare(buffer, gameSave->name[gameSave->actualy]) || !CheckIfSaveExist(buffer)) && GetSizeString(buffer))
			{
				sfRectangleShape_setFillColor(data.rect, sfGreen);
			}
			else
			{
				sfRectangleShape_setFillColor(data.rect, sfRed);
			}
		}

	}
}

int MouseButtonPressedLoadGameSave(sfMouseButtonEvent* _mouseButtonEvent)
{
	GameSave* gameSave = GetGameSave();

	if (gameSave->actualy != -1)
	{
		sfFloatRect rect = sfRectangleShape_getGlobalBounds(data.rect);
		if (rect.left < _mouseButtonEvent->x && rect.left + rect.width > _mouseButtonEvent->x && rect.top < _mouseButtonEvent->y && rect.top + rect.height > _mouseButtonEvent->y)
		{
			sfRectangleShape_setFillColor(data.rect, sfGreen);
			data.nameTrigger = 1;
		}
		else
		{
			data.nameTrigger = 0;
		}

		if (CompareColor(sfWhite, sfText_getColor(data.destroy)))
		{
			if (gameSave->count)
			{
				DestroySave();
				ReloadLoadGameSave();
			}
		}
	}

	if (CompareColor(sfWhite, sfText_getColor(data.newGame)))
	{
		AddGameSave();
		ReloadLoadGameSave();
	}
	else if (CompareColor(sfWhite, sfText_getColor(data.next)))
	{
		data.pageI = (data.pageI + 1) % ((gameSave->count / NB_SAVE) + 1);
		ReloadLoadGameSave();
	}
	else if (CompareColor(sfWhite, sfText_getColor(data.load)))
	{
		if (gameSave->actualy == -1)
		{
			AddGameSave();
#if DEV_MODE
			gameSave->dataActualy.levelUnlock = 3;
			gameSave->dataActualy.weaponUnlock = 7;
			gameSave->dataActualy.secondaryUnlock = 7;
#endif
			ReloadLoadGameSave();
			return 1;
		}
		else
		{
			return 1;
		}
	}
	else if (CompareColor(sfWhite, sfText_getColor(data.back)))
	{
		if (!data.pageI)
		{
			data.pageI = gameSave->count / NB_SAVE;
		}
		else
		{
			data.pageI = (data.pageI - 1) % ((gameSave->count / NB_SAVE) + 1);
		}
		ReloadLoadGameSave();
	}
	else
	{
		for (int i = 0; i < NB_SAVE; i++)
		{
			if (CompareColor(sfWhite, sfText_getColor(data.save[i])))
			{
				LoadGameData(i + NB_SAVE * data.pageI);
				ReloadLoadGameSave();
			}
		}
	}
	return 0;
}

void MouseMovedLoadGameSave(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfVector2f posMouse = { _mouseMovedEvent->x , _mouseMovedEvent->y };
	for (int i = 0; i < NB_SAVE; i++)
	{
		UpdateTextHighlightTextColor(data.save[i], COLOR_ORANGE, sfWhite, posMouse);
	}
	for (int i = 0; i < NB_MAP; i++)
	{
		UpdateTextHighlightTextColor(data.score[i], COLOR_ORANGE, sfWhite, posMouse);
	}
	UpdateTextHighlightTextColor(data.back, COLOR_ORANGE, sfWhite, posMouse);
	UpdateTextHighlightTextColor(data.next, COLOR_ORANGE, sfWhite, posMouse);
	UpdateTextHighlightTextColor(data.destroy, COLOR_ORANGE, sfWhite, posMouse);
	UpdateTextHighlightTextColor(data.load, COLOR_ORANGE, sfWhite, posMouse);
	UpdateTextHighlightTextColor(data.newGame, COLOR_ORANGE, sfWhite, posMouse);
	UpdateTextHighlightTextColor(data.unlocker, COLOR_ORANGE, sfWhite, posMouse);
}

void SetVisibleLoadGameSave(sfBool _visible)
{
	sfColor color = COLOR_ORANGE;
	color.a *= _visible;
	for (int i = 0; i < NB_SAVE; i++)
	{
		sfText_setColor(data.save[i], color);
	}
	for (int i = 0; i < NB_MAP; i++)
	{
		sfText_setColor(data.score[i], color);
	}

	sfText_setColor(data.back, color);
	sfText_setColor(data.next, color);
	sfText_setColor(data.destroy, color);
	sfText_setColor(data.load, color);
	sfText_setColor(data.newGame, color);
	sfText_setColor(data.name, color);
	sfText_setColor(data.unlocker, color);
	sfRectangleShape_setOutlineColor(data.rect, color);
}

char GetCaractere(sfKeyCode _key)
{
	char buffer[14] = { 0 };
	if (_key >= 0 && _key < 26)
	{
		return 'A' + _key;
	}
	else if (_key > 25 && _key < 36)
	{
		return '0' + _key - 26;
	}
	else if (_key == sfKeySpace)
	{
		return ' ';
	}
	else if (_key == sfKeyDelete || _key == sfKeyBackspace)
	{
		return -1;
	}
	else if (_key > 74 && _key < 85)
	{
		return '0' + _key - 75;
	}
	return 0;
}

void ReloadLoadGameSave(void)
{
	GameSave* gameSave = GetGameSave();

	for (int i = 0; i < NB_SAVE; i++)
	{
		if (i + data.pageI * NB_SAVE < gameSave->count)
		{
			sfText_setString(data.save[i], gameSave->name[i + data.pageI * NB_SAVE]);
		}
		else
		{
			sfText_setString(data.save[i], "");
		}
	}

	char bufferNb[11] = { 0 };
	char buffer[30] = { 0 };
	char* bufferL[LEVEL_TEST] = { "Level 1: " , "Level 2: " , "Level 3: " };
	char* temp[2] = { bufferL[0], bufferNb};

	for (int i = 0; i < LEVEL_TEST; i++)
	{
		temp[0] = bufferL[i];
		TransformIntToString(bufferNb, gameSave->dataActualy.score[i]);
		FusionString(buffer, 2, temp);
		sfText_setString(data.score[i], buffer);
		int j = 0;
		while (buffer[j])
		{
			buffer[j] = 0;
			j++;
		}
	}

	temp[0] = "Unlocked: ";
	TransformIntToString(bufferNb, gameSave->dataActualy.levelUnlock);
	FusionString(buffer, 2, temp);
	sfText_setString(data.unlocker, buffer);

	if (gameSave->count && gameSave->actualy != -1 && gameSave->actualy != gameSave->count)
	{
		sfText_setString(data.name, gameSave->name[gameSave->actualy]);
	}
	else
	{
		sfText_setString(data.name, "");
	}
}