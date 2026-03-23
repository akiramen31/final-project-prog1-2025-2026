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

float GetFloatToSave(FloatSave _index);
void SetFloatToSave(FloatSave _index, float _value);

char GetCharToSave(CharSave _index);
void SetCharToSave(CharSave _index, char _value);

int GetIntToSave(IntSave _index);
void SetIntToSave(IntSave _index, int _value);

#endif // !BACKUP_H
