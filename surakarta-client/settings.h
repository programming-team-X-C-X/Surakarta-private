#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include "info_piece.h"

// 源文件中有所注释
extern unsigned int BOARD_SIZE;
extern unsigned int WINDOW_SIZE;
extern const unsigned int SLEEP_TIME;
extern const unsigned int PIECE_SIZE;
extern double PIECE_SPEED;
extern unsigned int TIME_LIMIT;
extern unsigned int MAX_NO_CAPTURE_ROUND;
extern bool PLAYER_COLOR;
extern int GAME_ROUND;
extern QString NAME;
extern bool RIGHT_COLOR;
extern bool IsAi;
extern SurakartaPlayer MY_COLOR;
extern int AI_SEARCH_DEPTH;
extern int DEAD_SEARCH_DEPTH;

#endif // SETTINGS_H
