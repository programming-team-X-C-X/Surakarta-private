#include "local_mainwindow.h"
#include "local_end_dialog.h"
#include <QDebug>

LocalMainWindow::LocalMainWindow(/*QWidget *parent*/SurakartaGameMode gameMode)
    // : QMainWindow(parent)
    :game(new SurakartaGame(gameMode)),
    gameMode(gameMode)
{
    setWindowTitle(tr("苏拉卡尔塔棋 --programming-team-X-C-X --Powered by Qt 6.8.0"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);

    buttonClose = new QPushButton("退出游戏", this);
    buttonStart = new QPushButton("开始游戏", this);
    buttonBack = new QPushButton("回到主菜单", this);
    playerInfoLabel = new QLabel(this);
    buttonStart->setFixedSize(100, 30);
    buttonBack->setFixedSize(100, 30);
    buttonClose->setFixedSize(100, 30);

    if (game->game_mode_ == SurakartaGameMode::PVE) {
        game->white_player_ = PLAYER_COLOR ? SurakartaRealPlayer::COMPUTER : SurakartaRealPlayer::PLAYER;
        game->black_player_ = PLAYER_COLOR ? SurakartaRealPlayer::PLAYER : SurakartaRealPlayer::COMPUTER;
    }
    else if (game->game_mode_ == SurakartaGameMode::EVE) {
        game->white_player_ = SurakartaRealPlayer::COMPUTER;
        game->black_player_ = SurakartaRealPlayer::COMPUTER;
    }
    else if (game->game_mode_ == SurakartaGameMode::PVP) {
        game->white_player_ = SurakartaRealPlayer::PLAYER;
        game->black_player_ = SurakartaRealPlayer::PLAYER;
    }
    // QString whiteColor = game->GetGameInfo()->player_color_ == SurakartaPlayer::BLACK ? QString("BLACK") : QString("WHITE");
    // QString blackColor = game->GetGameInfo()->computer_color_ == SurakartaPlayer::BLACK ? QString("BLACK") : QString("WHITE");
    QString whiteColor = game->black_player_ == SurakartaRealPlayer::COMPUTER ? QString("电脑") : QString("玩家");
    QString blackColor = game->white_player_ == SurakartaRealPlayer::COMPUTER ? QString("电脑") : QString("玩家");
    QString playerInfo = QString("黑方：%1\n白方：%2").arg(whiteColor, blackColor);
    playerInfoLabel->setText(playerInfo);

    QVBoxLayout *vlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();

    vlayout->addStretch(10);
    vlayout->addWidget(playerInfoLabel);
    vlayout->addStretch(20);
    vlayout->addWidget(buttonStart);
    vlayout->addStretch(1);
    vlayout->addWidget(buttonBack);
    vlayout->addStretch(1);
    vlayout->addWidget(buttonClose);
    vlayout->addStretch(1);
    vlayout->addStretch(4);
    hlayout->addStretch(6);
    hlayout->addLayout(vlayout);
    hlayout->addStretch(1);
    // setLayout(hlayout);
    // 创建一个新的容器并设置布局
    QWidget *container = new QWidget();
    container->setLayout(hlayout);
    setCentralWidget(container);
    connect(buttonClose, &QPushButton::clicked, this, &LocalMainWindow::close);
    connect(buttonStart, &QPushButton::clicked, this,  &LocalMainWindow::Initialize);
    connect(buttonStart, &QPushButton::clicked, buttonStart, &QPushButton::deleteLater);
    connect(buttonBack, &QPushButton::clicked, this, &LocalMainWindow::backToStartDialog);

}

void LocalMainWindow::Initialize() {
    chessBoard = new ChessBoardWidget();
    buttonGiveUp = new QPushButton("认输", this);
    // buttonGiveUp->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*4/6, 100, 30);

    setCentralWidget(chessBoard);
    // chessBoard->setGeometry(0, 0, WINDOW_SIZE, WINDOW_SIZE);
    connect(buttonGiveUp, &QPushButton::clicked, this, &LocalMainWindow::giveUp);
    connect(chessBoard, &ChessBoardWidget::playerMove, this, &LocalMainWindow::playerMove);
    connect(chessBoard, &ChessBoardWidget::animationFinished, this, &LocalMainWindow::judgeEnd);
    connect(chessBoard, &ChessBoardWidget::requestHints, this, &LocalMainWindow::provideHints);
    connect(this, &LocalMainWindow::sendHints, chessBoard, &ChessBoardWidget::receiveHints);

    playerInfoLabel = new QLabel(this);
    currentPlayerLabel = new QLabel(this);
    remainingPiecesLabel = new QLabel(this);
    currentRoundLabel = new QLabel(this);

    countdownLabel = new QLabel(this);
    QFont labelFont = countdownLabel->font();
    labelFont.setPointSize(50);
    countdownLabel->setText(tr("剩余时间: %1").arg(countdownTime));

    computerMoveTimer = new QTimer(this);
    computerMoveTimer->setSingleShot(true);
    connect(computerMoveTimer, &QTimer::timeout, this, &LocalMainWindow::computerMove);

    countdownTimer = new QTimer(this);
    countdownTimer->setInterval(1000); // 设置定时器每秒触发一次
    connect(countdownTimer, &QTimer::timeout, this, &LocalMainWindow::updateCountdown);
    // 创建一个新的布局
    QVBoxLayout *vlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();
    // 将按钮添加到布局中
    buttonClose->setFixedSize(100, 30);
    buttonGiveUp->setFixedSize(100, 30);
    // buttonStart->setFixedSize(100, 30);
    vlayout->addStretch(10);
    vlayout->addWidget(playerInfoLabel);
    vlayout->addWidget(currentRoundLabel);
    vlayout->addWidget(currentPlayerLabel);
    vlayout->addWidget(countdownLabel);
    vlayout->addStretch(20);
    vlayout->addWidget(buttonGiveUp);
    vlayout->addStretch(1);
    vlayout->addWidget(buttonClose);
    vlayout->addStretch(1);
    vlayout->addStretch(4);
    // 将棋盘添加到布局中
    chessBoard->setFixedSize(WINDOW_SIZE/0.9, WINDOW_SIZE/0.9);
    hlayout->addWidget(chessBoard);
    hlayout->addStretch(1);
    hlayout->addLayout(vlayout);
    hlayout->addStretch(1);
    // 创建一个新的容器并设置布局
    QWidget *container = new QWidget();
    container->setLayout(hlayout);
    // 将容器设置为中央组件
    // container->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*4/6, 100, 30);
    setCentralWidget(container);

    game->StartGame();
    chessBoard->board = game->board_;
    // chessBoard->setMode(ChessBoardWidget::PieceMode);
    countdownTime = TIME_LIMIT;
    countdownTimer->start();
    judgeEnd();
    // if (game.game_info_->current_player_ == game.GetGameInfo()->computer_color_)
    //     startComputerMove();
}

void LocalMainWindow::judgeEnd() {
    updateGameInfo();
    if (game->IsEnd() && game->GetGameInfo()->end_reason_ != SurakartaEndReason::ILLIGAL_MOVE)
        showEndDialog();
    // if (game->game_mode_ == SurakartaGameMode::PVE || game->game_mode_ == SurakartaGameMode::EVE) {
    // if (game->game_info_->current_player_ == game->GetGameInfo()->computer_color_)
    if (game->game_info_->current_player_ == SurakartaPlayer::BLACK) {
        if (game->black_player_ == SurakartaRealPlayer::COMPUTER)
            startComputerMove();
    }
    else if (game->game_info_->current_player_ == SurakartaPlayer::WHITE) {
        if (game->white_player_ == SurakartaRealPlayer::COMPUTER)
            startComputerMove();
    }
    // }

}

void LocalMainWindow::updateGameInfo() {
    // judgeEnd();
    // chessBoard->setMode(ChessBoardWidget::PieceMode);
    QString player = game->GetGameInfo()->current_player_ == SurakartaPlayer::BLACK ? QString("BLACK") : QString("WHITE");
    QString currentPlayer = QString("当前回合：%1").arg(player);
    QString currentRound = QString("当前回合数：%1").arg(game->GetGameInfo()->num_round_);
    currentPlayerLabel->setText(currentPlayer);
    currentRoundLabel->setText(currentRound);
}

void LocalMainWindow::computerMove() {
    SurakartaMove move = agentMine->CalculateMove();
    game->Move(move);
    chessBoard->movePiece(move);
    // updateGameInfo();
}

void LocalMainWindow::playerMove(SurakartaPosition from, SurakartaPosition to) {
    if (game->IsEnd()) return;
    SurakartaMove move(from, to, game->GetGameInfo()->current_player_);

    // game.Move(move);
    if (game->Move(move).IsLegal())
        chessBoard->movePiece(move);
    updateGameInfo();
}

void LocalMainWindow::startComputerMove() {
    // countdownTimer->stop();
    countdownTime = TIME_LIMIT;
    computerMoveTimer->start(computerMoveDelay);
}

void LocalMainWindow::updateCountdown() {
    countdownTime--;
    updateCountdownDisplay();
    if (countdownTime <= 0) {
        // countdownTimer->stop();
        game->game_info_->winner_ = ReverseColor(game->game_info_->current_player_);
        game->game_info_->end_reason_ = SurakartaEndReason::TIMEOUT;
        buttonGiveUp->hide();
        showEndDialog();
    }
}

void LocalMainWindow::updateCountdownDisplay() {
    QString color = "black";
    if (countdownTime <= 10)
        color = "red";
    countdownLabel->setStyleSheet((QString("color:%1").arg(color)));
    countdownLabel->setText(tr("剩余时间: %1").arg(countdownTime));
    // countdownLabel->show();
}

void LocalMainWindow::provideHints(SurakartaPosition pos) {
    auto hints = game->rule_manager_->GetAllLegalTarget(pos);
    std::vector<SurakartaPosition> hintVector = *hints;
    emit sendHints(hintVector);
}

void LocalMainWindow::showEndDialog() {
    // chessBoard->close();
    countdownTimer->stop();
    std::shared_ptr<SurakartaGameInfo> info = game->GetGameInfo();
    LocalEndDialog *enddialog = new LocalEndDialog(this);
    // QString winnerColor = (game.game_info_->winner_ == PieceColor::BLACK) ? "Black" : "White";
    QString winnerColor;
    if (game->game_info_->winner_ == PieceColor::BLACK)
        winnerColor = "获胜者：黑方";
    else if (game->game_info_->winner_ == PieceColor::WHITE)
        winnerColor = "获胜者：白方获胜";
    else if (game->game_info_->winner_ == PieceColor::NONE)
        winnerColor = "平局";

    QString message = QString("<div style='text-align: center;'>"
                              "<p><span style='font-size:12pt; color:red;'>GAME OVER!</span></p>"
                              "<p><span style='font-size:10pt; color:blue;'>%1</span>.</p>"
                              "<p><span style='font-size:10pt; color:blue;'>进行轮数：%2</span>.</p>"
                              "<p><span style='font-size:10pt; color:green;'>最后捕获轮：%3</span>.</p>"
                              // "<p><span style='font-size:10pt; color:orange;'>结束原因：%4</span>.</p>"
                              "</div>")
                          .arg(winnerColor)
                          .arg(info->num_round_)
                          .arg(info->last_captured_round_);
    // .arg(endReasonToString(info->end_reason_)) // 假设有一个endReasonToString函数将结束原因转换为字符串
    // .arg(info->max_no_capture_round_);

    enddialog->setText(message);
    connect(enddialog, &LocalEndDialog::restartGame, this, &LocalMainWindow::restartGame);
    connect(enddialog, &LocalEndDialog::backToStart, this, &LocalMainWindow::backToStartDialog);
    enddialog->exec();
}

void LocalMainWindow::restartGame() {
    delete chessBoard;
    Initialize();
}

void LocalMainWindow::giveUp() {
    game->game_info_->end_reason_ = SurakartaEndReason::RESIGN;
    // game->game_info_->winner_ = PieceColor::NONE;
    game->game_info_->winner_ = ReverseColor(game->game_info_->current_player_);
    buttonGiveUp->hide();
    showEndDialog();
}

LocalMainWindow::~LocalMainWindow() {
    // delete ui;
}

void LocalMainWindow::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    // 绘制棋盘网格
    painter.setPen(Qt::black);
    for (int i = arcNum; i < rawNum - arcNum - 1; ++i) {
        for (int j = arcNum; j < rawNum - arcNum - 1; ++j) {
            QLineF line1((i + 1) * gridSize, (j + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum), (j + 1) * gridSize);
            QLineF line2((i + 1) * gridSize, (j + 1) * gridSize, (i + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum));
            painter.drawLine(line1);
            painter.drawLine(line2);
        }
    }
    for (int i = 0; i < arcNum; i++)
        painter.drawArc(gridSize*(arcNum-i), gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 0, 16*270);
    for (int i = 0; i < arcNum; i++)
        painter.drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 16*270, 16*270);
    for (int i = 0; i < arcNum; i++)
        painter.drawArc(gridSize*(arcNum-i), (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*90, 16*270);
    for (int i = 0; i < arcNum; i++)
        painter.drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*180, 16*270);
}
