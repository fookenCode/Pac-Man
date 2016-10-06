#ifndef _MOVING_ENTITY_H_
#define _MOVING_ENTITY_H_

#include "Entity.h"
#include "Constants.h"

class MovingEntity : public Entity {
protected:
    double movementSpeed;
    int movementDirection, maxValidWidth, maxValidHeight;
public:
    MovingEntity() { }
    virtual ~MovingEntity() { }

    double getMovementSpeed() { return movementSpeed; }
    int getMovementDirection() { return movementDirection; }
    int getMaxValidWidth() { return maxValidWidth; }
    int getMaxValidHeight() { return maxValidHeight; }
    void setMovementSpeed(double newSpeed) { movementSpeed = newSpeed; }
    void setMovementDirection(int newDirection) { movementDirection = newDirection; }
    void setMaxValidWidth(int newWidth) { maxValidWidth = newWidth; }
    void setMaxValidHeight(int newHeight) { maxValidHeight = newHeight; }

    virtual void Update(unsigned validDirections, double timeStep) = 0;
    virtual void Move(double timeStep) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
};

#endif //_MOVING_ENTITY_H_