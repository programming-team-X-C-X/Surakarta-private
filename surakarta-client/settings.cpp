#include "settings.h"

// 窗口设置
unsigned int WINDOW_SIZE = 700;

// 棋盘设置
unsigned int BOARD_SIZE = 6;

// 休眠设置
const unsigned int SLEEP_TIME = 500;

// 棋子设置
const unsigned int PIECE_SIZE = 30;

// 动画设置
double PIECE_SPEED = 150.0;

// 回合设置
unsigned int TIME_LIMIT = 60;
unsigned int MAX_NO_CAPTURE_ROUND = 40;

// 玩家设置
bool PLAYER_COLOR = 1; //1 black 2 white

// 行棋信息
bool RIGHT_COLOR  = 1;
int GAME_ROUND = 1;
bool IsAi = 1;
SurakartaPlayer MY_COLOR;
QString NAME;

// AI搜索深度设置
int AI_SEARCH_DEPTH = 3;

// 濒死搜索深度设置
int DEAD_SEARCH_DEPTH = 1;
