#ifndef HISTORY_MAINWINDOW_H
#define HISTORY_MAINWINDOW_H

#include <QMainWindow>
#include "info_game.h"
#include "info_common.h"
#include "chess_board_widght.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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

signals:
    // 步数达到最大/最小时 禁用对应的按钮
    void reachMax();
    void reachMin();

    // 步数非最大/最小时 启用对应的按钮
    void deMax();
    void deMin();

    // 进行（未）移动时禁止（允许）跳转
    void enableJump();
    void disableJump();

    // 介入对局时的相关信号
    void sendCaptureHints(const std::vector<SurakartaPosition>& hints);
    void sendNONCaptureHints(const std::vector<SurakartaPosition>& hints);

private slots:
    // 选择完对局的处理
    void on_pushButton_clicked();

    // 下一步
    void onNextButtonClicked();

    // 上一步
    void onPreButtonClicked();

    // 跳转到指定步数
    void onJumpButtonClicked();

    // 更改当前执棋颜色
    void changeGameColor();

    // 加载某一步数的棋盘
    void loadGame(unsigned cur_step);

    // 介入游戏时，进行对应初始化
    void initGame();

    // 介入对局的相关槽函数
    void provideHints(SurakartaPosition pos);
    void playerMove(SurakartaPosition from, SurakartaPosition to);

private:
    // 存储移动步
    std::vector<SurakartaMove> moves;
    std::vector<MiniBoard> boards;

    // 当前步数和最大的步数
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
    QLabel *player_color;
    QString game_color;
    QString userin_color;
    QLineEdit *jumpLineEdit;
    QPushButton *jumpButton;

    // 是否正在介入
    bool isDoing;
    // 是否播放
    bool isStaring;
};

#endif // HISTORY_MAINWINDOW_H
