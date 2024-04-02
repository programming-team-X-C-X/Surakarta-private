#include "settings.h"

//窗口设置
const unsigned int WINDOW_SIZE = 700;

//棋盘设置
unsigned int BOARD_SIZE = 6;

int arcNum = (BOARD_SIZE - 2) / 2;//环数
int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
int gridSize = WINDOW_SIZE / rawNum;//棋盘单位长度

//休眠设置
const unsigned int SLEEP_TIME = 500;

//棋子设置
const unsigned int PIECE_SIZE = 30;
double PIECE_SPEED = 100.0;

//回合限时
unsigned int TIME_LIMIT = 200;
