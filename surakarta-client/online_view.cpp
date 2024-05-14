#include "online_view.h"
#include "ui_online_view.h"
#include <QDebug>
#include "online_end_dialog.h"
GameView::GameView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game_view)
{
    // 初始化
    left_time = TIME_LIMIT;
    gameround = 1;
    RIGHT_COLOR = 1;

    ui->setupUi(this);
    setWindowTitle(tr("surakarta"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);

    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);

    ui->giveup_button->raise();
    ui->useAi->raise();

    connect(chessBoard, &ChessBoardWidget::playerMove, this, &GameView::Move);
    //connect(chessBoard, &ChessBoardWidget::animationFinished, this, &GameMainWindow::judgeEnd);
    connect(chessBoard, &ChessBoardWidget::requestHints, this, &GameView::provideHints);
    connect(this, &GameView::sendHints, chessBoard, &ChessBoardWidget::receiveHints);
    game.StartGame();
    show();
}

GameView::~GameView()
{
    delete ui;
}

void GameView::endShow(SurakartaEndReason rea, QString color, QString round)
{
    OnlineEndDialog *EView = new OnlineEndDialog(this);
    EView->SetInfo(rea,color,round);

    connect(EView,&OnlineEndDialog::restart_game,this,&GameView::restart_game);
    connect(EView,&OnlineEndDialog::return_start,this,&GameView::return_start);

    EView->exec();
}

void GameView::computerMove()
{
    // qDebug() << "开始开始 ";
    if(!game.IsEnd() && game.game_info_->current_player_ == mycolor){
        SurakartaMove move = agentMine->CalculateMove();
        emit AskMove(move);
    }

}

void GameView::update_gameinfo()
{
    ui->Lname->setText(QString(name));
    ui->Lcolor->setText((game.game_info_->current_player_ == PieceColor::BLACK) ? "BLACK" : "WHITE");
    ui->Lplayercolor->setText((PLAYER_COLOR == 1) ? "BLACK" : "WHITE");
    ui->Lround ->setText(QString::number(gameround));
}

void GameView::update_time()
{
    qDebug() << left_time ;
    ui->Ltime->setText( QString::number(left_time--));
    if(left_time < 0)
    {
        left_time = 0;
    }
}



void GameView::provideHints(SurakartaPosition pos) {
    auto hints = game.rule_manager_->GetAllLegalTarget(pos);
    std::vector<SurakartaPosition> hintVector = *hints;
    emit sendHints(hintVector);
}

// void GameView::on_pushButton_clicked()
// {   SurakartaMove move({0,1},{0,2},PieceColor::BLACK);
//     game.Move(move);
//     chessBoard->movePiece(move);
//     qDebug() << "正在点击";
// }


void GameView::on_giveup_button_clicked()
{
    // 点击就发出信号
    emit Resign();
}

void GameView::Move(SurakartaPosition from, SurakartaPosition to)
{

    SurakartaMove move(from, to, game.GetGameInfo()->current_player_);
    emit AskMove(move);
}


void GameView::on_useAi_clicked()
{
    if(ui->useAi->text() == "AI托管"){
        ui->useAi->setText("取消托管");
        IsAi = 1;
    }
    else {
        ui->useAi->setText("AI托管");
        IsAi = 0;
    }
}
