/****************************************************************************
File: Entity.h
Author: fookenCode
****************************************************************************/
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "Constants.h"

class Entity {
protected:
    int xPos, yPos;
    bool isInvalidated;
public:
    Entity() { }
    virtual ~Entity() { }

    int getXPosition() { return xPos; }
    int getYPosition() { return yPos; }
    bool IsInvalidated() { return isInvalidated; }

    void setXPos(int newXPos) { xPos = newXPos; }
    void setYPos(int newYPos) { yPos = newYPos; }
    void setPosition(int newXPos, int newYPos) { xPos = newXPos; yPos = newYPos; }
    void setInvalidated(bool newValue) { isInvalidated = newValue; }

    virtual void Render() = 0;
    virtual void Reset() = 0;
};
#endif // _ENTITY_H_