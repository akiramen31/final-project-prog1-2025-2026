#include "Player.h"

Player player;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
}

void MovePlayer(float _dt)
{

}

void KillPlayer(void)
{

}
