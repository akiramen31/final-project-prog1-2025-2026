#ifndef BACKUP_H
#define BACKUP_H

#include "Common.h"

typedef enum
{
	LIGHT_LEVEL,
	FLOAT_COUNT

}FloatSave;

typedef enum
{
	FULL_SCREEN,
	CHAR_COUNT

}CharSave;

typedef enum
{
	SCORE,
	LIFE,
	BOMB,
	FIRE,
	SPEED,
	ENNEMY_COUNT,
	LEVEL,
	INT_COUNT

}IntSave;

typedef struct
{
	FILE* saveFile;
	float valueFloat[FLOAT_COUNT];
	char valueChar[CHAR_COUNT];
	char valueInt[INT_COUNT];
}Backup;

void LoadBackup(void);
void SaveBackup(void);

float GetFloatFromSave(FloatSave _index);
void SetFloatToSave(FloatSave _index, float _value);
void AddFloatToSave(FloatSave _index, float _value);

char GetCharFromSave(CharSave _index);
void SetCharToSave(CharSave _index, char _value);
void AddCharToSave(CharSave _index, char _value);

int GetIntFromSave(IntSave _index);
void SetIntToSave(IntSave _index, int _value);
void AddIntToSave(IntSave _index, int _value);

#endif // !BACKUP_H
