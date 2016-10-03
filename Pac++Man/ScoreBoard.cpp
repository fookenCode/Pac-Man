#include "ScoreBoard.h"
#include <Windows.h>
#include <iostream>
ScoreBoard::ScoreBoard() :scoreTotal(0L){
    setInvalidated(true);
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
    if (isInvalidated) {
        COORD Position;
        Position.X = xPos;
        Position.Y = yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[40;37;1m" << SCORE_NAME_TEXT;
        Position.Y += 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[33;1m" << scoreTotal << "\033[0m";
        
        setInvalidated(false);
    }
}

void ScoreBoard::Reset() {

}