#ifndef BACKUP_H
#define BACKUP_H

#include "Common.h"

typedef enum FloatSave
{
	LIGHT_LEVEL,
	SOUND_VOLUME,
	FLOAT_COUNT

}FloatSave;

typedef enum CharSave
{
	FULL_SCREEN,
	KEY_JUMP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_GUN,
	KEY_DASH,
	KEY_HIT,
	KEY_SECOND,
	CHAR_COUNT

}CharSave;

typedef enum IntSave
{
	SCORE,
	LIFE,
	INT_COUNT

}IntSave;

typedef struct Backup
{
	float valueFloat[FLOAT_COUNT];
	char valueChar[CHAR_COUNT];
	int valueInt[INT_COUNT];
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

void SetSaveTemp(const void* _buffer, size_t _size, size_t _count);
void GetSaveTemp(void* _buffer, size_t _size, size_t _count);
#endif // !BACKUP_H
