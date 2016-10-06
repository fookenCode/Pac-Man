/****************************************************************************
File: Constants.h
Author: fookenCode
****************************************************************************/
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

enum MOVEMENT_DIRECTIONS { LEFT = 0, UP, RIGHT, DOWN, MAX_DIRECTION };
enum DIRECTION_BITS {LEFT_BIT = 0x1, UP_BIT = 0x2, RIGHT_BIT = 0x4, DOWN_BIT = 0x8};
enum AI_COLORS { GHOST_BLUE =(0x0008 | 0x0004 | 0x0001 | 0x0002 | 0x0010 | 0x0080), GREEN = 10, CYAN, RED, PINK, INVALID_COLOR };
enum LEVEL_COLORS {LVL_ONE=26, LVL_TWO=78, INVALID_LEVEL};
enum WALL_GROUPS { INNER = 0, OUTER, BOTH, INVALID_GROUP };
enum GAME_STATE { ATTRACT = 0, PAUSED, READY, RUNNING, WINNING };

const static double MOVING_ENTITY_DEFAULT_SPEED     = 2.25;
const static int SCORE_BOARD_HEIGHT_POSITION        = 2;
const static int MAX_VISIBLE_LIVES                  = 3;
const static int MAX_CREDITS_ALLOWED                = 99;
const static int MAX_ENEMIES                        = 4;
const static int DEFAULT_MAP_WIDTH                  = 37;
const static int DEFAULT_MAP_HEIGHT                 = 30;
const static int DEFAULT_PLAYER_X_POSITION          = 17;
const static int DEFAULT_PLAYER_Y_POSITION          = 22;
const static int DEFAULT_AI_Y_POSITION              = 13;
const static int DEFAULT_AI_X_POSITION              = 14;
const static int AI_BOX_ACTIVE_Y_POSITION           = 11;
const static int AI_BOX_ACTIVE_X_POSITION           = 17;
const static int MILLISECONDS_FPS_THRESHOLD         = 16;
const static int SCREEN_OFFSET_MARGIN               = 5;
const static int LIVES_BOARD_HEIGHT_POSITION        = 6;
const static int STATUS_TEXT_OFFSET_MARGIN          = SCREEN_OFFSET_MARGIN + 11;
const static int POWER_PELLET_SCORE_AMOUNT          = 50;
const static int NORML_PELLET_SCORE_AMOUNT          = 10;
const static int GHOST_SCORE_AMOUNT                 = 600;
const static int GHOST_SPAWN_TIMER                  = 5000;
const static int VULNERABILITY_TIME_LIMIT           = 4250;
const static int VK_1                               = 0x31;
const static char POWER_PELLET_CHARACTER            = (char)0xF9;
const static char NORML_PELLET_CHARACTER            = (char)0xFA;
const static char MAP_FILLER_CHARACTER              = (char)0x61;
const static char LIVES_BOARD_CHARACTER             = (char)0x3C;
const static char SPAWN_BOX_BARRIER_CHARACTER       = (char)0x7E;
const static char *LEVEL_FILENAME_TEMPLATE          = "Assets\\Levels\\PacMan_Level_%d.txt";
const static char *SCORE_NAME_TEXT                  = "Score";
const static char *LIVES_NAME_TEXT                  = "Lives";
const static char *CREDITS_NAME_TEXT                = "Credits ";
const static char *CLEAR_STATUS_TEXT                = "              ";
const static char *PRESS_START_TEXT                 = "Press '1'";
const static char *GAMEOVER_TEXT                    = "Game Over";
const static char *READY_TEXT                       = "Ready!";
const static char *PAUSED_TEXT                      = "Paused!";
const static char *TITLE_WINDOW_TEXT                = "Pac++Man by FookenCode!";
#endif // _CONSTANTS_H_