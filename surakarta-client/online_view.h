#ifndef ONLINE_VIEW_H
#define ONLINE_VIEW_H

#include <QMainWindow>
#include "info_game.h"
#include "agent_random.h"
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

    int left_time ;
    explicit GameView(QWidget *parent = nullptr);
    ~GameView();
    // 需要游戏，棋盘的图形界面
    SurakartaGame game;
    ChessBoardWidget *chessBoard;
    void endShow(SurakartaEndReason rea, QString color, QString round);    void computerMove();
    void update_gameinfo();
    void update_time();
    void restartGame();
    void provideHints(SurakartaPosition pos);
    // std::shared_ptr<SurakartaAgentRandom>   agentMine = std::make_shared<SurakartaAgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    std::shared_ptr<SurakartaAgentMine>agentMine = std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    void onAIMoveComputed(const SurakartaMove& move);
    void startAIThread();

private slots:
    // void on_pushButton_clicked();
    void on_useAi_clicked();
    void on_giveup_button_clicked();
    void Move(SurakartaPosition from, SurakartaPosition to);
    void AfterAnimationFinished();

private:
    Ui::game_view *ui;
    QThread *aiThread;
signals:
    void sendCaptureHints(const std::vector<SurakartaPosition>& hints);
    void sendNONCaptureHints(const std::vector<SurakartaPosition>& hints);
    void AskMove(SurakartaMove move);
    void Resign();
    void AnimationFinished();
    void AskComputerMove();
    void restart_game();
    void return_start();
};

#endif // ONLINE_VIEW_H
