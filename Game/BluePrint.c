#include "BluePrint.h"

BluePrint bp;

void LoadBluePrint(void)
{
	bp = (BluePrint){ 0 };
	bp.entity = Calloc(1, sizeof(BluePrintEntity));
}

void UpdateBluePrint(void)
{

}

void AddBluePrintEntity(InfoZone* _infoZone)
{
	bp.entity = Realloc(bp.entity, (size_t)(bp.count + 1) * sizeof(BluePrintEntity));

	if (_infoZone->name[0] == '1')
	{
		bp.entity[bp.count].visuel = CreateText(GetAsset(FONT), (sfVector2f) { _infoZone->hitbox.left, _infoZone->hitbox.top }, 10.f, 1.f);
		sfText_setString(bp.entity[bp.count].visuel, "+2500");
		bp.entity[bp.count].type = 2500;
	}
	else if (_infoZone->name[0] == '2')
	{
		bp.entity[bp.count].visuel = CreateText(GetAsset(FONT), (sfVector2f) { _infoZone->hitbox.left, _infoZone->hitbox.top }, 10.f, 1.f);
		sfText_setString(bp.entity[bp.count].visuel, "+5000");
		bp.entity[bp.count].type = 5000;

	}
	else if (_infoZone->name[0] == '3')
	{
		bp.entity[bp.count].visuel = CreateSprite(GetAsset("Assets/Sprites/BluePrint.png"), (sfVector2f) { _infoZone->hitbox.left, _infoZone->hitbox.top }, 1.f, 1.f);
	}
	else if (_infoZone->name[0] == '4')
	{
		bp.entity[bp.count].visuel = CreateText(GetAsset(FONT), (sfVector2f) { _infoZone->hitbox.left, _infoZone->hitbox.top }, 10.f, 1.f);
		sfText_setString(bp.entity[bp.count].visuel, "+1000");
		bp.entity[bp.count].type = 1000;
	}
	else if (_infoZone->name[0] == '5')
	{
		bp.entity[bp.count].visuel = CreateText(GetAsset(FONT), (sfVector2f) { _infoZone->hitbox.left, _infoZone->hitbox.top }, 10.f, 1.f);
		sfText_setString(bp.entity[bp.count].visuel, "+2500");
		bp.entity[bp.count].type = 2500;
	}
	else if (_infoZone->name[0] == '6')
	{

	}
	else if (_infoZone->name[0] == '7')
	{

	}
}