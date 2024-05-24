#ifndef HISTORY_MAINWINDOW_H
#define HISTORY_MAINWINDOW_H

#include <QMainWindow>
#include "info_game.h"
#include "info_common.h"
#include "settings.h"
#include "chess_board_widght.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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

signals:
    void reachMax();
    void reachMin();
    void deMax();
    void deMin();

    void enableJump();
    void disableJump();

    // 介入对局时的相关信号

    void sendCaptureHints(const std::vector<SurakartaPosition>& hints);
    void sendNONCaptureHints(const std::vector<SurakartaPosition>& hints);

private slots:
    void on_pushButton_clicked();
    void onNextButtonClicked();
    void onPreButtonClicked();
    void onJumpButtonClicked();


    void loadGame(unsigned cur_step);
    void initGame();

    // 介入对局的相关槽函数
    void provideHints(SurakartaPosition pos);
    void playerMove(SurakartaPosition from, SurakartaPosition to);

private:
    // 存储移动步
    std::vector<SurakartaMove> moves;


   // std::vector<SurakartaBoard> boards;

    std::vector<mini_board> boards;

    // 当前步数
    int step;
    int max_step;

    // 文件中的游戏信息
    QString game_info;

    // 游戏
    SurakartaGame *game;
    // 棋盘
    ChessBoardWidget *chessBoard;

    Ui::History_MainWindow *ui;
    QTimer* timer;

    QLabel *roundLabel;

    QLineEdit *jumpLineEdit;

    QPushButton *jumpButton;

    // 是否正在介入
    bool isDoing;
};

#endif // HISTORY_MAINWINDOW_H
