#ifndef HISTORY_MAINWINDOW_H
#define HISTORY_MAINWINDOW_H

#include <QMainWindow>
#include "info_game.h"
#include "info_common.h"
#include "settings.h"
#include "chess_board_widght.h"
#include <QToolBar>
class mini_board;


namespace Ui {
class History_MainWindow;
}

class History_MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit History_MainWindow(QWidget *parent = nullptr);
    ~History_MainWindow();

    void GetMove();

private slots:
    void on_pushButton_clicked();
    void onNextButtonClicked();
    void onPreButtonClicked();

    SurakartaBoard* toSuraBoard(const mini_board& board);
    void loadGame(unsigned cur_step);


private:
    // 存储移动步
    std::vector<SurakartaMove> moves;


   // std::vector<SurakartaBoard> boards;

    std::vector<mini_board> boards;

    // 当前步数
    unsigned step;

    // 文件中的游戏信息
    QString game_info;

    // 游戏
    SurakartaGame *game;
    // 棋盘
    ChessBoardWidget *chessBoard;

    Ui::History_MainWindow *ui;
};

#endif // HISTORY_MAINWINDOW_H
