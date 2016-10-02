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

    void updatePos()
    {
        switch (movementDirection)
        {
        case LEFT:
            xPos -= movementSpeed;
            break;
        case RIGHT:
            xPos += movementSpeed;
            break;
        case UP:
            yPos -= movementSpeed;
            break;
        case DOWN:
            yPos += movementSpeed;
            break;
        default:
            break;
        }
    }

    virtual void Render() = 0;
    virtual void Reset() = 0;
};

#endif //_MOVING_ENTITY_H_