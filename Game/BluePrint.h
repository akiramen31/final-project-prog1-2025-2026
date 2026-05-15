#ifndef BLUE_PRINT_H
#define BLUE_PRINT_H

#include "Common.h"

typedef struct BluePrintEntity
{
	void* visuel;
	int type;
}BluePrintEntity;


typedef struct BluePrint
{
	BluePrintEntity* entity;
	int count;
}BluePrint;

void LoadBluePrint(void);
void UpdateBluePrint(void);
void AddBluePrintEntity(InfoZone* _infoZone);


#endif //BLUE_PRINT_H