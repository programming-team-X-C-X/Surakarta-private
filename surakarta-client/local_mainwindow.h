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
#include "chess_board_widght.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LocalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
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
    void sendCaptureHints(const std::vector<SurakartaPosition>& hints);
    void sendNONCaptureHints(const std::vector<SurakartaPosition>& hints);

protected:
    void paintEvent(QPaintEvent */*event*/) override;

private:
    SurakartaGame *game;
    std::shared_ptr<SurakartaAgentMine>   agentMine = std::make_shared<SurakartaAgentMine>(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
    ChessBoardWidget *chessBoard;
    QTimer *computerMoveTimer;
    SurakartaGameMode gameMode;
    QPushButton *buttonGiveUp;
    QPushButton *buttonClose;
    QPushButton *buttonStart;
    QPushButton *buttonBack;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
    QTimer *countdownTimer;
    QLabel *countdownLabel;
    QLabel *currentRoundLabel;
    QLabel *currentPlayerLabel;
    QLabel *playerInfoLabel;
    QLabel *remainingPiecesLabel;
    int computerMoveDelay = 500; // 电脑走棋的延迟时间
    int countdownTime = TIME_LIMIT;
    int arcNum = (BOARD_SIZE - 2) / 2;//环数
    int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
    int gridSize = WINDOW_SIZE / rawNum;

};


#endif // LOCAL_MAINWINDOW_H
