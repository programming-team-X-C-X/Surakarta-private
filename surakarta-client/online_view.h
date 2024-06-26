#ifndef ONLINE_VIEW_H
#define ONLINE_VIEW_H

#include <QMainWindow>
#include "info_game.h"
#include "agent_mine.h"
#include "chess_board_widght.h"
#include <qthread.h>

namespace Ui {
class game_view;
}

class GameView : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameView(QWidget *parent = nullptr);
    ~GameView();
    // 需要游戏，棋盘的图形界面
    void endShow(SurakartaEndReason rea, QString color, QString round);    void computerMove();
    void update_gameinfo();
    void update_time();
    void restartGame();
    void provideHints(SurakartaPosition pos);
    void onAIMoveComputed(const SurakartaMove& move);
    void onDEADCalculateComputed(std::vector<SurakartaPosition> pos);
    void startAIThread();
    void startDEADCalculatorThread(SurakartaPosition fromPos, std::vector<SurakartaPosition> toPos);
    SurakartaGame game;
    ChessBoardWidget *chessBoard;
    int left_time ;

private slots:
    // void on_pushButton_clicked();
    void on_useAi_clicked();
    void on_giveup_button_clicked();
    void Move(SurakartaPosition from, SurakartaPosition to);
    void AfterAnimationFinished();
    void updateProgress(int progress);

private:
    Ui::game_view *ui;
    std::shared_ptr<SurakartaAgentMine>agentMine = std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    QThread *aiThread;
    // QThread *DEADCalculateThread;

signals:
    void sendCaptureHints(const std::vector<SurakartaPosition>& hints);
    void sendDengerousHints(const std::vector<SurakartaPosition>& hints);
    void sendNONCaptureHints(const std::vector<SurakartaPosition>& hints);
    void AskMove(SurakartaMove move);
    void Resign();
    void AnimationFinished();
    void AskComputerMove();
    void restart_game();
    void return_start();
};

#endif // ONLINE_VIEW_H
