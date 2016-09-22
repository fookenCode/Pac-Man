/****************************************************************************
File: Main.cpp
Author: fookenCode
****************************************************************************/
#include <iostream>
using namespace std;
#include <windows.h>
#include "PacGame.h"

int main(int args, char *argv)
{
	system("cls");
	CONSOLE_CURSOR_INFO info;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = { 80, 31 };
	SMALL_RECT windowDimensions = { 0,0,79,30 };
	SetConsoleScreenBufferSize(hOutput, bufferSize);
	SetConsoleWindowInfo(hOutput, TRUE, &windowDimensions);
	SetConsoleTitleA(TITLE_WINDOW_TEXT);
	// turn the cursor off
	info.bVisible = FALSE;
	info.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &info);

	PacGame myGame;
	int startTime;
	startTime = GetTickCount();

	while (!GetAsyncKeyState(VK_ESCAPE) && myGame.IsGameRunning())
	{
		myGame.GatherGamePlayInput();
		if (GetTickCount() - startTime > 93)
		{
			startTime = GetTickCount();

			myGame.Update();
			
		}

		myGame.Render();
	}

	// GAME END
	COORD Position;
	Position.X = 30;
	Position.Y = 16;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
	cout << GAMEOVER_TEXT;
	Position.X = 21;
	Position.Y = 30;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
	system("PAUSE");
	return EXIT_SUCCESS;
}