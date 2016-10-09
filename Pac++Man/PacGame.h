/****************************************************************************
File: PacGame.h
Author: fookenCode
TODO: Make class Singleton
****************************************************************************/
#ifndef _PAC_GAME_H_
#define _PAC_GAME_H_
#include <iostream>
using namespace std;
#include <Windows.h>
#include "Constants.h"
#include "RenderEngine.h"

#include "GameMap.h"
#include "GhostEntity.h"
#include "PlayerEntity.h"
#include "ScoreBoard.h"
#include "LivesBoard.h"
#include "CreditsBoard.h"

class PacGame {
public:
    int gameState, lastAISpawnTime, vulnerabilityTimer, restartDelayTimer, ghostMultiplier;
    bool creditInserted;
    
    PlayerEntity mPlayer;
    GhostEntity mGhosts[MAX_ENEMIES];
    GameMap mGameMap;

    ScoreBoard mScoreBoard;
    LivesBoard mLivesBoard;
    CreditsBoard mCreditsBoard;

    PacGame()
    {
        // Initialize all game data
        gameState = ATTRACT;

        mScoreBoard.setPosition(mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN * 2, SCORE_BOARD_HEIGHT_POSITION);

        mLivesBoard.setPosition(mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN * 2, LIVES_BOARD_HEIGHT_POSITION);
        
        mCreditsBoard.setPosition(SCREEN_OFFSET_MARGIN, mGameMap.getMapHeight());

        ghostMultiplier = 1;
        creditInserted = false;

        {
            RenderEngine &inst = RenderEngine::GetInstance();
            inst.InitializeEngine();
        }
        Reset();
        RenderStatusText(PRESS_START_TEXT);
    } // End Constructor

    bool HasCredits() { return (mCreditsBoard.getCreditTotal() > 0); }
    bool IsGameRunning() { return !((gameState == GAME_OVER || gameState == RUNNING) && IsGameOver()); }
    bool IsGameOver()    { return (mLivesBoard.getLivesLeft() < 0); }
    bool IsPaused() { return (gameState == PAUSED); }
    int  getGameState()  { return gameState; }
    void Reset();
    void RestartLevel();
    void PauseGame();
    void Update(double timeStep);
    bool CanMoveInSpecifiedDirection(int direction, int xPos, int yPos, int movementSpeed = 1);
    void TriggerGhostEaten(GhostEntity &entity);
    void TriggerNewLevel();
    void UpdateAICharacters(double timeStep);
    void CheckCollisions();
    void UpdatePlayerCharacter(double timeStep);
    void UpdatePlayerDirection(int direction);
    void setAllGhostsVulnerable(bool status);
    void GatherGamePlayInput();
    void Render();
    void RenderAI();
    void RenderStatusText(const char *stringToDisplay);
    void ClearStatusText();
};

#endif // _PAC_GAME_H_