#ifndef SETTINGS_H
#define SETTINGS_H
#include "info_piece.h"
#include <QString>
// int BOARD_SIZE = 6;
extern unsigned int BOARD_SIZE;
extern unsigned int WINDOW_SIZE;
extern int arcNum;
extern int rawNum;
extern int gridSize;
extern const unsigned int SLEEP_TIME;
extern const unsigned int PIECE_SIZE;
extern double PIECE_SPEED;
extern unsigned int TIME_LIMIT;
extern unsigned int MAX_NO_CAPTURE_ROUND;
extern bool PLAYER_COLOR;
extern int gameround;
extern QString name;
extern bool RIGHT_COLOR;
extern SurakartaPlayer mycolor;
#endif // SETTINGS_H
