#include "GameOver.h"

GameOver gameOver;

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	SetViewZoom(1.f);
	int highScore = 0;
	int score = GetIntFromSave(CURRENT_SCORE);

	switch (GetCurrentMap())
	{
	case LEVEL1:
		highScore = GetIntFromSave(HIGH_SCORE_1);
		break;
	case LEVEL2:
		highScore = GetIntFromSave(HIGH_SCORE_2);
		break;
	case LEVEL3:
		highScore = GetIntFromSave(HIGH_SCORE_3);
		break;
	default:
		break;
	}



}

void PollEventGameOver(sfEvent* _event)
{

}

void UpdateGameOver(float _dt)
{

}
