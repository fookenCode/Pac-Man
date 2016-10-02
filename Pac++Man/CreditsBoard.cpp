#include "CreditsBoard.h"
#include <Windows.h>
#include <iostream>
CreditsBoard::CreditsBoard() : creditTotal(MAX_CREDITS_ALLOWED) {

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
    COORD Position;
    Position.X = xPos;
    Position.Y = yPos;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    std::cout << CREDITS_NAME_TEXT << creditTotal;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void CreditsBoard::Reset() {

}