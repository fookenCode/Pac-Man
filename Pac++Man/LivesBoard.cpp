#include "LivesBoard.h"
#include <Windows.h>
#include <iostream>

LivesBoard::LivesBoard() {

}

LivesBoard::~LivesBoard() {

}

/****************************************************************************
Function: Render
Parameter(s): N/A
Output: N/A
Comments: Renders the number of Player lives that are currently Active.
****************************************************************************/
void LivesBoard::Render() {
    COORD Position;
    Position.X = xPos;
    Position.Y = yPos;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    std::cout << LIVES_NAME_TEXT;
    Position.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    for (int i = 0; i < livesLeft; ++i)
    {
        std::cout << LIVES_BOARD_CHARACTER << ' ';
    }
    for (int i = livesLeft; i < MAX_VISIBLE_LIVES; ++i) {
        std::cout << ' ';
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void LivesBoard::Reset() {

}