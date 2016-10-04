#ifndef _MOVING_ENTITY_H_
#define _MOVING_ENTITY_H_

#include "Entity.h"
#include "Constants.h"

class MovingEntity : public Entity {
protected:
    int movementSpeed, movementDirection, maxValidWidth, maxValidHeight;
public:
    MovingEntity() { }
    virtual ~MovingEntity() { }

    int getMovementSpeed() { return movementSpeed; }
    int getMovementDirection() { return movementDirection; }
    int getMaxValidWidth() { return maxValidWidth; }
    int getMaxValidHeight() { return maxValidHeight; }
    void setMovementSpeed(int newSpeed) { movementSpeed = newSpeed; }
    void setMovementDirection(int newDirection) { movementDirection = newDirection; }
    void setMaxValidWidth(int newWidth) { maxValidWidth = newWidth; }
    void setMaxValidHeight(int newHeight) { maxValidHeight = newHeight; }

    virtual void Update(unsigned validDirections, double timeStep) = 0;
    virtual void Move(double timeStep) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
};

#endif //_MOVING_ENTITY_H_