#include "CreditsBoard.h"
#include <Windows.h>
#include <iostream>
CreditsBoard::CreditsBoard() : creditTotal(MAX_CREDITS_ALLOWED) {
    setInvalidated(true);
}

CreditsBoard::~CreditsBoard() {

}

/****************************************************************************
Function: Render
Parameter(s): N/A
Output: N/A
Comments: Renders the number of Credits the Player currently has inserted
at the bottom of the screen.
****************************************************************************/
void CreditsBoard::Render() {
    if (isInvalidated) {
        COORD Position;
        Position.X = (int)xPos;
        Position.Y = (int)yPos;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << CREDITS_NAME_TEXT << creditTotal;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        setInvalidated(false);
    }
}

void CreditsBoard::Reset() {
    // no-op
}