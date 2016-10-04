#include "LivesBoard.h"
#include <Windows.h>
#include <iostream>

LivesBoard::LivesBoard() {
    setInvalidated(true);
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
    if (isInvalidated) {
        COORD Position;
        Position.X = (int)xPos;
        Position.Y = (int)yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[40;37;1m" << LIVES_NAME_TEXT;
        Position.Y += 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[33;1m" << LIVES_BOARD_CHARACTER << " x " << livesLeft << "\033[0m";

        // Reset the Invalidated flag
        isInvalidated = false;
    }
}

void LivesBoard::Reset() {
    isInvalidated = false;
}