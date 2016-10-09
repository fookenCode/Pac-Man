#include "PlayerEntity.h"
#include <Windows.h>
#include <iostream>

PlayerEntity::PlayerEntity() {
    // Initialize all Player Character Icons
    playerCharacterIcons[LEFT] = 0x3E;
    playerCharacterIcons[UP] = 0x56;
    playerCharacterIcons[RIGHT] = 0x3C;
    playerCharacterIcons[DOWN] = 0x5E;
    setInvalidated(true);
    setMovementSpeed(MOVING_ENTITY_DEFAULT_SPEED);
    Reset();
}

/****************************************************************************
Function: Reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the PlayerEntity to base.
****************************************************************************/
void PlayerEntity::Reset() {
    currentPlayerIcon = playerCharacterIcons[LEFT];
    setInvalidated(true);
    setMovementDirection(MAX_DIRECTION);
    setXPos(DEFAULT_PLAYER_X_POSITION);
    setYPos(DEFAULT_PLAYER_Y_POSITION);
} // END Reset

/****************************************************************************
Function: Render
Parameter(s): bool - Boolean value on whether the Position needs to be set
prior to rendering the Player character icon.
Output: N/A
Comments: Renders the Player to screen at the Position stored internally.
****************************************************************************/
void PlayerEntity::Render() {
    if (isInvalidated) {
        COORD Position;
        Position.X = (int)xPos + SCREEN_OFFSET_MARGIN;
        Position.Y = (int)yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[33;1m" << this->getIconForDirection() << "\033[0m";
        setInvalidated(false);
    }
} // END Render

/****************************************************************************
Function: Update
Parameter(s): unsigned - Byte that contains all eligible directions to move.
double - Time (in milliseconds) since last update.
Output: N/A
Comments: Evaluates the direction the Player needs to proceed in, and calls
Move() to perform the move.
****************************************************************************/
void PlayerEntity::Update(unsigned validDirections, double timeStep) {
    if (getMovementSpeed() <= 0)
    {
        return;
    }
    
    if (validDirections & (LEFT_BIT << getMovementDirection()))
    {
        Move(timeStep);
    }
} // END Update

/****************************************************************************
Function: Move
Parameter(s): double - Time (in milliseconds) since last update.
Output: N/A
Comments: Moves in the current direction taking into consideration
time elapsed.  Also appropriately sets the character to the Left or Right
edge of the screen on wrap.
****************************************************************************/
void PlayerEntity::Move(double timeStep) {
    int movementDirection = getMovementDirection();
    double movementSpeed = (double) getMovementSpeed() / timeStep;
    double xPos = getXPosition();
    double yPos = getYPosition();
    double nextPos = 0.0;

    setInvalidated(true);
    switch (movementDirection)
    {
    case LEFT:
        nextPos = xPos - movementSpeed;
        if ((int)nextPos <= 0) {
            nextPos = getMaxValidWidth();
        }
        setXPos(nextPos);
        break;
    case RIGHT:
        nextPos = xPos + movementSpeed;
        if ((int)nextPos > getMaxValidWidth()) {
            nextPos = 0.0;
        }
        setXPos(nextPos);
        break;
    case UP:
        nextPos = yPos - movementSpeed;
        setYPos(nextPos);
        break;
    case DOWN:
        nextPos = yPos + movementSpeed;
        setYPos(nextPos);
        break;
    }
} // END Move