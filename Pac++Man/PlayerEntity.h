/****************************************************************************
File: PlayerEntity.cpp
Author: fookenCode
****************************************************************************/
#ifndef _PLAYER_ENTITY_H_
#define _PLAYER_ENTITY_H_

#include "MovingEntity.h"

class PlayerEntity : public MovingEntity {
private:
    char playerCharacterIcons[MAX_DIRECTION], currentPlayerIcon;

public:
    PlayerEntity();
    virtual ~PlayerEntity() { }
    virtual void Update();
    virtual void Move();
    virtual void Render();
    virtual void Reset();
    char getPlayerIcon() { return this->currentPlayerIcon; }
    char getIconForDirection(int direction = -1) { return (direction < 0)? 
                                                          this->playerCharacterIcons[getMovementDirection()]
                                                          : this->playerCharacterIcons[direction]; 
    }
};
#endif // _PLAYER_ENTITY_H_