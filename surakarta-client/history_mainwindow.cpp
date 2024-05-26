#include "history_mainwindow.h"
#include "ui_history_mainwindow.h"
#include "info_piece.h"
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <iostream>




History_MainWindow::History_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::History_MainWindow)
{
    ui->setupUi(this);
    QString directoryPath = "../history";

    // 打开该路径下
    QDir directory(directoryPath);

    // 找到对应文件
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList fileList = directory.entryList();

    // 加入下拉框
    ui->comboBox->addItems(fileList);

    //  未介入
    isDoing = false;
}

History_MainWindow::~History_MainWindow()
{
    delete ui;
}

void History_MainWindow::on_pushButton_clicked()
{
    // 点击确定开始载入游戏
    qDebug() << "Failed to open file";

    // 初始化
    //std::vector<SurakartaBoard> boards;

    game_info = "";
    moves.clear();
    //boards.clear();
    step = 0;
    game = new SurakartaGame;
    game->StartGame();
    //boards.push_back(*game->board_.get());

    // 放入最初始的状态
    mini_board *newBoard = new mini_board;
    *newBoard = *(game->board_.get());
    boards.push_back(*newBoard);

    // 绑定 打开 对应文件
    QFile file("../history/" + ui->comboBox->currentText());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file here";
    }

    QTextStream read_game(&file);

    game_info = read_game.readLine();

    // 将文件存储变成对应步数
    QStringList moveList = game_info.split(' ');
    max_step = moveList.size() - 1;
    // 5步  step 最高取 4

    for(unsigned i = 0; i < moveList.size() - 1;i++)
    {
        SurakartaMove move;
        SurakartaBoard board_{BOARD_SIZE};

        move.from = SurakartaPosition(moveList[i][0].unicode() - 65,moveList[i][1].digitValue() - 1);
        move.to   = SurakartaPosition(moveList[i][3].unicode() - 65,moveList[i][4].digitValue() - 1);
        move.player = static_cast<SurakartaPlayer> (i%2);

        game->Move(move);

        moves.push_back(move);

        // 放入每一步
        mini_board *newBoard = new mini_board;
        *newBoard = *(game->board_.get());
        boards.push_back(*newBoard);
    }

    // 创建对局
    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);


    QToolBar* toolBar = addToolBar("Tools");

    // 放入 开始  上一步  下一步   介入对局  按钮
    QAction* startPauseAction = new QAction("开始播放");
    QAction* preAction = new QAction("上一步");
    QAction* nextAction = new QAction("下一步");
    QAction* doAction  = new QAction("介入对局");

    toolBar->addAction(startPauseAction);
    toolBar->addAction(preAction);
    toolBar->addAction(nextAction);
    toolBar->addAction(doAction);
    // 轮数
    roundLabel = new QLabel("当前轮数: 0", this);
    ui->statusbar->addWidget(roundLabel);

    // 跳转
    jumpLineEdit = new QLineEdit(this);
    jumpButton = new QPushButton("跳转", this);
    jumpLineEdit->setFixedSize(50, 20);
    ui->statusbar->addWidget(new QLabel("跳转到步数:", this));
    ui->statusbar->addWidget(jumpLineEdit);
    ui->statusbar->addWidget(jumpButton);

    // 初始点击状态
    preAction->setDisabled(true);
    nextAction->setDisabled(true);
    jumpButton->setDisabled(true);


    connect(nextAction, &QAction::triggered, this, &History_MainWindow::onNextButtonClicked);
    connect(preAction, &QAction::triggered, this, &History_MainWindow::onPreButtonClicked);
    connect(doAction,&QAction::triggered, this,[=](){
        if(doAction->text() == "介入对局")
        {
            // 禁用系列按钮
            nextAction->setDisabled(true);
            preAction->setDisabled(true);
            jumpButton->setDisabled(true);
            startPauseAction->setDisabled(true);

            // 初始化游戏
            initGame();


            // 连接对应信号
            connect(chessBoard, &ChessBoardWidget::requestHints, this, &History_MainWindow::provideHints);
            connect(this, &History_MainWindow::sendCaptureHints, chessBoard, &ChessBoardWidget::receiveCaptureHints);
            connect(this, &History_MainWindow::sendNONCaptureHints, chessBoard, &ChessBoardWidget::receiveNONCaptureHints);
            connect(chessBoard, &ChessBoardWidget::playerMove, this, &History_MainWindow::playerMove);

            // 更新文字 状态
            doAction->setText("恢复原状态");
            isDoing = true;
        }

        else
        {
            // 启用系列按钮
            nextAction->setDisabled(false);
            preAction->setDisabled(false);
            jumpButton->setDisabled(false);
            startPauseAction->setDisabled(false);

            // 恢复原状
            loadGame(step);

            // 断开相应信号
            disconnect(chessBoard, &ChessBoardWidget::requestHints, this, &History_MainWindow::provideHints);
            disconnect(this, &History_MainWindow::sendCaptureHints, chessBoard, &ChessBoardWidget::receiveCaptureHints);
            disconnect(this, &History_MainWindow::sendNONCaptureHints, chessBoard, &ChessBoardWidget::receiveNONCaptureHints);
            disconnect(chessBoard, &ChessBoardWidget::playerMove, this, &History_MainWindow::playerMove);

            // 更新文字
            doAction->setText("介入对局");
            isDoing = false;

        }
    });

    connect(this,&History_MainWindow::reachMax,[=](){

        nextAction->setDisabled(true);
    });

    connect(this,&History_MainWindow::reachMin,[=](){
        preAction->setDisabled(true);
    });

    connect(this,&History_MainWindow::deMax,[=](){
        nextAction->setDisabled(false);
    });

    connect(this,&History_MainWindow::deMin,[=](){
        preAction->setDisabled(false);
    });


    connect(this,&History_MainWindow::disableJump,[=](){
        jumpButton->setDisabled(true);
    });

    connect(this,&History_MainWindow::enableJump,[=](){
        jumpButton->setDisabled(false);
    });

    connect(startPauseAction, &QAction::triggered, this, [=](){

        if(startPauseAction->text() == "开始播放")
        {

            startPauseAction->setText("暂停");

            preAction->setDisabled(true);
            nextAction->setDisabled(true);
            jumpButton->setDisabled(true);
            timer = new QTimer;
            timer->start(3000);

            connect(timer,&QTimer::timeout,this,[=](){
                if(step < max_step)
                {
                    chessBoard->movePiece(moves[step]);
                    step++;
                    roundLabel->setText(QString("当前轮数: %1").arg(step));
                }
                else
                {
                    timer->stop();
                }
            });


        }
        else
        {
            timer->stop();

            startPauseAction->setText("开始播放");

            preAction->setDisabled(false);
            nextAction->setDisabled(false);
            jumpButton->setDisabled(false);
        }
    });

    connect(jumpButton, &QPushButton::clicked, this, &History_MainWindow::onJumpButtonClicked);

    connect(chessBoard,&ChessBoardWidget::animationFinished,[=](){
        if(!isDoing)
        {
            emit deMax();
            emit deMin();
            emit enableJump();
            if(step == max_step) emit reachMax();
            if(step <= 0) emit reachMin();
        }
    });


}

void History_MainWindow::onNextButtonClicked()
{
    if(step == max_step)
    {
        return;
    }

    emit reachMin();
    emit reachMax();
    emit disableJump();


    chessBoard->movePiece(moves[step]);
    step++;
    roundLabel->setText(QString("当前轮数: %1").arg(step));

}

void History_MainWindow::onPreButtonClicked()
{
    emit deMax();
    emit enableJump();


    loadGame(--step);
    roundLabel->setText(QString("当前轮数: %1").arg(step));
    //

    if(step <= 0)
    {
        emit reachMin();
    }
}

void History_MainWindow::onJumpButtonClicked()
{

    int targetStep = jumpLineEdit->text().toInt();
    if(targetStep >= 0 && targetStep < max_step) {
        step = targetStep;
        loadGame(step);
        roundLabel->setText(QString("当前轮数: %1").arg(step));
    }
}

void History_MainWindow::loadGame(unsigned cur_step)
{
    // mini_board  ---->   SuarkartaBoards

    // 清空棋盘
    if (chessBoard) {
        delete chessBoard;
    }

    chessBoard = new ChessBoardWidget(boards[cur_step]);
    connect(chessBoard,&ChessBoardWidget::animationFinished,[=](){
        if(!isDoing)
        {
            emit deMax();
            emit deMin();
            emit enableJump();
            if(step == max_step) emit reachMax();
            if(step <= 0) emit reachMin();
        }
    });
    setCentralWidget(chessBoard);
}

void History_MainWindow::initGame()
{
    if(game) delete game;

    game = new SurakartaGame;
    game->StartGame();

    // 初始化游戏
    for(int i = 0; i < step;i++)
    {
        game->Move(moves[i]);
    }

    PLAYER_COLOR = RIGHT_COLOR = ((step % 2) ==  0) ? 1 : 0;
}


void History_MainWindow::provideHints(SurakartaPosition pos) {
    auto captureHints = game->rule_manager_->GetAllLegalCaptureTarget(pos);
    std::vector<SurakartaPosition> captureHintVector = *captureHints;
    emit sendCaptureHints(captureHintVector);
    auto NONcaptureHints = game->rule_manager_->GetAllLegalNONCaptureTarget(pos);
    std::vector<SurakartaPosition> NONcaptureHintVector = *NONcaptureHints;
    emit sendNONCaptureHints(NONcaptureHintVector);
}

void History_MainWindow::playerMove(SurakartaPosition from, SurakartaPosition to) {
    if (game->IsEnd()) return;
    SurakartaMove move(from, to, game->GetGameInfo()->current_player_);

    // game.Move(move);
    if (game->Move(move).IsLegal())
        chessBoard->movePiece(move);
    PLAYER_COLOR = !PLAYER_COLOR;
    RIGHT_COLOR = !RIGHT_COLOR;

}
