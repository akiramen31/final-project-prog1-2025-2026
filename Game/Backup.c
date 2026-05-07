#include "Backup.h"

Backup backup;

void LoadBackup(void)
{
	int file = open("Game.sav", 0, 0644);
	if (file == -1)
	{
		return;
	}
	read(file, &backup, sizeof(Backup));
	if (backup.valueFloat[LIGHT_LEVEL] < 0.25f || backup.valueFloat[LIGHT_LEVEL] > 1.f)
	{
		backup.valueFloat[LIGHT_LEVEL] = 1.f;
	}
	close(file);
}

void SaveBackup(void)
{
	int file = open("Game.sav",1, 0644);
	if (file == -1)
	{
		return;
	}
	write(file, &backup, sizeof(Backup));
	close(file);
}

float GetFloatFromSave(FloatSave _index)
{
	return backup.valueFloat[_index];
}
void SetFloatToSave(FloatSave _index, float _value)
{
	backup.valueFloat[_index] = _value;
	SaveBackup();
}
void AddFloatToSave(FloatSave _index, float _value)
{
	backup.valueFloat[_index] += _value;
	SaveBackup();
}

char GetKeyFromSave(KeySave _index)
{
	return backup.valueKey[_index];
}
void SetKeyToSave(KeySave _index, char _value)
{
	backup.valueKey[_index] = _value;
	SaveBackup();
}
char GetMouseKeyFromSave(KeySave _index)
{
	return (backup.valueKey[_index] - sfKeyCount);
}
void SetMouseKeyToSave(KeySave _index, char _value)
{
	backup.valueKey[_index] = _value + sfKeyCount;
	SaveBackup();
}
sfBool IfControlKeyPressed(KeySave _index)
{
	return sfKeyboard_isKeyPressed(backup.valueKey[_index]) || sfMouse_isButtonPressed((backup.valueKey[_index] - sfKeyCount));
}

int GetIntFromSave(IntSave _index)
{
	return backup.valueInt[_index];
}
void SetIntToSave(IntSave _index, int _value)
{
	backup.valueInt[_index] = _value;
	SaveBackup();
}
void AddIntToSave(IntSave _index, int _value)
{
	backup.valueInt[_index] += _value;
	SaveBackup();
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
