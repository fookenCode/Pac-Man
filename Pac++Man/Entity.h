/****************************************************************************
File: Entity.h
Author: fookenCode
****************************************************************************/
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "Constants.h"

class Entity {
private:
	int movementSpeed, movementDirection, xPos, yPos;
public:
	Entity() { }
	virtual ~Entity() { }
	int getMovementSpeed() { return movementSpeed; }
	int getMovementDirection() { return movementDirection; }
	int getXPosition() { return xPos; }
	int getYPosition() { return yPos; }

	void setMovementSpeed(int newSpeed) { movementSpeed = newSpeed; }
	void setMovementDirection(int newDirection) { movementDirection = newDirection; }
	void setXPos(int newXPos) { xPos = newXPos; }
	void setYPos(int newYPos) { yPos = newYPos; }
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

	virtual void render() = 0;
	virtual void reset() = 0;
};
#endif // _ENTITY_H_