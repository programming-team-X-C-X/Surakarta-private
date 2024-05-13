#include "game_view.h"
#include "ui_game_view.h"
#include <QDebug>
#include <QTimer>
game_view::game_view(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game_view)
{
    // 初始化

    ui->setupUi(this);
    setWindowTitle(tr("surakarta"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);
    game.StartGame();

}

game_view::~game_view()
{
    delete ui;
}

void game_view::MOVE(SurakartaPosition from, SurakartaPosition to)
{
    SurakartaMove move(from, to, game.GetGameInfo()->current_player_);
    AskMove(move);
}

void game_view::computerMove()
{
    //QTimer timer;
    //timer.start(2000);

    //connect(timer,&QTimer::timeout,);
    SurakartaMove move = agentMine->CalculateMove();
    AskMove(move);
}
