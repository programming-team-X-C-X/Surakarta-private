#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <QMainWindow>
#include "info_game.h"
#include "agent_random.h"
#include "chess_board_widght.h"
#include "settings.h"

namespace Ui {
class game_view;
}

class game_view : public QMainWindow
{
    Q_OBJECT

public:

    int left_time ;
    explicit game_view(QWidget *parent = nullptr);
    ~game_view();
    // 需要游戏，棋盘的图形界面
    SurakartaGame game;
    ChessBoardWidget *chessBoard;
    void ENDSHOW(SurakartaEndReason rea, QString color, QString round);
    void computerMove();
    void update_gameinfo();
    void update_time();
    void restartGame();
    void provideHints(SurakartaPosition pos);
    std::shared_ptr<SurakartaAgentRandom>   agentMine = std::make_shared<SurakartaAgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
private slots:
    // void on_pushButton_clicked();

    void on_giveup_button_clicked();
    void MOVE(SurakartaPosition from, SurakartaPosition to);



private:
    Ui::game_view *ui;
signals:
    void sendHints(const std::vector<SurakartaPosition>& hints);
    void AskMove(SurakartaMove move);
    void Resign();

    void restart_game();
    void return_start();
};

#endif // GAME_VIEW_H
