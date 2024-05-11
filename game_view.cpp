#include "game_view.h"
#include "ui_game_view.h"
#include <QDebug>
#include "dialog.h"
game_view::game_view(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game_view)
{
    // 初始化
    left_time = TIME_LIMIT;
    gameround = 1;

    ui->setupUi(this);
    setWindowTitle(tr("surakarta"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);

    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);

    ui->giveup_button->raise();

    connect(chessBoard, &ChessBoardWidget::playerMove, this, &game_view::MOVE);
    //connect(chessBoard, &ChessBoardWidget::animationFinished, this, &GameMainWindow::judgeEnd);
    connect(chessBoard, &ChessBoardWidget::requestHints, this, &game_view::provideHints);
    connect(this, &game_view::sendHints, chessBoard, &ChessBoardWidget::receiveHints);
    game.StartGame();
    show();
}

game_view::~game_view()
{
    delete ui;
}

void game_view::ENDSHOW(SurakartaEndReason rea, QString color, QString round)
{
    Dialog *EView = new Dialog(this);
    EView->SetInfo(rea,color,round);

    connect(EView,&Dialog::restart_game,this,&game_view::restart_game);
    connect(EView,&Dialog::return_start,this,&game_view::return_start);

    EView->exec();
}

void game_view::computerMove()
{
    qDebug() << "开始开始 ";
    if(game.game_info_->current_player_ == game.game_info_->player_color_)
    {
        //
        qDebug() << "进入移动";
        SurakartaMove move = agentMine->CalculateMove();
        // 自己的回合就走

        // 发起行棋请求
        emit AskMove(move);
        qDebug() << "computerMove End";
    }

}

void game_view::update_gameinfo()
{
    ui->Lname->setText(QString(name));
    ui->Lcolor->setText((game.game_info_->current_player_ == PieceColor::BLACK) ? "BLACK" : "WHITE");
    ui->Lplayercolor->setText((PLAYER_COLOR == 1) ? "BLACK" : "WHITE");
    ui->Lround ->setText(QString::number(gameround));
}

void game_view::update_time()
{
    qDebug() << left_time ;
    ui->Ltime->setText( QString::number(left_time--));
    if(left_time < 0)
    {
        left_time = 0;
    }
}



void game_view::provideHints(SurakartaPosition pos) {
    auto hints = game.rule_manager_->GetAllLegalTarget(pos);
    std::vector<SurakartaPosition> hintVector = *hints;
    emit sendHints(hintVector);
}

// void game_view::on_pushButton_clicked()
// {   SurakartaMove move({0,1},{0,2},PieceColor::BLACK);
//     game.Move(move);
//     chessBoard->movePiece(move);
//     qDebug() << "正在点击";
// }


void game_view::on_giveup_button_clicked()
{
    // 点击就发出信号
    emit Resign();
}

void game_view::MOVE(SurakartaPosition from, SurakartaPosition to)
{
    SurakartaMove move(from, to, game.GetGameInfo()->current_player_);
    AskMove(move);
}

