#include "PlayerEntity.h"

PlayerEntity::PlayerEntity() {
	// Initialize all Player Character Icons
	playerCharacterIcons[LEFT] = 0x3E;
	playerCharacterIcons[UP] = 0x56;
	playerCharacterIcons[RIGHT] = 0x3C;
	playerCharacterIcons[DOWN] = 0x5E;

	reset();
}

/****************************************************************************
Function: reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the PlayerEntity to base.
****************************************************************************/
void PlayerEntity::reset() {
	currentPlayerIcon = playerCharacterIcons[LEFT];
	setMovementSpeed(0);
	setMovementDirection(MAX_DIRECTION);
	setXPos(DEFAULT_PLAYER_X_POSITION);
	setYPos(DEFAULT_PLAYER_Y_POSITION);
}

void PlayerEntity::render() {
	// TODO
}