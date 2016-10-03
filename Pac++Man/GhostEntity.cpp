/****************************************************************************
File: GhostEntity.cpp
Author: fookenCode
****************************************************************************/
#include "GhostEntity.h"

GhostEntity::GhostEntity() {
    mGhostIcon = 0x94;
}

/****************************************************************************
Function: reset
Parameter(s): N/A
Output: N/A
Comments: Used to reset the status of the GhostEntity to base.
****************************************************************************/
void GhostEntity::Reset() {
    setMovementSpeed(0);
    setRespawnTimer(0);
    setMovementDirection(MAX_DIRECTION);
    mActive = false;
    mVulnerableStatus = INVULNERABLE;
}

/****************************************************************************
Function: initializeMapObject
Parameter(s): N/A
Output: N/A
Comments: Used internally to create the Character Map for game board.
****************************************************************************/
void GhostEntity::Render() {
    // TODO
}

void GhostEntity::Update() {
    // TODO
}

void GhostEntity::Move() {
    // TODO
}