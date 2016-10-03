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
    Reset();
}

/****************************************************************************
Function: reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the PlayerEntity to base.
****************************************************************************/
void PlayerEntity::Reset() {
    currentPlayerIcon = playerCharacterIcons[LEFT];
    setInvalidated(true);
    setMovementSpeed(0);
    setMovementDirection(MAX_DIRECTION);
    setXPos(DEFAULT_PLAYER_X_POSITION);
    setYPos(DEFAULT_PLAYER_Y_POSITION);
}

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
        Position.X = xPos + SCREEN_OFFSET_MARGIN;
        Position.Y = yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        std::cout << "\033[33;1m" << this->getIconForDirection() << "\033[0m";
        setInvalidated(false);
    }
    
}

void PlayerEntity::Update() {
    // TODO
}

void PlayerEntity::Move() {
    // TODO
}