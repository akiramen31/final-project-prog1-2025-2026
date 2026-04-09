#include "GUI.h"

PauseMenu pauseMenu;

void LoadGUI()
{
	pauseMenu.background = CreateSprite(GetAsset("Assets/Sprites/Pause_background.png"), (sfVector2f){SCREEN_WIDTH/2,  SCREEN_HEIGHT/2}, 8.f, 10);
	sfSprite_setOrigin(pauseMenu.background, (sfVector2f){0});
	sfFont* font = GetAsset("Assets/Fonts/Daydream.otf");

	for (int i = 0; i < 4; i++)
	{
		pauseMenu.buttonText[i] = CreateText(font, (sfVector2f) { 0 }, 8.f, 0);
	}
	sfText_setString(pauseMenu.buttonText[0], "Resume");
	sfText_setString(pauseMenu.buttonText[1], "Settings");
	sfText_setString(pauseMenu.buttonText[2], "Save");
	sfText_setString(pauseMenu.buttonText[3], "Quit");
}

void UpdateGUI(float _dt)
{
	
}

void TogglePauseMenu(int _bool)
{
	if (_bool)
	{
		for (int i = 0; i < 4; i++)
		{
			sfText_setScale(pauseMenu.buttonText[i], (sfVector2f){8, 8});
		}
		
	}
	else
	{

	}
}