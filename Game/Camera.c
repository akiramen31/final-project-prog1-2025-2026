#include "Camera.h"

void MoveCameraSlow(sfVector2f _pos, float _dt)
{
	sfVector2f tempPos = GetViewPosition();
	sfVector2f newPos = { 0 };
	newPos.x = tempPos.x + (tempPos.x - _pos.x) * _dt;
	newPos.y = tempPos.y + (tempPos.y - _pos.y) * _dt;

	printf("%f %f\n", newPos.x, newPos.y);
	SetViewCenter(newPos);

	//sfView_move()
	SetViewCenter(_pos);
}
