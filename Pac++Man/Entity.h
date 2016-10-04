/****************************************************************************
File: Entity.h
Author: fookenCode
****************************************************************************/
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "Constants.h"

class Entity {
protected:
    double xPos, yPos;
    bool isInvalidated;
public:
    Entity() { }
    virtual ~Entity() { }

    double getXPosition() { return xPos; }
    double getYPosition() { return yPos; }
    bool IsInvalidated() { return isInvalidated; }

    void setXPos(double newXPos) { xPos = newXPos; }
    void setYPos(double newYPos) { yPos = newYPos; }
    void setPosition(double newXPos, double newYPos) { xPos = newXPos; yPos = newYPos; }
    void setInvalidated(bool newValue) { isInvalidated = newValue; }

    virtual void Render() = 0;
    virtual void Reset() = 0;
};
#endif // _ENTITY_H_