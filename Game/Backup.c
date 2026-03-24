#include "Backup.h"

Backup backup;

void LoadBackup(void)
{
	FILE* file;
	if (fopen_s(&file, "Game.sav", "r") != 0)
	{
		return;
	}

	{
		char buffer[13] = { "Float0 : %f\n" };
		for (int i = 0; i < FLOAT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(file, (const char*)buffer, &backup.valueFloat[i]);
		}
	}
	{
		char buffer[13] = { "Char 0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(file, (const char*)buffer, &backup.valueChar[i]);
		}
	}
	{
		char buffer[13] = { "Int  0 : %d\n" };
		int o = backup.valueInt[0];
		for (int i = 0; i < INT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(file, (const char*)buffer, &backup.valueInt[i]);
		}
		o = backup.valueInt[0];
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

	{
		char buffer[13] = { "Float0 : %f\n" };
		for (int i = 0; i < FLOAT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(file, (const char*)buffer, backup.valueFloat[i]);
		}
	}
	{
		char buffer[13] = { "Char 0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(file, (const char*)buffer, backup.valueChar[i]);
		}
	}
	{
		char buffer[13] = { "Int  0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(file, (const char*)buffer, backup.valueInt[i]);
		}
	}

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

char GetCharFromSave(CharSave _index)
{
	return backup.valueChar[_index];
}
void SetCharToSave(CharSave _index, char _value)
{
	backup.valueChar[_index] = _value;
}
void AddCharToSave(CharSave _index, char _value)
{
	backup.valueChar[_index] += _value;
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
