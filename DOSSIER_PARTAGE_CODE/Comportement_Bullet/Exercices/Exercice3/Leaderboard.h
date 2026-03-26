#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "Common.h"

#define SCORE_COUNT 10
#define SCORE_MAX 10000

typedef struct Score
{
	unsigned int value;
	sfText* text;
}Score;

void LoadLeaderboard(void);
void DrawLeaderboard(sfRenderWindow* const _renderWindow);
void CleanupLeaderboard(void);

void CreateScore(void);

#endif // !LEADERBOARD_H
