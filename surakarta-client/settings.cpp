#include "settings.h"

//窗口设置
unsigned int WINDOW_SIZE = 700;
// unsigned int WINDOW_SIZE = gridSize * rawNum;

int arcNum = (BOARD_SIZE - 2) / 2;//环数
int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
int gridSize = WINDOW_SIZE / rawNum;//棋盘单位长度
// int gridSize = 70;

//棋盘设置
unsigned int BOARD_SIZE = 6;

//休眠设置
const unsigned int SLEEP_TIME = 500;

//棋子设置
const unsigned int PIECE_SIZE = 30;
double PIECE_SPEED = 100.0;

//回合设置
unsigned int TIME_LIMIT = 60;
unsigned int MAX_NO_CAPTURE_ROUND = 40;

//玩家设置
bool PLAYER_COLOR = 1; //1 black 2 white

// 当前行棋颜色
bool RIGHT_COLOR  = 1;

int gameround     = 1;

bool IsAi = 0;

SurakartaPlayer mycolor;

QString name;
