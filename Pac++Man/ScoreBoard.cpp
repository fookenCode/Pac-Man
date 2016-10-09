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
        Position.X = (int)xPos;
        Position.Y = (int)yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[40;37;1m" << SCORE_NAME_TEXT;
        Position.Y += 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[33;1m" << scoreTotal << "\033[0m";
        
        setInvalidated(false);
    }
} // END Render

/****************************************************************************
Function: Reset
Parameter(s): N/A
Output: N/A
Comments: Resets the internal score to Zero for next game.
****************************************************************************/
void ScoreBoard::Reset() {
    this->scoreTotal = 0L;
    COORD Position;
    Position.X = (int)xPos;
    Position.Y = (int)yPos+1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    std::cout << CLEAR_STATUS_TEXT;
    setInvalidated(true);
} // END Reset