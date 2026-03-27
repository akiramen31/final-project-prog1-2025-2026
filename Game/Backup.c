#include "Backup.h"

Backup backup;

void LoadBackup(void)
{
	FILE* file;
	if (fopen_s(&file, "Game.sav", "r") != 0)
	{
		return;
	}

	fread(&backup, sizeof(Backup), 3, file);
	if (backup.valueFloat[LIGHT_LEVEL] < 0.25f || backup.valueFloat[LIGHT_LEVEL] > 1.f)
	{
		backup.valueFloat[LIGHT_LEVEL] = 1.f;
	} 
	fclose(file);
}

void SaveBackup(void)
{
	FILE* file;
	if (fopen_s(&file, "Game.sav", "w") != 0)
	{
		return;
	}

	fwrite(&backup, sizeof(Backup), 3, file);
	fclose(file);
}

float GetFloatFromSave(FloatSave _index)
{
	return backup.valueFloat[_index];
}
void SetFloatToSave(FloatSave _index, float _value)
{
	backup.valueFloat[_index] = _value;
}
void AddFloatToSave(FloatSave _index, float _value)
{
	backup.valueFloat[_index] += _value;
}

char GetKeyFromSave(KeySave _index)
{
	return backup.valueKey[_index];
}
void SetKeyToSave(KeySave _index, char _value)
{
	backup.valueKey[_index] = _value;
}
char GetMouseKeyFromSave(KeySave _index)
{
	return (backup.valueKey[_index] - sfKeyCount);
}
void SetMouseKeyToSave(KeySave _index, char _value)
{
	backup.valueKey[_index] = _value + sfKeyCount;
}

int GetIntFromSave(IntSave _index)
{
	return backup.valueInt[_index];
}
void SetIntToSave(IntSave _index, int _value)
{
	backup.valueInt[_index] = _value;
}
void AddIntToSave(IntSave _index, int _value)
{
	backup.valueInt[_index] += _value;
}

void SetSaveTemp(const void* _buffer, size_t _size, size_t _count)
{
	FILE* file;
	if (fopen_s(&file, "GameTemp.sav", "w") != 0)
	{
		return;
	}

	fwrite(_buffer, _size, _count, file);

	fclose(file);
}


void GetSaveTemp(void* _buffer, size_t _size, size_t _count)
{
	FILE* file;
	if (fopen_s(&file, "GameTemp.sav", "r") != 0)
	{
		return;
	}

	fread(_buffer, _size, _count, file);

	fclose(file);
}
