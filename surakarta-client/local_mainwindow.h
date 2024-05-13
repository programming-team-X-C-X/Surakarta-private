#ifndef LOCAL_MAINWINDOW_H
#define LOCAL_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include "info_game.h"
#include "agent_mine.h"
// #include "my_agent_random.h"
#include "chess_board_widght.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// 在绘制事件中调用此函数来绘制实心点

class LocalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SurakartaGame *game;//顺序讲究
    // std::shared_ptr<SurakartaAgentRandom> agent     = std::make_shared<SurakartaAgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    std::shared_ptr<SurakartaAgentMine>   agentMine = std::make_shared<SurakartaAgentMine>(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
    ChessBoardWidget *chessBoard;
    QTimer *computerMoveTimer;
    int computerMoveDelay = 500; // 电脑走棋的延迟时间，1000毫秒
    int countdownTime = TIME_LIMIT;
    int arcNum = (BOARD_SIZE - 2) / 2;//环数
    int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
    int gridSize = WINDOW_SIZE / rawNum;
    QTimer *countdownTimer;
    QLabel *countdownLabel;
    QLabel *currentRoundLabel;
    QLabel *currentPlayerLabel;
    QLabel *playerInfoLabel;
    QLabel *remainingPiecesLabel;
    LocalMainWindow(/*QWidget *parent = nullptr, */SurakartaGameMode gameMode);
    ~LocalMainWindow();
    void computerMove();
    void startComputerMove();
    void showEndDialog();
    void updateCountdownDisplay();
    void updateGameInfo();

public slots:
    void playerMove(SurakartaPosition from, SurakartaPosition to);
    void Initialize();
    void updateCountdown();
    void giveUp();
    void restartGame();
    void provideHints(SurakartaPosition pos);
    void judgeEnd();

signals:
    void backToStartDialog();
    void sendHints(const std::vector<SurakartaPosition>& hints);

protected:
        void paintEvent(QPaintEvent */*event*/) override;

private:
        SurakartaGameMode gameMode;
    QPushButton *buttonGiveUp;
    QPushButton *buttonClose;
    QPushButton *buttonStart;
    QPushButton *buttonBack;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
};


#endif // LOCAL_MAINWINDOW_H
