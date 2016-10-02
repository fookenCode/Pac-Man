#include "ScoreBoard.h"
#include <Windows.h>
#include <iostream>
ScoreBoard::ScoreBoard() :scoreTotal(0L){

}

ScoreBoard::~ScoreBoard() {

}

/****************************************************************************
Function: Render
Parameter(s): N/A
Output: N/A
Comments: Renders the Score information on the right-side of screen.
****************************************************************************/
void ScoreBoard::Render() {
    COORD Position;
    Position.X = xPos;
    Position.Y = yPos;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    std::cout << SCORE_NAME_TEXT;
    Position.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    std::cout << scoreTotal;
}

void ScoreBoard::Reset() {

}