/****************************************************************************
File: GhostEntity.cpp
Author: fookenCode
****************************************************************************/
#include "GhostEntity.h"
#include <Windows.h>
#include <iostream>
GhostEntity::GhostEntity() {
    mGhostIcon = (char)0x94;
    mTarget = nullptr;
}

/****************************************************************************
Function: reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the GhostEntity to base.
****************************************************************************/
void GhostEntity::Reset() {
    mVulnerableStatus = INVULNERABLE; 
    setXPos(DEFAULT_AI_X_POSITION + 2 * (mColor % GREEN));
    setYPos(DEFAULT_AI_Y_POSITION);
    setMovementSpeed(0);
    setRespawnTimer(0);
    setMovementDirection(MAX_DIRECTION);
    mActive = false;
    setInvalidated(true);
}

/****************************************************************************
Function: initializeMapObject
Parameter(s): N/A
Output: N/A
Comments: Used internally to create the Character Map for game board.
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

void GhostEntity::initializeGhost() {
    mActive = true;
    setXPos(AI_BOX_ACTIVE_X_POSITION);
    setYPos(AI_BOX_ACTIVE_Y_POSITION);
    setMovementDirection(LEFT);
    setMovementSpeed(1);
    setInvalidated(true);
}


void GhostEntity::Update(unsigned validDirections, double timeStep) {
    bool canMoveCurr = (validDirections & LEFT_BIT << getMovementDirection())?true:false;
    bool canMoveNext = false;
    int nextMoveDir = MAX_DIRECTION;
    if (mTarget != nullptr) {
        switch (getMovementDirection()) {
        case LEFT:
        case RIGHT:
            nextMoveDir = (getYPosition() < mTarget->getYPosition()) ? DOWN : UP;
            canMoveNext = (validDirections & LEFT_BIT << nextMoveDir) ? true : false;
            break;
        case UP:
        case DOWN:
            nextMoveDir = (getXPosition() < mTarget->getXPosition()) ? RIGHT : LEFT;
            canMoveNext = (validDirections & LEFT_BIT << nextMoveDir) ? true : false;
            break;
        default:
            break;
        };
    }
    if (canMoveNext || !canMoveCurr) {
        setMovementDirection(nextMoveDir);
    }
    if (canMoveNext || canMoveCurr) {
        Move(timeStep);
    }
}

void GhostEntity::Move(double timeStep) {
    int movementDirection = getMovementDirection();
    double movementSpeed = (double)getMovementSpeed() * timeStep;
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