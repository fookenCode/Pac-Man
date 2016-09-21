/****************************************************************************
File: GameMap.h
Author: fookenCode
****************************************************************************/
#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "Constants.h"

class GameMap {
private:
	unsigned int mapSizeX, mapSizeY, currentLevel;
	int totalDots;
	char mapStrings[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH], unalteredMapStrings[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH];

public:
	GameMap();
	virtual ~GameMap() { }

	void initializeMapObject();
	void renderMap();
	int getTotalDotsRemaining() { return this->totalDots; }
	void decrementDotsRemaining() { this->totalDots--; }
	bool isWallCharacter(int xPos, int yPos, int wallGroupToTest);

	void setCharacterAtPosition(char toEnter, int xPos, int yPos);
	char getCharacterAtPosition(int xPos, int yPos);
	int getMapWidth() { return mapSizeX; }
	int getMapHeight() { return mapSizeY; }
};
#endif // _GAME_MAP_H_