/****************************************************************************
File: GhostEntity.cpp
Author: fookenCode
****************************************************************************/
#include "GhostEntity.h"
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>

GhostEntity::GhostEntity() {
    mGhostIcon = (char)0x94;
    timeToSwitchDir = 0.0;
    mTarget = nullptr;
    setMovementSpeed(MOVING_ENTITY_DEFAULT_SPEED);
    srand(time(NULL));
}

/****************************************************************************
Function: Reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the GhostEntity to base.
****************************************************************************/
void GhostEntity::Reset() {
    mVulnerableStatus = INVULNERABLE; 
    setXPos(DEFAULT_AI_X_POSITION + 2 * (mColor % GREEN));
    setYPos(DEFAULT_AI_Y_POSITION);
    setRespawnTimer(0);
    setMovementDirection(MAX_DIRECTION);
    timeToSwitchDir = 0.0;
    mActive = false;
    setInvalidated(true);
}

/****************************************************************************
Function: Render
Parameter(s): N/A
Output: N/A
Comments: Draws the Ghost entity to the screen at its current X/Y Position.
****************************************************************************/
void GhostEntity::Render() {
    if (isInvalidated) {
        COORD Position;
        Position.X = (int)getXPosition() + SCREEN_OFFSET_MARGIN;
        Position.Y = (int)getYPosition();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), getGhostColor());
        std::cout << getGhostIcon();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        setInvalidated(false);
    }
}

/****************************************************************************
Function: initializeGhost
Parameter(s): N/A
Output: N/A
Comments: Sets all vital fields for the Ghost when activated from the Spawn
          Box.
****************************************************************************/
void GhostEntity::initializeGhost() {
    mActive = true;
    setXPos(AI_BOX_ACTIVE_X_POSITION);
    setYPos(AI_BOX_ACTIVE_Y_POSITION);
    setMovementDirection(LEFT);
    setInvalidated(true);
}

/****************************************************************************
Function: Update
Parameter(s): unsigned - Byte that contains all eligible directions to move.
              double - Time (in milliseconds) since last update.
Output: N/A
Comments: Evaluates the direction the Ghost needs to proceed in, and calls
          Move() to perform the move.
****************************************************************************/
void GhostEntity::Update(unsigned validDirections, double timeStep) {
    bool canMoveCurr = (validDirections & LEFT_BIT << getMovementDirection())?true:false;
    bool canMoveNext = false;
    int nextMoveDir = MAX_DIRECTION;
    timeToSwitchDir -= (int)timeStep;
    
    if (timeToSwitchDir <= 0 && mTarget != nullptr) {
        switch (getMovementDirection()) {
        case LEFT:
        case RIGHT:
            nextMoveDir = (mTarget->getYPosition() - getYPosition() > DBL_EPSILON) ? DOWN : UP;
            canMoveNext = (validDirections & LEFT_BIT << nextMoveDir) ? true : false;
            break;
        case UP:
        case DOWN:
            nextMoveDir = (mTarget->getXPosition() - getXPosition() > DBL_EPSILON) ? RIGHT : LEFT;
            canMoveNext = (validDirections & LEFT_BIT << nextMoveDir) ? true : false;
            break;
        default:
            break;
        };
    
        if (canMoveNext || !canMoveCurr) {
            setMovementDirection(nextMoveDir);
        }

        // Allow Ghost to update only after 10 frames
        timeToSwitchDir = MILLISECONDS_FPS_THRESHOLD * 10;
    }

    
    if (canMoveNext || canMoveCurr) {
        Move(timeStep);
    }
}

/****************************************************************************
Function: Move
Parameter(s): double - Time (in milliseconds) since last update.
Output: N/A
Comments: Moves in the current direction taking into consideration 
time elapsed.  Also appropriately sets the character to the Left or Right
edge of the screen on wrap.
****************************************************************************/
void GhostEntity::Move(double timeStep) {
    int movementDirection = getMovementDirection();
    double movementSpeed = (double)getMovementSpeed() / timeStep;
    double xPos = getXPosition();
    double yPos = getYPosition();
    double nextPos = 0.0;
    setInvalidated(true);

    switch (movementDirection)
    {
    case LEFT:
        nextPos = xPos - movementSpeed;
        if ((int)nextPos <= 0)
        {
            nextPos = getMaxValidWidth();
        }
        setXPos(nextPos);
        break;
    case RIGHT:
        nextPos = xPos + movementSpeed;
        if ((int)nextPos > getMaxValidWidth())
        {
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
    };
}