/****************************************************************************
File: Constants.h
Author: fookenCode
****************************************************************************/
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

enum MOVEMENT_DIRECTIONS { LEFT = 0, UP, RIGHT, DOWN, MAX_DIRECTION };
enum AI_COLORS { GHOST_BLUE =(0x0008 | 0x0004 | 0x0001 | 0x0002 | 0x0010 | 0x0080), GREEN = 10, CYAN, RED, PINK, INVALID_COLOR };
enum LEVEL_COLORS {LVL_ONE=26, LVL_TWO=78, INVALID_LEVEL};
enum WALL_GROUPS { INNER = 0, OUTER, BOTH, INVALID_GROUP };
enum GAME_STATE { ATTRACT = 0, PAUSED, RUNNING, WINNING };
const static int MAX_ENEMIES = 4;
const static int DEFAULT_MAP_WIDTH = 37;
const static int DEFAULT_MAP_HEIGHT = 30;
const static int DEFAULT_MAP_EDGE = DEFAULT_MAP_WIDTH - 2;
const static int DEFAULT_PLAYER_X_POSITION = 17;
const static int DEFAULT_PLAYER_Y_POSITION = 22;
const static int DEFAULT_AI_Y_POSITION = 13;
const static int DEFAULT_AI_X_POSITION = 14;
const static int AI_BOX_ACTIVE_Y_POSITION = 11;
const static int AI_BOX_ACTIVE_X_POSITION = 17;
const static int SCREEN_OFFSET_MARGIN = 16;
#endif // _CONSTANTS_H_