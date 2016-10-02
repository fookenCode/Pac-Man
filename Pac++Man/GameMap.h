/****************************************************************************
File: GameMap.h
Author: fookenCode
****************************************************************************/
#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "Constants.h"

class GameMap {
private:
    int mapSizeX, mapSizeY, currentLevel;
    int totalDots;
    char **mapStrings, **unalteredMapStrings;

public:
    GameMap();
    virtual ~GameMap();
    bool allocateMapAssetMemory();
    void releaseMapAssetMemory();
    void initializeMapObject();
    bool loadMap();
    void renderMap();
    
    int getCurrentLevel() { return currentLevel; }
    void setCurrentLevel(int newLevel) { currentLevel = newLevel; }

    int getTotalDotsRemaining() { return this->totalDots; }
    inline void decrementDotsRemaining() { this->totalDots--; }
    inline void incrementDotsRemaining() { this->totalDots++; }
    
    bool isWallCharacter(int xPos, int yPos, int wallGroupToTest);

    void setCharacterAtPosition(char toEnter, int xPos, int yPos);
    char getCharacterAtPosition(int xPos, int yPos);
    int getMapWidth() { return mapSizeX; }
    int getMapHeight() { return mapSizeY; }
    inline int getMapEdge() { return mapSizeX - 2; }
};
#endif // _GAME_MAP_H_