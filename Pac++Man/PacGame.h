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
        mLivesBoard.setLivesLeft(MAX_VISIBLE_LIVES);
        
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
        // CommandQueue used to create free-flowing and fast response to input by 
        // queueing until next opportunity to move in desired direction
        commandQueue = -1;
        // Controls the state of whether the player has accessed the tunnel to either side of Map
        switchedSides = false;
        ghostMultiplier = 1;
        vulnerabilityTimer = 0;

        // Initialize Player object and status
        mPlayer.Reset();
        mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), mPlayer.getXPosition(), mPlayer.getYPosition());


        // Initialize all AI objects and status
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            mGhosts[i].Reset();
            mGhosts[i].setGhostColor(GREEN + i);
            if (i == 0) {
                mGhosts[i].setActive(true);
                mGhosts[i].setXPos(AI_BOX_ACTIVE_X_POSITION);
                mGhosts[i].setYPos(AI_BOX_ACTIVE_Y_POSITION);
                mGhosts[i].setMovementDirection(LEFT);
                mGhosts[i].setMovementSpeed(1);
            }
            else {
                mGhosts[i].setXPos(DEFAULT_AI_X_POSITION + i*2);
                mGhosts[i].setYPos(DEFAULT_AI_Y_POSITION);
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
        RenderAI(true);
        mPlayer.Render();
    } // END Reset

    /****************************************************************************
    Function: RestartLevel
    Parameter(s): N/A
    Output: N/A
    Comments: Called when Player is caught by Ghosts or Level Completes
    ****************************************************************************/
    void RestartLevel() {
        mGameMap.setCharacterAtPosition(' ', mPlayer.getXPosition(), mPlayer.getYPosition());
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
    void Update()
    {
        if (gameState == RUNNING) {
            // Move Character
            UpdatePlayerCharacter();

            // Move AI
            UpdateAICharacters();

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
        bool returnCase = false;
        int nextPos = 0;
        char spaceToMove;
        switch (direction)
        {
        case LEFT:
            nextPos = xPos - movementSpeed;
            if (nextPos <0 && yPos == 11)
            {
                returnCase = true;
                switchedSides = true;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(nextPos, yPos);
                if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                {
                    returnCase = true;
                }
            }
            break;
        case RIGHT:
            nextPos = xPos + movementSpeed;
            if (nextPos > mGameMap.getMapEdge() && yPos == 11)
            {
                returnCase = true;
                switchedSides = true;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(nextPos, yPos);
                if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                {
                    returnCase = true;
                }
            }
            break;
        case UP:
            nextPos = yPos - movementSpeed;
            spaceToMove = mGameMap.getCharacterAtPosition(xPos, nextPos);
            if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
            {
                returnCase = true;
            }
            break;
        case DOWN:
            nextPos = yPos + movementSpeed;
            // Protect from Entity entering the Home Base of the Ghosts
            if (((xPos >= 15 && xPos <= 18) && yPos == 11))
            {
                returnCase = false;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(xPos, nextPos);
                if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                {
                    returnCase = true;
                }
            }
            break;
        }
        return returnCase;
    } // END CanMoveInSpecifiedDirection

    /****************************************************************************
    Function: GhostCanMoveInSpecifiedDirection
    Parameter(s): int - Enum value (See @Constants.h) for valid Directions input.
    int - X coordinate of Position to check
    int - Y coordinate of Position to check
    int - Movement speed (default = 1)
    Output: bool - True if move is allowed, False if not.
    Comments: Universal check for all Entities on the Map for
    whether next position is valid move.
    ****************************************************************************/
    bool GhostCanMoveInSpecifiedDirection(GhostEntity &ghost, int testDirection = MAX_DIRECTION) {
        bool returnCase = false;
        int nextPos = 0;
        int xPos = ghost.getXPosition();
        int yPos = ghost.getYPosition();
        int movementSpeed = ghost.getMovementSpeed();
        char playerIcon = mPlayer.getIconForDirection();
        char spaceToMove;
        switch ((testDirection==MAX_DIRECTION)?ghost.getMovementDirection():testDirection)
        {
        case LEFT:
            nextPos = xPos - movementSpeed;
            if (nextPos <1 && yPos == 11)
            {
                returnCase = true;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(nextPos, yPos);
                if (!IsGhostAtPosition(nextPos, yPos) && (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ' || spaceToMove == playerIcon))
                {
                    returnCase = true;
                }
            }
            break;
        case RIGHT:
            nextPos = xPos + movementSpeed;
            if (nextPos > mGameMap.getMapEdge() && yPos == 11)
            {
                returnCase = true;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(nextPos, yPos);
                if (!IsGhostAtPosition(nextPos, yPos) && (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ' || spaceToMove == playerIcon))
                {
                    returnCase = true;
                }
            }
            break;
        case UP:
            nextPos = yPos - movementSpeed;
            spaceToMove = mGameMap.getCharacterAtPosition(xPos, nextPos);
            if (!IsGhostAtPosition(xPos, nextPos) && (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ' || spaceToMove == playerIcon))
            {
                returnCase = true;
            }
            break;
        case DOWN:
            nextPos = yPos + movementSpeed;
            // Protect from Entity entering the Home Base of the Ghosts
            if (((xPos >= 15 && xPos <= 18) && yPos == 11))
            {
                returnCase = false;
            }
            else
            {
                spaceToMove = mGameMap.getCharacterAtPosition(xPos, nextPos);
                if (!IsGhostAtPosition(xPos, nextPos) && (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ' || spaceToMove == playerIcon))
                {
                    returnCase = true;
                }
            }
            break;
        }
        return returnCase;
    } // END GhostCanMoveInSpecifiedDirection

    /****************************************************************************
    Function: TriggerGhostEaten
    Parameter(s): GhostEntity & - Reference to the Ghost that was just caught
                                  by player.
    Output: N/A
    Comments: Resets the GhostEntity back to Spawn Box position with Respawn Timer.
    ****************************************************************************/
    void TriggerGhostEaten(GhostEntity &entity) {
        entity.setActive(false);
        entity.setVulnerable(false);
        mScoreBoard.addScoreTotal(GHOST_SCORE_AMOUNT*ghostMultiplier);
        entity.setXPos(DEFAULT_AI_X_POSITION + 2*(entity.getGhostColor() % GREEN));
        entity.setYPos(DEFAULT_AI_Y_POSITION);
        entity.setMovementDirection(MAX_DIRECTION);
        entity.setMovementSpeed(0);
        entity.setRespawnTimer(GetTickCount());
        RenderSingleAI(entity);
    } // END TriggerGhostEaten

    /****************************************************************************
    Function: UpdateAICharacters
    Parameter(s): N/A
    Output: N/A
    Comments: Update all Active AI currently on the Map
    ****************************************************************************/
    void UpdateAICharacters() {
        bool canMoveCurr, canMoveNext = false;
        int nextMoveDir = MAX_DIRECTION;
        for (int i =0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive()) {
                int respawnTimer = mGhosts[i].getRespawnTimer();
                if (GetTickCount() - lastAISpawnTime > GHOST_SPAWN_TIMER && (!respawnTimer || GetTickCount() - respawnTimer > GHOST_SPAWN_TIMER*4)) {
                    mGhosts[i].setActive(true);
                    ClearInitialGhostPosition(mGhosts[i]);
                    mGhosts[i].setXPos(AI_BOX_ACTIVE_X_POSITION);
                    mGhosts[i].setYPos(AI_BOX_ACTIVE_Y_POSITION);
                    mGhosts[i].setMovementDirection(LEFT);
                    mGhosts[i].setMovementSpeed(1);
                    lastAISpawnTime = GetTickCount();
                }
                continue;
            }

            canMoveNext = false;
            nextMoveDir = MAX_DIRECTION;
            canMoveCurr = GhostCanMoveInSpecifiedDirection(mGhosts[i]);
            switch (mGhosts[i].getMovementDirection()) {
            case LEFT:
            case RIGHT:
                nextMoveDir = (mGhosts[i].getYPosition() < mPlayer.getYPosition()) ? DOWN : UP;
                canMoveNext = GhostCanMoveInSpecifiedDirection(mGhosts[i], nextMoveDir);
                break;
            case UP:
            case DOWN:
                nextMoveDir = (mGhosts[i].getXPosition() < mPlayer.getXPosition()) ? RIGHT : LEFT;
                canMoveNext = GhostCanMoveInSpecifiedDirection(mGhosts[i], nextMoveDir);
                break;
            default:
                break;
            };
            if (canMoveNext || !canMoveCurr) {
                mGhosts[i].setMovementDirection(nextMoveDir);
            }
            if (canMoveNext || canMoveCurr) {
                MoveAI(mGhosts[i]);
                // Exit this function early to ensure that no further AI movements
                // are performed if the GameState has changed due to collision with player
                if (gameState == READY) {
                    return;
                }
            }
        } // END For(i<MAX_ENEMIES)
    } // END UpdateAICharacters

    /****************************************************************************
    Function: MoveAI
    Parameter(s): GhostEntity & - Reference to the AI object to move
    Output: N/A
    Comments: Move the AI along their currently set Direction, 
              and check for collision.
    ****************************************************************************/
    void MoveAI(GhostEntity &entity)
    {
        int movementDirection = entity.getMovementDirection();
        int movementSpeed = entity.getMovementSpeed();
        int xPos = entity.getXPosition();
        int yPos = entity.getYPosition();
        int nextPos = 0;
        char charAtNext;
        COORD Position;
        Position.X = xPos + SCREEN_OFFSET_MARGIN;
        Position.Y = yPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        cout << mGameMap.getCharacterAtPosition(xPos, yPos);

        switch (movementDirection)
        {
        case LEFT:
            nextPos = xPos - movementSpeed;
            if (nextPos == 0)
            {
                entity.setXPos(mGameMap.getMapEdge());
            }
            else
            {
                charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
                entity.setXPos(nextPos);
                if (charAtNext == mPlayer.getIconForDirection())
                {
                    if (entity.isVulnerable()) {
                        TriggerGhostEaten(entity);
                    }
                    else {
                        RestartLevel();
                    }
                }
            }

            break;
        case RIGHT:
            nextPos = xPos + movementSpeed;
            if (nextPos > mGameMap.getMapEdge())
            {
                entity.setXPos(0);
            }
            else
            {
                charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
                entity.setXPos(nextPos);
                if (charAtNext == mPlayer.getIconForDirection())
                {
                    if (entity.isVulnerable()) {
                        TriggerGhostEaten(entity);
                    }
                    else {
                        RestartLevel();
                    }
                }
            }
            break;
        case UP:
            nextPos = yPos - movementSpeed;
            charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
            entity.setYPos(nextPos);
            if (charAtNext == mPlayer.getIconForDirection())
            {
                if (entity.isVulnerable()) {
                    TriggerGhostEaten(entity);
                }
                else {
                    RestartLevel();
                }
            }
            break;
        case DOWN:
            nextPos = yPos + movementSpeed;
            charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
            entity.setYPos(nextPos);
            if (charAtNext == mPlayer.getIconForDirection())
            {
                if (entity.isVulnerable()) {
                    TriggerGhostEaten(entity);
                }
                else {
                    RestartLevel();
                }
            }
            break;
        };
    } // End MoveAI()

    /*********************************************************************************
    Function: UpdatePlayerCharacter
    Parameter(s): N/A
    Output: N/A
    Comments: Input Update for Player object.  Command Queue logic used for ensuring
              the next possible move in the direction queued will be attempted.
    *********************************************************************************/
    void UpdatePlayerCharacter()
    {
        if (mPlayer.getMovementSpeed() <= 0)
        {
            return;
        }

        if (commandQueue != -1)
        {
            int plyrDirection = mPlayer.getMovementDirection();
            if ((commandQueue == UP && plyrDirection == DOWN) || (commandQueue == DOWN && plyrDirection == UP)
                || (commandQueue == RIGHT && plyrDirection == LEFT) || (commandQueue == LEFT && plyrDirection == RIGHT))
            {
                commandQueue = -1;
            }
        }

        if (commandQueue != -1 && CanMoveInSpecifiedDirection(commandQueue, mPlayer.getXPosition(), mPlayer.getYPosition()))
        {
            mPlayer.setMovementDirection(commandQueue);
            mPlayer.setMovementSpeed(1);
            MovePlayerCharacter();
            commandQueue = -1;
        }
        else if (CanMoveInSpecifiedDirection(mPlayer.getMovementDirection(), mPlayer.getXPosition(), mPlayer.getYPosition()))
        {
            MovePlayerCharacter();
        }
    }

    void UpdatePlayerDirection(int direction)
    {
        if (CanMoveInSpecifiedDirection(direction, mPlayer.getXPosition(), mPlayer.getYPosition()))
        {
            mPlayer.setMovementDirection(direction);
            mPlayer.setMovementSpeed(1);
        }
        else
        {
            commandQueue = direction;
        }
    } // END UpdatePlayerCharacter

    /****************************************************************************
    Function: MovePlayerCharacter
    Parameter(s): N/A
    Output: N/A
    Comments: Performs the move based on Direction & MovementSpeed along path.
              Also, checks for collision and updates score accordingly.
    ****************************************************************************/
    void MovePlayerCharacter()
    {
        int movementDirection = mPlayer.getMovementDirection();
        int movementSpeed = mPlayer.getMovementSpeed();
        int xPos = mPlayer.getXPosition();
        int yPos = mPlayer.getYPosition();
        int nextPos = 0;
        char charAtNext = ' ';
        mGameMap.setCharacterAtPosition(' ', xPos, yPos);
        mGameMap.pushRenderQueuePosition(GameMap::RenderQueuePosition(xPos, yPos));
        mPlayer.setInvalidated(true);
        switch (movementDirection)
        {
        case LEFT:
            nextPos = xPos - movementSpeed;
            if (nextPos < 0)
            {
                mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), mGameMap.getMapEdge(), yPos);
                mPlayer.setXPos(mGameMap.getMapEdge());
            }
            else
            {
                charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
                mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), nextPos, yPos);
                mPlayer.setXPos(nextPos);
            }

            break;
        case RIGHT:
            nextPos = xPos + movementSpeed;
            if (nextPos > mGameMap.getMapEdge())
            {
                mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), 0, yPos);
                mPlayer.setXPos(0);
            }
            else
            {
                charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
                mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), nextPos, yPos);
                mPlayer.setXPos(nextPos);
            }
            break;
        case UP:
            nextPos = yPos - movementSpeed;
            charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
            mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), xPos, nextPos);
            mPlayer.setYPos(nextPos);
            break;
        case DOWN:
            nextPos = yPos + movementSpeed;
            charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
            mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), xPos, nextPos);
            mPlayer.setYPos(nextPos);
            break;
        }

        if (charAtNext == NORML_PELLET_CHARACTER)
        {
            mGameMap.decrementDotsRemaining();
            mScoreBoard.addPointsForPickup(charAtNext);
        }
        else if (charAtNext == POWER_PELLET_CHARACTER)
        {
            mGameMap.decrementDotsRemaining();
            setAllGhostsVulnerable(true);
            vulnerabilityTimer = GetTickCount();
            mScoreBoard.addPointsForPickup(charAtNext);
        }
    } // End MovePlayerCharacter()

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
    Function: ClearInitialGhostPosition
    Parameter(s): GhostEntity & - Reference to entity that is becoming Active,
                                  this function clears the Box Spawn Position
    Output: N/A
    Comments: Clears the Box Spawn position of the GhostEntity.
    ****************************************************************************/
    void ClearInitialGhostPosition(GhostEntity &entity) {
        COORD Position;
        Position.X = entity.getXPosition() + SCREEN_OFFSET_MARGIN;
        Position.Y = entity.getYPosition();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        cout << ' ';
    } // END ClearInitialGhostPosition

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
            if (mGameMap.getTotalDotsRemaining() == 0)
            {
                gameState = WINNING;
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
    void RenderAI(bool forceRenderAll = false)
    {
        COORD Position;
        HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (!mGhosts[i].isActive() && !forceRenderAll) {
                continue;
            }
            Position.X = mGhosts[i].getXPosition() + SCREEN_OFFSET_MARGIN;
            Position.Y = mGhosts[i].getYPosition();
            SetConsoleCursorPosition(outHandle, Position);
            SetConsoleTextAttribute(outHandle, mGhosts[i].getGhostColor());
            cout << mGhosts[i].getGhostIcon();
            SetConsoleTextAttribute(outHandle, 7);
        }
    } // END RenderAI

    /****************************************************************************
    Function: RenderSingleAI
    Parameter(s): GhostEntity & - Reference to the Ghost to manually render.
    Output: N/A
    Comments: Renders the provided Ghost AI on-screen.
    ****************************************************************************/
    void RenderSingleAI(GhostEntity &entity)
    {
        COORD Position;
        Position.X = entity.getXPosition() + SCREEN_OFFSET_MARGIN;
        Position.Y = entity.getYPosition();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), entity.getGhostColor());
        cout << entity.getGhostIcon();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    } // END RenderSingleAI


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