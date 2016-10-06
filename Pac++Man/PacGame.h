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
    int commandQueue, gameState, lastAISpawnTime, vulnerabilityTimer, restartDelayTimer, ghostMultiplier;
    bool switchedSides, creditInserted;
    
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
        commandQueue = -1;
        creditInserted = false;
        switchedSides = false;

        {
            RenderEngine &inst = RenderEngine::GetInstance();
            inst.InitializeEngine();
        }
        Reset();
        RenderStatusText(PRESS_START_TEXT);
    } // End Constructor

    void SidesReset()    { switchedSides = false; }
    bool IsGameRunning() { return !((gameState == WINNING || gameState == RUNNING) && IsGameOver()); }
    bool IsGameOver()    { return (mGameMap.getTotalDotsRemaining() <= 0 || mLivesBoard.getLivesLeft() < 0); }
    bool IsGhostAtPosition(int xPos, int yPos) {
        bool returnVal = false;
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive()) {
                continue;
            }
            if (mGhosts[i].getXPosition() == xPos && mGhosts[i].getYPosition() == yPos) {
                return true;
            }
        }
        return returnVal;
    }
    int  getGameState()  { return gameState; }

    /****************************************************************************
    Function: Reset
    Parameter(s): N/A
    Output: N/A
    Comments: Reset Game States and re-render all objects to screen
    ****************************************************************************/
    void Reset()
    {
        ghostMultiplier = 1;
        vulnerabilityTimer = 0;

        // Initialize Player object and status
        mPlayer.setMaxValidWidth(mGameMap.getMapEdge());
        mPlayer.Reset();

        // Initialize all AI objects and status
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            mGhosts[i].setGhostColor(GREEN + i);
            mGhosts[i].Reset();
            mGhosts[i].setMaxValidWidth(mGameMap.getMapEdge());
            if (i == 0) {
                mGhosts[i].setTarget(&mPlayer);
                mGhosts[i].initializeGhost();
            }
        }

        COORD Position;
        Position.X = 0;
        Position.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        // Render all components
        mGameMap.renderMap(true);
        mScoreBoard.Render();
        mLivesBoard.Render();
        mCreditsBoard.Render();
        RenderAI();
        mPlayer.Render();
    } // END Reset

    /****************************************************************************
    Function: RestartLevel
    Parameter(s): N/A
    Output: N/A
    Comments: Called when Player is caught by Ghosts or Level Completes
    ****************************************************************************/
    void RestartLevel() {
        mGameMap.setCharacterAtPosition(' ', (int)mPlayer.getXPosition(), (int)mPlayer.getYPosition());
        Reset();        
        restartDelayTimer = GetTickCount();
        mLivesBoard.decLives();

        if (mLivesBoard.getLivesLeft() >= 0) {
            RenderStatusText(READY_TEXT);
            gameState = READY;
        }
        else {
            gameState = WINNING;
        }

    } // END RestartLevel

    /****************************************************************************
    Function: PauseGame
    Parameter(s): N/A
    Output: N/A
    Comments: Moves Game State between PAUSED and RUNNING states to allow for
              users to take a break.
    ****************************************************************************/
    void PauseGame() {
        if (gameState == RUNNING) {
            gameState = PAUSED;
            RenderStatusText(PAUSED_TEXT);
        }
        else if (gameState == PAUSED) {
            gameState = RUNNING;
            lastAISpawnTime = GetTickCount();
            ClearStatusText();
        }
    } // END PauseGame

    /****************************************************************************
    Function: Update
    Parameter(s): N/A
    Output: N/A
    Comments: Game Update on time interval
    ****************************************************************************/
    void Update(double timeStep)
    {
        if (gameState == RUNNING) {
            // Move Character
            UpdatePlayerCharacter(timeStep);

            // Move AI
            UpdateAICharacters(timeStep);

            if (vulnerabilityTimer > 0 && GetTickCount() - vulnerabilityTimer > VULNERABILITY_TIME_LIMIT) {
                // Reset all of the AI Characters Vulnerability
                setAllGhostsVulnerable(false);
            }
        }
    } // END Update

    /****************************************************************************
    Function: CanMoveInSpecifiedDirection
    Parameter(s): int - Enum value (See @Constants.h) for valid Directions input.
                  int - X coordinate of Position to check
                  int - Y coordinate of Position to check
                  int - Movement speed (default = 1)
    Output: bool - True if move is allowed, False if not.
    Comments: Universal check for all Entities on the Map for 
              whether next position is valid move.
    ****************************************************************************/
    bool CanMoveInSpecifiedDirection(int direction, int xPos, int yPos, int movementSpeed = 1)
    {
        GameMap::RenderQueuePosition posToCheck(xPos, yPos);
        switch (direction)
        {
        case LEFT:
            posToCheck.xPos -= movementSpeed;
            break;
        case RIGHT:
            posToCheck.xPos += movementSpeed;
            break;
        case UP:
            posToCheck.yPos -= movementSpeed;
            break;
        case DOWN:
            posToCheck.yPos += movementSpeed;
            break;
        }
        return mGameMap.checkForEmptySpace(posToCheck);
    } // END CanMoveInSpecifiedDirection

    /****************************************************************************
    Function: TriggerGhostEaten
    Parameter(s): GhostEntity & - Reference to the Ghost that was just caught
                                  by player.
    Output: N/A
    Comments: Resets the GhostEntity back to Spawn Box position with Respawn Timer.
    ****************************************************************************/
    void TriggerGhostEaten(GhostEntity &entity) {
        mScoreBoard.addScoreTotal(GHOST_SCORE_AMOUNT*ghostMultiplier++);
        entity.Reset();
        entity.setRespawnTimer(GetTickCount());
        entity.Render();
    } // END TriggerGhostEaten

    /****************************************************************************
    Function: UpdateAICharacters
    Parameter(s): N/A
    Output: N/A
    Comments: Update all Active AI currently on the Map
    ****************************************************************************/
    void UpdateAICharacters(double timeStep) {
        for (int i =0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive()) {
                int respawnTimer = mGhosts[i].getRespawnTimer();
                if (GetTickCount() - lastAISpawnTime > GHOST_SPAWN_TIMER && (!respawnTimer || GetTickCount() - respawnTimer > GHOST_SPAWN_TIMER*4)) {
                    mGameMap.pushRenderQueuePosition(GameMap::RenderQueuePosition((int)mGhosts[i].getXPosition(), (int)mGhosts[i].getYPosition()));
                    mGhosts[i].initializeGhost();
                    mGhosts[i].setTarget(&mPlayer);
                    lastAISpawnTime = GetTickCount();
                }
                continue;
            }
            int xPos = (int)mGhosts[i].getXPosition();
            int yPos = (int)mGhosts[i].getYPosition();

            mGhosts[i].Update(mGameMap.getAvailableDirectionsForPosition(xPos, yPos), timeStep);
            
            if (mGhosts[i].IsInvalidated()) {
                mGameMap.pushRenderQueuePosition(GameMap::RenderQueuePosition(xPos, yPos));
            }
        } // END For(i<MAX_ENEMIES)

        CheckCollisions();
    } // END UpdateAICharacters


    /*********************************************************************************
    Function: CheckCollisions
    Parameter(s): N/A
    Output: N/A
    Comments: Checks for collisions of Player against the Map and Active Ghosts.
    *********************************************************************************/
    void CheckCollisions() {
        char charAtPos = ' ';
        int xPos = (int)mPlayer.getXPosition();
        int yPos = (int)mPlayer.getYPosition();
        charAtPos = mGameMap.getCharacterAtPosition(xPos, yPos);
        
        if (charAtPos == NORML_PELLET_CHARACTER)
        {
            mGameMap.decrementDotsRemaining();
            mGameMap.setCharacterAtPosition(' ', xPos, yPos);
            mScoreBoard.addPointsForPickup(charAtPos);
        }
        else if (charAtPos == POWER_PELLET_CHARACTER)
        {
            mGameMap.decrementDotsRemaining();
            mGameMap.setCharacterAtPosition(' ', xPos, yPos); 
            setAllGhostsVulnerable(true);
            ghostMultiplier = 1;
            vulnerabilityTimer = GetTickCount();
            mScoreBoard.addPointsForPickup(charAtPos);
        }

        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive()) {
                continue;
            }

            if (xPos == (int)mGhosts[i].getXPosition() && yPos == (int)mGhosts[i].getYPosition()) {
                if (mGhosts[i].isVulnerable()) {
                    TriggerGhostEaten(mGhosts[i]);
                }
                else {
                    RestartLevel();
                }
            }
        }
    }


    /*********************************************************************************
    Function: UpdatePlayerCharacter
    Parameter(s): N/A
    Output: N/A
    Comments: Input Update for Player object.  Command Queue logic used for ensuring
              the next possible move in the direction queued will be attempted.
    *********************************************************************************/
    void UpdatePlayerCharacter(double timeStep)
    {
        int cacheXPos = (int)mPlayer.getXPosition();
        int cacheYPos = (int)mPlayer.getYPosition();
        mPlayer.Update( mGameMap.getAvailableDirectionsForPosition(cacheXPos, cacheYPos), timeStep );
        int xPos = (int)mPlayer.getXPosition();
        int yPos = (int)mPlayer.getYPosition();

        // Player is invalidated if a Move has occurred
        if (mPlayer.IsInvalidated()) {
            mGameMap.pushRenderQueuePosition(GameMap::RenderQueuePosition(cacheXPos, cacheYPos));
            CheckCollisions();
        }
    }

    void UpdatePlayerDirection(int direction)
    {
        if (CanMoveInSpecifiedDirection(direction, (int)mPlayer.getXPosition(), (int)mPlayer.getYPosition()))
        {
            mPlayer.setMovementDirection(direction);
        }
    } // END UpdatePlayerCharacter

    /****************************************************************************
    Function: setAllGhostsVulnerable
    Parameter(s): bool - Boolean value used to set whether the Ghosts are 
                         vulnerable after Player collides with specific object.
    Output: N/A
    Comments: Updates all Active Ghost objects on Map vulnerability status.
    ****************************************************************************/
    void setAllGhostsVulnerable(bool status) {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive()) {
                continue;
            }
            mGhosts[i].setVulnerable(status);
        }
    } // END setAllGhostsVulnerable

    /****************************************************************************
    Function: GatherGamePlayInput
    Parameter(s): N/A
    Output: N/A
    Comments: State machine for the GameState, retrieves input and updates Player
              when detected.
    ****************************************************************************/
    void GatherGamePlayInput()
    {
        switch (gameState)
        {
        case ATTRACT:
        {
            if (GetAsyncKeyState(VK_ADD))
            {
                mCreditsBoard.incCredits();
                creditInserted = true;
            }

            if ((GetAsyncKeyState(VK_NUMPAD1)||GetAsyncKeyState(VK_1)) && mCreditsBoard.getCreditTotal() > 0)
            {
                mCreditsBoard.decCredits();
                ClearStatusText();
                RenderStatusText(READY_TEXT);
                restartDelayTimer = GetTickCount();
                gameState = READY;
            }
            break;
        }
        case RUNNING:
        {
            if (GetAsyncKeyState(VK_LEFT))
            {
                UpdatePlayerDirection(LEFT);
            }
            if (GetAsyncKeyState(VK_RIGHT))
            {
                UpdatePlayerDirection(RIGHT);
            }
            if (GetAsyncKeyState(VK_UP))
            {
                UpdatePlayerDirection(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                UpdatePlayerDirection(DOWN);
            }
            if (GetAsyncKeyState(VK_SPACE))
            {
                PauseGame();
            }
            break;
        }
        case READY:
        {
            if (GetTickCount() - restartDelayTimer > 3000) {
                gameState = RUNNING;
                lastAISpawnTime = GetTickCount();
                ClearStatusText();
                vulnerabilityTimer = 0;
            }
        }
        case PAUSED:
            if (GetAsyncKeyState(VK_SPACE))
            {
                PauseGame();
            }
            break;
        case WINNING:
        default:
            break;
        };
    } // END GatherGamePlayInput

    /****************************************************************************
    Function: Render
    Parameter(s): N/A
    Output: N/A
    Comments: Renders the entire frame and update for all Objects for Game.
    ****************************************************************************/
    void Render()
    {
        mScoreBoard.Render();
        mLivesBoard.Render();
        mCreditsBoard.Render();
        mGameMap.renderMap(); 
        mPlayer.Render(); 
        RenderAI();
        
        // TODO: Add the RenderEngine to run through all TrackedEntities for Rendering
        //          Score, Lives, Credits, etc.
    } // END Render

    /****************************************************************************
    Function: RenderAI
    Parameter(s): N/A
    Output: N/A
    Comments: Renders the Active Ghost AI on-screen at their present positions.
    ****************************************************************************/
    void RenderAI()
    {
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            mGhosts[i].Render();
        }
    } // END RenderAI

    /****************************************************************************
    Function: RenderStatusText
    Parameter(s): const char * - String to display in the line below the Ghost
                                 Spawn box.
    Output: N/A
    Comments: Renders string to screen below Ghost Spawn box.
    ****************************************************************************/
    void RenderStatusText(const char *stringToDisplay) {

        COORD Position;
        Position.X = STATUS_TEXT_OFFSET_MARGIN;
        Position.Y = 16;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        // Attempt to pad the string display to center the text 
        // under the Ghost Spawn box
        int length = strlen(stringToDisplay);
        length = (15 - length) / 2;
        for (int i = 0; i < length; ++i) {
            cout << ' ';
        }
        cout << stringToDisplay;
    } // END RenderStatusText

    /****************************************************************************
    Function: ClearStatusText
    Parameter(s): N/A
    Output: N/A
    Comments: Clears the Text from the screen for gameplay at the line below
              the Ghost Spawn box
    ****************************************************************************/
    void ClearStatusText() {
        COORD Position;
        Position.X = STATUS_TEXT_OFFSET_MARGIN;
        Position.Y = 16;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        cout << CLEAR_STATUS_TEXT;
    } // END ClearStatusText
};

#endif // _PAC_GAME_H_