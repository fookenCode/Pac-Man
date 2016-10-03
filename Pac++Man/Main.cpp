/****************************************************************************
File: Main.cpp
Author: fookenCode
****************************************************************************/
#include <iostream>
#include <ctime>
using namespace std;
#include <windows.h>
#include "PacGame.h"

int main(int args, char *argv)
{
    system("cls");
    CONSOLE_CURSOR_INFO info;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { 55, 31 };
    SMALL_RECT windowDimensions = { 0,0,54,30 };
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
    
    COORD FrameCounterPosition;
    FrameCounterPosition.X = 18;
    FrameCounterPosition.Y = 30;
    
    int frames = 0;
    clock_t absStart = clock();
    clock_t deltaTime = 0;
    double frameRate = 30;
    double averageFramesPerMS = 33.333;
    double lowestFPS, highestFPS;
    lowestFPS = highestFPS = 0.0;
    while (!GetAsyncKeyState(VK_ESCAPE) && myGame.IsGameRunning())
    {
        clock_t start = clock();
        myGame.GatherGamePlayInput();
        if (GetTickCount() - startTime > 93)
        {
            startTime = GetTickCount();
            //clock_t begin = clock();
            myGame.Update();
            //clock_t end = clock();
            //double elapsed_ms = double(end - begin);
            //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), FrameCounterPosition);
            //cout << ": " << elapsed_ms;
        }
        myGame.Render();
        clock_t end = clock();
        frames++;

        if (end - absStart > CLOCKS_PER_SEC && frames > 10) {
            double fps = (double)frames / ((end - absStart) / CLOCKS_PER_SEC);
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), FrameCounterPosition);
            absStart = end;
            frames = 0;
            cout << "FPS: " << fps;
            if (fps > highestFPS) {
                highestFPS = fps;
            }
            if (fps < lowestFPS || lowestFPS == 0.0) {
                lowestFPS = fps;
            }
        }

    }

    // GAME END
    myGame.RenderStatusText("\033[33;1mbold red text\033[0m");
    COORD Position;
    Position.X = 21;
    Position.Y = 30;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    cout << "\nLowest: " << lowestFPS << " Highest: " << highestFPS << endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}