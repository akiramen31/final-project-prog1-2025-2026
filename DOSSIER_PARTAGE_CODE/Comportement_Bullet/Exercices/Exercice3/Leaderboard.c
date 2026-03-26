#include "Leaderboard.h"

sfFont* font;
sfText* titleText;

Score leaderboard[SCORE_COUNT] = { 0 };

void SortLeaderboardValue(void);
void SetTextValue(sfText* _text, unsigned int _rank, unsigned int _value);
void UpdateLeaderboardText(void);

void LoadLeaderboard(void)
{
	font = sfFont_createFromFile("Assets/Fonts/MaxterBoardSt.ttf");

	titleText = sfText_create();
	sfText_setFont(titleText, font);
	sfText_setCharacterSize(titleText, 55);
	sfText_setString(titleText, "LEADERBOARD");
	sfFloatRect textHitbox = sfText_getLocalBounds(titleText);
	sfText_setOrigin(titleText, (sfVector2f) { textHitbox.width / 2, 0 });
	sfText_setPosition(titleText, (sfVector2f) { SCREEN_WIDTH / 2, 25 });

	for (unsigned i = 0; i < SCORE_COUNT; i++)
	{
		leaderboard[i].value = 0;
		leaderboard[i].text = sfText_create();
		sfText_setFont(leaderboard[i].text, font);
		sfText_setCharacterSize(leaderboard[i].text, 50);

		SetTextValue(leaderboard[i].text, i + 1, leaderboard[i].value);

		sfFloatRect textHitbox = sfText_getLocalBounds(leaderboard[i].text);
		sfText_setOrigin(leaderboard[i].text, (sfVector2f) { textHitbox.width / 2, textHitbox.height / 2 });
		sfText_setPosition(leaderboard[i].text, (sfVector2f) { SCREEN_WIDTH / 2, (float)(150 + i * 55) });
	}
}

void DrawLeaderboard(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawText(_renderWindow, titleText, NULL);

	for (unsigned i = 0; i < SCORE_COUNT; i++)
	{
		sfRenderWindow_drawText(_renderWindow, leaderboard[i].text, NULL);
	}
}

void CleanupLeaderboard(void)
{
	sfFont_destroy(font);
	font = NULL;

	sfText_destroy(titleText);
	titleText = NULL;

	for (unsigned i = 0; i < SCORE_COUNT; i++)
	{
		sfText_destroy(leaderboard[i].text);
		leaderboard[i].text = NULL;
	}
}

void SetTextValue(sfText* _text, unsigned int _rank, unsigned int _value)
{
	char textBuffer[11];
	sprintf_s(textBuffer, 11, "%02d - %05d", _rank, _value);
	sfText_setString(_text, textBuffer);
}

void CreateScore(void)
{
	unsigned int newScore = 1 + rand() % SCORE_MAX;

	if (newScore > leaderboard[SCORE_COUNT - 1].value)
	{
		leaderboard[SCORE_COUNT - 1].value = newScore;
		SortLeaderboardValue();
		UpdateLeaderboardText();
	}
}

void SortLeaderboardValue(void)
{
	for (unsigned i = SCORE_COUNT - 1; i >= 1; i--)
	{
		if (leaderboard[i].value > leaderboard[i - 1].value)
		{
			unsigned temp = leaderboard[i].value;
			leaderboard[i].value = leaderboard[i - 1].value;
			leaderboard[i - 1].value = temp;
		}
		else
		{
			return;
		}
	}
}

void UpdateLeaderboardText(void)
{
	for (unsigned i = 0; i < SCORE_COUNT; i++)
	{
		SetTextValue(leaderboard[i].text, i + 1, leaderboard[i].value);
	}
}
