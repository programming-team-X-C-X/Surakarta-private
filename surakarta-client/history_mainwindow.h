#ifndef HISTORY_MAINWINDOW_H
#define HISTORY_MAINWINDOW_H

#include <QMainWindow>
#include "info_game.h"
#include "info_common.h"
#include "settings.h"
#include "chess_board_widght.h"
#include <QToolBar>

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
    void loadGame(unsigned cur_step);


private:
    // 存储移动步
    std::vector<SurakartaMove> moves;

    // 存储棋盘状态
    struct mini_board{
        mini_board()
        {
            board.resize(BOARD_SIZE);
        }
        std::vector<std::vector<PieceColor>> board;

        void operator=(const SurakartaBoard& bd)
        {
            for(unsigned i = 0;i < BOARD_SIZE;++i)
            {
                for(unsigned j = 0;j < BOARD_SIZE;++j)
                {
                    board[i].push_back(bd[i][j]->color_);
                }
            }
        }
    };
   // std::vector<SurakartaBoard> boards;

    //std::vector<mini_board> boards;

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
