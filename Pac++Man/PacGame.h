/****************************************************************************
File: PacGame.h
Author: fookenCode
TODO: Make class Singleton
****************************************************************************/
#ifndef _PAC_GAME_H_
#define _PAC_GAME_H_

#include <Windows.h>
#include "Constants.h"
#include "GameMap.h"
#include "GhostEntity.h"
#include "PlayerEntity.h"
#include "Entity.h"
class PacGame {
public:
	unsigned int lives, score;
	int commandQueue, gameState, playerCredits;
	bool switchedSides, creditInserted;
	
	PlayerEntity mPlayer;
	GhostEntity mGhosts[MAX_ENEMIES];
	GameMap mGameMap;

	PacGame()
	{
		// Initialize all game data
		gameState = ATTRACT;
		lives = 3;
		score = 0;
		playerCredits = 99;
		commandQueue = -1;
		creditInserted = false;
		switchedSides = false;

		Reset();
	} // End Constructor

	void SidesReset()    { switchedSides = false; }
	bool IsGameRunning() { return !((gameState == WINNING || gameState == RUNNING) && IsGameOver()); }
	bool IsGameOver()    { return (mGameMap.getTotalDotsRemaining() <= 0); }
	int  getGameState()  { return gameState; }

	void Reset()
	{
		// CommandQueue used to create free-flowing and fast response to input by 
		// queueing until next opportunity to move in desired direction
		commandQueue = -1;
		// Controls the state of whether the player has accessed the tunnel to either side of Map
		switchedSides = false;

		// Initialize Player object and status
		mPlayer.reset();
		mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), mPlayer.getXPosition(), mPlayer.getYPosition());


		// Initialize all AI objects and status
		for (int i = 0; i < MAX_ENEMIES; ++i) {
			mGhosts[i].reset();
			mGhosts[i].setGhostColor(GREEN + i);
			if (i == 0) {
				mGhosts[i].setActive(true);
				mGhosts[i].setXPos(AI_BOX_ACTIVE_X_POSITION);
				mGhosts[i].setYPos(AI_BOX_ACTIVE_Y_POSITION);
			}
			else {
				mGhosts[i].setXPos(DEFAULT_AI_X_POSITION + i);
				mGhosts[i].setYPos(DEFAULT_AI_Y_POSITION);
			}
			mGameMap.setCharacterAtPosition(mGhosts[i].getGhostIcon(), mGhosts[i].getXPosition(), mGhosts[i].getYPosition());
		}
		
		// Render all components
		mGameMap.renderMap();
		RenderPlayer(true);
		RenderAI();
		RenderStartText();
	}

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
		}
	}

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
			if (nextPos <1 && yPos == 11)
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
			if (nextPos > DEFAULT_MAP_EDGE && yPos == 11)
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
			spaceToMove = mGameMap.getCharacterAtPosition(xPos, yPos - movementSpeed);
			if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
			{
				returnCase = true;
			}
			break;
		case DOWN:
			// Protect from Entity entering the Home Base of the Ghosts
			if (((xPos >= 15 && xPos <= 18) && yPos == 11))
			{
				returnCase = false;
			}
			else
			{
				spaceToMove = mGameMap.getCharacterAtPosition(xPos, yPos + movementSpeed);
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
	Function: UpdateAICharacters
	Parameter(s): N/A
	Output: N/A
	Comments: Update all Active AI currently on the Map
	****************************************************************************/
	void UpdateAICharacters() {
		for (int i =0; i < MAX_ENEMIES; ++i) {
			if (!mGhosts[i].isActive()) {
				continue;
			}

			mGhosts[i].setMovementDirection(LEFT);
			mGhosts[i].setMovementSpeed(1);
			if (CanMoveInSpecifiedDirection(mGhosts[i].getMovementDirection(), mGhosts[i].getXPosition(), mGhosts[i].getYPosition()))
			{
				MoveAI(mGhosts[i]);
			}
		}
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
		mGameMap.setCharacterAtPosition(' ', xPos, yPos);
		switch (movementDirection)
		{
		case LEFT:
			nextPos = xPos - movementSpeed;
			if (nextPos == 0)
			{
				mGameMap.setCharacterAtPosition(entity.getGhostIcon(), DEFAULT_MAP_EDGE, yPos);
				entity.setXPos(DEFAULT_MAP_EDGE);
			}
			else
			{
				charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
				if (charAtNext == mPlayer.getIconForDirection())
				{
					mPlayer.reset();
					lives--;
				}
				mGameMap.setCharacterAtPosition(entity.getGhostIcon(), nextPos, yPos);
				entity.setXPos(nextPos);
			}

			break;
		case RIGHT:
			nextPos = xPos + movementSpeed;
			if (nextPos > DEFAULT_MAP_EDGE)
			{
				mGameMap.setCharacterAtPosition(entity.getGhostIcon(), 0, yPos);
				entity.setXPos(0);
			}
			else
			{
				charAtNext = mGameMap.getCharacterAtPosition(nextPos, yPos);
				if (charAtNext == mPlayer.getIconForDirection())
				{
					mPlayer.reset();
					lives--;
				}
				mGameMap.setCharacterAtPosition(entity.getGhostIcon(), nextPos, yPos);
				mPlayer.setXPos(nextPos);
			}
			break;
		case UP:
			nextPos = yPos - movementSpeed;
			charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
			if (charAtNext == mPlayer.getIconForDirection())
			{
				mPlayer.reset();
				lives--;
			}
			mGameMap.setCharacterAtPosition(entity.getGhostIcon(), xPos, nextPos);
			mPlayer.setYPos(nextPos);
			break;
		case DOWN:
			nextPos = yPos + movementSpeed;
			charAtNext = mGameMap.getCharacterAtPosition(xPos, nextPos);
			if (charAtNext == 'ú')
			{
				mPlayer.reset();
				lives--;
			}
			mGameMap.setCharacterAtPosition(entity.getGhostIcon(), xPos, nextPos);
			mPlayer.setYPos(nextPos);
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
		switch (movementDirection)
		{
		case LEFT:
			nextPos = xPos - movementSpeed;
			if (nextPos == 0)
			{
				mGameMap.setCharacterAtPosition(mPlayer.getIconForDirection(), DEFAULT_MAP_EDGE, yPos);
				mPlayer.setXPos(DEFAULT_MAP_EDGE);
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
			if (nextPos > DEFAULT_MAP_EDGE)
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

		if (charAtNext == 'ú')
		{
			mGameMap.decrementDotsRemaining();
			score += 10;
		}
		else if (charAtNext == 'ù')
		{
			mGameMap.decrementDotsRemaining();
			setAllGhostsVulnerable(true);
			score += 50;
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

			mGhosts[i].setVulnerable(true);
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
			if (GetAsyncKeyState(VK_ADD) && creditInserted == false)
			{
				playerCredits++;
				creditInserted = true;
			}
			else if (!GetAsyncKeyState(VK_ADD))
			{
				creditInserted = false;
			}

			if (GetAsyncKeyState(VK_NUMPAD1) && playerCredits > 0)
			{
				playerCredits--;
				ClearStartText();
				gameState = RUNNING;
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
			if (mGameMap.getTotalDotsRemaining() == 0)
			{
				gameState = WINNING;
			}
			break;
		}
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
		COORD Position;
		int xPos = mPlayer.getXPosition(), yPos = mPlayer.getYPosition(), direction = mPlayer.getMovementDirection();
		switch (direction)
		{
		case LEFT:
			if (switchedSides == true)
			{
				Position.X = 17;
				Position.Y = yPos;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				cout << mGameMap.getCharacterAtPosition(0, yPos);
				Position.X = xPos + SCREEN_OFFSET_MARGIN;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				RenderPlayer();
				SidesReset();
			}
			else
			{

				Position.X = xPos + SCREEN_OFFSET_MARGIN;
				Position.Y = yPos;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				RenderPlayer();
				if (mGameMap.isWallCharacter(xPos+1, yPos, BOTH) == false)
				{
					cout << mGameMap.getCharacterAtPosition(xPos+1, yPos);
				}
			}
			break;
		case RIGHT:
			if (switchedSides == true)
			{
				Position.X = (DEFAULT_MAP_EDGE) +SCREEN_OFFSET_MARGIN;
				Position.Y = yPos;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				cout << mGameMap.getCharacterAtPosition(DEFAULT_MAP_EDGE, yPos);
				Position.X = xPos + SCREEN_OFFSET_MARGIN;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				RenderPlayer();
				SidesReset();
			}
			else
			{
				if (mGameMap.isWallCharacter(xPos - 1, yPos, BOTH) == true)
				{
					RenderPlayer(true);
				}
				else
				{
					Position.X = xPos + 15;
					Position.Y = yPos;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
					cout << mGameMap.getCharacterAtPosition(xPos - 1, yPos);
					RenderPlayer();
				}
			}
			break;
		case UP:
			Position.X = xPos + SCREEN_OFFSET_MARGIN;
			Position.Y = yPos;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
			RenderPlayer();

			if (mGameMap.isWallCharacter(xPos, yPos+1, BOTH) == false)
			{
				Position.Y = yPos + 1;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				cout << mGameMap.getCharacterAtPosition(xPos, yPos+1);
			}
			break;
		case DOWN:
			Position.X = xPos + SCREEN_OFFSET_MARGIN;
			Position.Y = yPos;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
			RenderPlayer();
			if (mGameMap.isWallCharacter(xPos, yPos - 1, BOTH) == false)
			{
				Position.Y = yPos - 1;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
				cout << mGameMap.getCharacterAtPosition(xPos, yPos - 1);
			}
			break;
		}
		RenderAI();
		RenderScore();
		RenderLives();
		RenderCredits();
	} // END Render

	/****************************************************************************
	Function: RenderPlayer
	Parameter(s): bool - Boolean value on whether the Position needs to be set
	                     prior to rendering the Player character icon.
	Output: N/A
	Comments: Renders the player at their present position, if True.  Renders
	          at last Position set in Console, if False.
	****************************************************************************/
	void RenderPlayer(bool moveCursorBeforeRender = false)
	{
		if (moveCursorBeforeRender)
		{
			COORD Position;
			Position.X = mPlayer.getXPosition() + SCREEN_OFFSET_MARGIN;
			Position.Y = mPlayer.getYPosition();
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		cout << mGameMap.getCharacterAtPosition(mPlayer.getXPosition(), mPlayer.getYPosition());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	} // END RenderPlayer

	/****************************************************************************
	Function: RenderAI
	Parameter(s): N/A
	Output: N/A
	Comments: Renders the Active Ghost AI on-screen at their present positions.
	****************************************************************************/
	void RenderAI()
	{
		COORD Position;
		for (int i = 0; i < MAX_ENEMIES; ++i) {
			if (!mGhosts[i].isActive()) {
				continue;
			}
			Position.X = mGhosts[i].getXPosition() + SCREEN_OFFSET_MARGIN;
			Position.Y = mGhosts[i].getYPosition();
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mGhosts[i].getGhostColor());
			cout << mGameMap.getCharacterAtPosition(mGhosts[i].getXPosition(), mGhosts[i].getYPosition());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
	} // END RenderAI

	/****************************************************************************
	Function: RenderScore
	Parameter(s): N/A
	Output: N/A
	Comments: Renders the Score information on the right-side of screen.
	****************************************************************************/
	void RenderScore()
	{
		COORD Position;
		Position.X = mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN + 8;
		Position.Y = 2;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << "Score";
		Position.X = mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN + 10;
		Position.Y = 3;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << score;
	} // END RenderScore

	/****************************************************************************
	Function: RenderLives
	Parameter(s): N/A
	Output: N/A
	Comments: Renders the number of Player lives that are currently Active.
	****************************************************************************/
	void RenderLives()
	{
		COORD Position;
		Position.X = mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN + 8;
		Position.Y = 6;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << "Lives";
		Position.X = mGameMap.getMapWidth() + SCREEN_OFFSET_MARGIN + 9;
		Position.Y = 7;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		for (int i = 0; i < lives; ++i)
		{
			cout << mPlayer.getIconForDirection(RIGHT) << ' ';
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	} // END RenderLives

	/****************************************************************************
	Function: RenderCredits
	Parameter(s): N/A
	Output: N/A
	Comments: Renders the number of Credits the Player currently has inserted
	          at the bottom of the screen.
	****************************************************************************/
	void RenderCredits()
	{
		COORD Position;
		Position.X = 5;
		Position.Y = mGameMap.getMapHeight();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << "Credits " << playerCredits;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	} // END RenderCredits

	/****************************************************************************
	Function: RenderStartText
	Parameter(s): N/A
	Output: N/A
	Comments: Renders help text to alert user to press '1' key to Start.
	****************************************************************************/
	void RenderStartText() {

		COORD Position;
		Position.X = 30;
		Position.Y = 16;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << "Press '1'";
	} // END RenderStartText

	/****************************************************************************
	Function: ClearStartText
	Parameter(s): N/A
	Output: N/A
	Comments: Clears the Start Help Text from the screen for gameplay.
	****************************************************************************/
	void ClearStartText() {
		COORD Position;
		Position.X = 30;
		Position.Y = 16;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
		cout << "         ";
	} // END ClearStartText
};

#endif // _PAC_GAME_H_