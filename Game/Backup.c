#include "Backup.h"
#include <windows.h>

Backup backup;

void LoadBackup(void)
{
	FILE* file = fopen("Game.sav", "r");
	if (!file)
	{
		return;
	}
	fread(&backup, sizeof(Backup), 1, file);
	if (backup.parametre.valueFloat[LIGHT_LEVEL] < 0.25f || backup.parametre.valueFloat[LIGHT_LEVEL] > 1.f)
	{
		backup.parametre.valueFloat[LIGHT_LEVEL] = 1.f;
	}
	fclose(file);

	backup.gameSave.name = Calloc(1, sizeof(char*));
	WIN32_FIND_DATAA entry = { 0 };
	backup.gameSave.count = 0;
	HANDLE h = FindFirstFileA("Saves.\\*", &entry);
	if (h != -1)
	{
		do {
			if (!(entry.cFileName[0] == '.' && (entry.cFileName[1] == '\0' || (entry.cFileName[1] == '.' && entry.cFileName[2] == '\0'))) && entry.dwFileAttributes == 32)
			{
				backup.gameSave.name = Realloc(backup.gameSave.name, (size_t)(backup.gameSave.count + 1) * sizeof(char*));
				int i = 0;
				while (entry.cFileName[i])
				{
					i++;
				}
				backup.gameSave.name[backup.gameSave.count] = StringCopy(entry.cFileName);
				backup.gameSave.count++;
			}
		} while (FindNextFileA(h, &entry));
		FindClose(h);
	}
	backup.gameSave.actualy = -1;
}

void SaveBackup(void)
{
	FILE* file = fopen("Game.sav", "w");
	if (!file)
	{
		return;
	}
	fwrite(&backup.parametre, sizeof(Backup), 1, file);
	fclose(file);
}

float GetFloatFromSave(FloatSave _index)
{
	return backup.parametre.valueFloat[_index];
}
void SetFloatToSave(FloatSave _index, float _value)
{
	backup.parametre.valueFloat[_index] = _value;
	SaveBackup();
}
void AddFloatToSave(FloatSave _index, float _value)
{
	backup.parametre.valueFloat[_index] += _value;
	SaveBackup();
}

char GetKeyFromSave(KeySave _index)
{
	return backup.parametre.valueKey[_index];
}
void SetKeyToSave(KeySave _index, char _value)
{
	backup.parametre.valueKey[_index] = _value;
	SaveBackup();
}
char GetMouseKeyFromSave(KeySave _index)
{
	return (backup.parametre.valueKey[_index] - sfKeyCount);
}
void SetMouseKeyToSave(KeySave _index, char _value)
{
	backup.parametre.valueKey[_index] = _value + sfKeyCount;
	SaveBackup();
}
sfBool IfControlKeyPressed(KeySave _index)
{
	if (sfRenderWindow_hasFocus(GetRenderWindow()))
	{
		return sfKeyboard_isKeyPressed(backup.parametre.valueKey[_index]) || sfMouse_isButtonPressed((backup.parametre.valueKey[_index] - sfKeyCount));
	}
	return 0;
}

int GetIntFromSave(IntSave _index)
{
	return backup.parametre.valueInt[_index];
}
void SetIntToSave(IntSave _index, int _value)
{
	backup.parametre.valueInt[_index] = _value;
	SaveBackup();
}
void AddIntToSave(IntSave _index, int _value)
{
	backup.parametre.valueInt[_index] += _value;
	SaveBackup();
}

void LoadGameData(int _index)
{
	if (_index < backup.gameSave.count)
	{
		char buffer[40] = { 0 };
		char* temp[2] = { "Saves/" , backup.gameSave.name[_index] };
		FusionString(buffer, 2, temp);
		FILE* file = fopen(buffer, "r");
		if (file)
		{
			fread(&backup.gameSave.dataActualy, sizeof(GameData), 1, file);
			fclose(file);
		}
		backup.gameSave.actualy = _index;
	}
}
GameSave* GetGameSave(void)
{
	return &backup.gameSave;
}
GameData* GetGameData(void)
{
	return &backup.gameSave.dataActualy;
}
void RenameSave(char* _newName)
{
	char buffer[40] = { 0 };
	char buffer2[40] = { 0 };
	char* temp[2] = { "Saves/" , backup.gameSave.name[backup.gameSave.actualy] };
	char* temp2[2] = { "Saves/" , _newName };
	FusionString(buffer, 2, temp);
	FusionString(buffer2, 2, temp2);
	int i = rename(buffer, buffer2);
	Free(backup.gameSave.name[backup.gameSave.actualy]);
	backup.gameSave.name[backup.gameSave.actualy] = StringCopy(_newName);
}
void DestroySave(void)
{
	if (backup.gameSave.actualy != -1)
	{
		char buffer[40] = { 0 };
		char* temp[2] = { "Saves/" , backup.gameSave.name[backup.gameSave.actualy] };
		FusionString(buffer, 2, temp);
		remove(buffer);
		Free(backup.gameSave.name[backup.gameSave.actualy]);
		backup.gameSave.count--;

		for (int i = backup.gameSave.actualy; i < backup.gameSave.count; i++)
		{
			backup.gameSave.name[i] = backup.gameSave.name[i + 1];
		}
	}
}
void SaveGameData(void)
{
	if (backup.gameSave.actualy != -1)
	{
		char buffer[40] = { 0 };
		char* temp[2] = { "Saves/" , backup.gameSave.name[backup.gameSave.actualy] };
		FusionString(buffer, 2, temp);
		FILE* file = fopen(buffer, "w");
		if (file)
		{
			fwrite(&backup.gameSave.dataActualy, sizeof(GameData), 1, file);
			fclose(file);
		}
	}
}
void AddGameSave(void)
{
	char buffer[25] = { 'N','e','w' };
	char nbBuffer[11] = { 0 };
	char flag = 1;
	int count = 1;
	while (flag)
	{
		flag = 0;
		for (int i = 0; i < backup.gameSave.count; i++)
		{
			if (StringCompare(buffer, backup.gameSave.name[i]))
			{
				i = backup.gameSave.count;
				flag = 1;
				count++;
				if (count == 1000000)
				{
					count = 1;
					buffer[0] = 'C';
					buffer[1] = 'o';
					buffer[2] = 'n';
					for (int i = 3; i < 20; i++)
					{
						buffer[i] = 0;
					}
				}
				TransformIntToString(nbBuffer, count);
				buffer[3] = ' ';
				int j = 0;
				while (nbBuffer[j])
				{
					buffer[4 + j] = nbBuffer[j];
					j++;
				}
			}
		}
	}

	backup.gameSave.name = Realloc(backup.gameSave.name, (size_t)(backup.gameSave.count + 1) * sizeof(char*));
	backup.gameSave.name[backup.gameSave.count] = StringCopy(buffer);
	backup.gameSave.actualy = backup.gameSave.count;
	backup.gameSave.count++;
	backup.gameSave.dataActualy = (GameData){ 0 };
	backup.gameSave.dataActualy.levelUnlock = 1;
	backup.gameSave.dataActualy.weaponUnlock = 1;
	char bigBuffer[40] = { 0 };
	char* temp[2] = { "Saves/" , buffer };
	FusionString(bigBuffer, 2, temp);
	FILE* file = fopen(bigBuffer, "w");
	if (file)
	{
		fclose(file);
	}
}

char CheckIfSaveExist(char* _newName)
{
	for (int i = 0; i < backup.gameSave.count; i++)
	{
		if (StringCompare(_newName, backup.gameSave.name[i]))
		{
			return 1;
		}
	}
	return 0;
}