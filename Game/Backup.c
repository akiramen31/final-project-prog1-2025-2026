#include "Backup.h"

Backup backup;

void LoadBackup(void)
{
	if (fopen_s(&backup.saveFile, "Game.sav", "r") != 0)
	{
		return;
	}

	{
		char buffer[13] = { "Float0 : %f\n" };
		for (int i = 0; i < FLOAT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(backup.saveFile, (const char*)buffer, &backup.valueFloat[i]);
		}
	}
	{
		char buffer[13] = { "Char 0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(backup.saveFile, (const char*)buffer, &backup.valueChar[i]);
		}
	}
	{
		char buffer[13] = { "Int  0 : %d\n" };
		int o = backup.valueInt[0];
		for (int i = 0; i < INT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fscanf_s(backup.saveFile, (const char*)buffer, &backup.valueInt[i]);
		}
		o = backup.valueInt[0];
	}

	fclose(backup.saveFile);
	backup.saveFile = NULL;
}

void SaveBackup(void)
{
	if (fopen_s(&backup.saveFile, "Game.sav", "w") != 0)
	{
		return;
	}

	{
		char buffer[13] = { "Float0 : %f\n" };
		for (int i = 0; i < FLOAT_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(backup.saveFile, (const char*)buffer, backup.valueFloat[i]);
		}
	}
	{
		char buffer[13] = { "Char 0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(backup.saveFile, (const char*)buffer, backup.valueChar[i]);
		}
	}
	{
		char buffer[13] = { "Int  0 : %d\n" };
		for (int i = 0; i < CHAR_COUNT; i++)
		{
			buffer[5] = '0' + i;
			fprintf_s(backup.saveFile, (const char*)buffer, backup.valueInt[i]);
		}
	}

	fclose(backup.saveFile);
	backup.saveFile = NULL;
}

float GetFloatToSave(FloatSave _index)
{
	return backup.valueFloat[_index];
}
void SetFloatToSave(FloatSave _index, float _value)
{
	backup.valueFloat[_index] = _value;
}

char GetCharToSave(CharSave _index)
{
	return backup.valueChar[_index];
}
void SetCharToSave(CharSave _index, char _value)
{
	backup.valueChar[_index] = _value;
}

int GetIntToSave(IntSave _index)
{
	return backup.valueInt[_index];
}
void SetIntToSave(IntSave _index, int _value)
{
	backup.valueInt[_index] = _value;
}