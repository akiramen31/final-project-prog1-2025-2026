#ifndef BACKUP_H
#define BACKUP_H

#include "Common.h"

typedef enum FloatSave
{
	LIGHT_LEVEL,
	SOUND_VOLUME,
	FLOAT_COUNT

}FloatSave;

typedef enum KeySave
{
	KEY_JUMP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_GUN,
	KEY_DASH,
	KEY_HIT,
	KEY_SECOND,
	KEY_COUNT

}KeySave;

typedef enum IntSave
{
	UNLOCKED_LEVEL,
	UNLOCKED_WEAPON,
	UNLOCKED_SECONDARY,
	FULL_SCREEN,
	TROPHY,
	DEV_MODE_FLY,
	CURRENT_MUSIC,
	HIGH_SCORE_1,
	HIGH_SCORE_2,
	HIGH_SCORE_3,
	CURRENT_SCORE,
	INT_COUNT

}IntSave;

typedef struct GameData
{
	char levelUnlock;
	char weaponUnlock;
	char secondaryUnlock;
	int score1;
	int score2;
	int score3;
}GameData;

typedef struct GameSave
{
	char** name;
	int count;

	GameData dataActualy;
	int actualy;
}GameSave;

typedef struct Parametre
{
	float valueFloat[FLOAT_COUNT];
	char valueKey[KEY_COUNT];
	int valueInt[INT_COUNT];
}Parametre;

typedef struct Backup
{
	Parametre parametre;
	GameSave gameSave;
}Backup;

void LoadBackup(void);
void SaveBackup(void);

float GetFloatFromSave(FloatSave _index);
void SetFloatToSave(FloatSave _index, float _value);
void AddFloatToSave(FloatSave _index, float _value);

char GetKeyFromSave(KeySave _index);
void SetKeyToSave(KeySave _index, char _value);
char GetMouseKeyFromSave(KeySave _index);
void SetMouseKeyToSave(KeySave _index, char _value);
sfBool IfControlKeyPressed(KeySave _index);

int GetIntFromSave(IntSave _index);
void SetIntToSave(IntSave _index, int _value);
void AddIntToSave(IntSave _index, int _value);

void LoadGameData(int _index);
GameSave* GetGameSave(void);
GameData* GetGameData(void);
void RenameSave(char* _new);
void SaveGameData(void);
void DestroySave(void);
void AddGameSave(void);
char CheckIfSaveExist(char* _newName);
#endif // !BACKUP_H
