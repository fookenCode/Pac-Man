#ifndef _MOVING_ENTITY_H_
#define _MOVING_ENTITY_H_

#include "Entity.h"
#include "Constants.h"

class MovingEntity : public Entity {
protected:
    int movementSpeed, movementDirection;
public:
    MovingEntity() { }
    virtual ~MovingEntity() { }

    int getMovementSpeed() { return movementSpeed; }
    int getMovementDirection() { return movementDirection; }
    void setMovementSpeed(int newSpeed) { movementSpeed = newSpeed; }
    void setMovementDirection(int newDirection) { movementDirection = newDirection; }

    virtual void Update() = 0;
    virtual void Move() = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
};

#endif //_MOVING_ENTITY_H_