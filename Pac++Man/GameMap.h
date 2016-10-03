/****************************************************************************
File: GameMap.h
Author: fookenCode
****************************************************************************/
#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "Constants.h"
#include <vector>
class GameMap {
public:
    struct RenderQueuePosition {
        int xPos, yPos;
        RenderQueuePosition() :xPos(0), yPos(0) { }
        RenderQueuePosition(int xPos, int yPos):xPos(xPos), yPos(yPos) { }
    };
private:
    int mapSizeX, mapSizeY, currentLevel;
    int totalDots;
    char **mapStrings, **unalteredMapStrings;
    std::vector<RenderQueuePosition> renderQueue;
public:

    GameMap();
    virtual ~GameMap();
    bool allocateMapAssetMemory();
    void releaseMapAssetMemory();
    void initializeMapObject();
    bool loadMap();
    void renderMap(bool forceFullRender = false);
    
    int getCurrentLevel() { return currentLevel; }
    void setCurrentLevel(int newLevel) { currentLevel = newLevel; }

    void pushRenderQueuePosition(RenderQueuePosition newPos);

    int getTotalDotsRemaining() { return this->totalDots; }
    inline void decrementDotsRemaining() { this->totalDots--; }
    inline void incrementDotsRemaining() { this->totalDots++; }
    
    bool isWallCharacter(int xPos, int yPos, int wallGroupToTest);
    bool checkForEmptySpace(int xPos, int yPos);
    unsigned getAvailableDirectionsForPosition(int xPos, int yPos);


    void setCharacterAtPosition(char toEnter, int xPos, int yPos);
    char getCharacterAtPosition(int xPos, int yPos);
   
    int getMapWidth() { return mapSizeX; }
    int getMapHeight() { return mapSizeY; }
    inline int getMapEdge() { return mapSizeX - 2; }
};
#endif // _GAME_MAP_H_