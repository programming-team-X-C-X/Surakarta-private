#include "mainwindow_pve.h"
#include "end_dialog.h"
#include <QDebug>

MainWindow_PVE::MainWindow_PVE(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowTitle(tr("surakarta"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);

    buttonClose = new QPushButton("close", this);
    buttonStart = new QPushButton("start", this);
    buttonBack = new QPushButton("back", this);
    buttonStart->setFixedSize(100, 30);
    buttonBack->setFixedSize(100, 30);
    buttonClose->setFixedSize(100, 30);

    QVBoxLayout *vlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();

    vlayout->addStretch(30);
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
    connect(buttonClose, &QPushButton::clicked, this, &MainWindow_PVE::close);
    connect(buttonStart, &QPushButton::clicked, this,  &MainWindow_PVE::Initialize);
    connect(buttonStart, &QPushButton::clicked, buttonStart, &QPushButton::deleteLater);
    connect(buttonBack, &QPushButton::clicked, this, &MainWindow_PVE::backToStartDialog);

}

void MainWindow_PVE::Initialize() {
    chessBoard = new ChessBoardWidget();
    buttonGiveUp = new QPushButton("认输", this);
    buttonGiveUp->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*4/6, 100, 30);

    setCentralWidget(chessBoard);
    // chessBoard->setGeometry(0, 0, WINDOW_SIZE, WINDOW_SIZE);
    connect(buttonGiveUp, &QPushButton::clicked, this, &MainWindow_PVE::giveUp);
    connect(chessBoard, &ChessBoardWidget::playerMove, this, &MainWindow_PVE::playerMove);
    connect(chessBoard, &ChessBoardWidget::animationFinished, this, &MainWindow_PVE::startComputerMove);

    countdownLabel = new QLabel(this);
    QFont labelFont = countdownLabel->font();
    labelFont.setPointSize(50);
    countdownLabel->setText(tr("Remaining Time: %1").arg(countdownTime));

    computerMoveTimer = new QTimer(this);
    computerMoveTimer->setSingleShot(true);
    connect(computerMoveTimer, &QTimer::timeout, this, &MainWindow_PVE::computerMove);

    countdownTimer = new QTimer(this);
    countdownTimer->setInterval(1000); // 设置定时器每秒触发一次
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow_PVE::updateCountdown);
    // 创建一个新的布局
    QVBoxLayout *vlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();
    // 将按钮添加到布局中
    buttonClose->setFixedSize(100, 30);
    buttonGiveUp->setFixedSize(100, 30);
    // buttonStart->setFixedSize(100, 30);
    vlayout->addStretch(10);
    vlayout->addWidget(countdownLabel);
    vlayout->addStretch(20);
    vlayout->addWidget(buttonGiveUp);
    vlayout->addStretch(1);
    vlayout->addWidget(buttonClose);
    vlayout->addStretch(1);
    vlayout->addStretch(4);
    // 将棋盘添加到布局中
    chessBoard->setFixedSize(WINDOW_SIZE, WINDOW_SIZE);
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

    game.StartGame();
    chessBoard->board = game.board_;
    chessBoard->setMode(ChessBoardWidget::PieceMode);
    countdownTime = TIME_LIMIT;
    countdownTimer->start();
}

// void MainWindow_PVE::updateGame() {
//     chessBoard->setMode(ChessBoardWidget::PieceMode);
// }

void MainWindow_PVE::computerMove() {
    SurakartaMove move = agentMine->CalculateMove();
    game.Move(move);
    // handlePlayerMove(move.from, move.to);
    chessBoard->movePiece(move);
    // updateGame();
    if (game.IsEnd() && game.GetGameInfo()->end_reason_ != SurakartaEndReason::ILLIGAL_MOVE)
        showEndDialog();
    // countdownTimer->start(TIME_LIMIT);
}

void MainWindow_PVE::playerMove(SurakartaPosition from, SurakartaPosition to) {
    if (game.IsEnd()) return;
    // SurakartaMove move = handlePlayerMove(from, to);
    SurakartaMove move(from, to, game.GetGameInfo()->current_player_);

    game.Move(move);
    chessBoard->movePiece(move);
    // updateGame();
    if (game.IsEnd() && game.GetGameInfo()->end_reason_ != SurakartaEndReason::ILLIGAL_MOVE)
        showEndDialog();
}

void MainWindow_PVE::startComputerMove() {
    if (game.game_info_->current_player_ == SurakartaPlayer::WHITE) {
        // countdownTimer->stop();
        countdownTime = TIME_LIMIT;
        computerMoveTimer->start(computerMoveDelay);
    }
}

void MainWindow_PVE::updateCountdown() {
    countdownTime--;
    updateCountdownDisplay();
    if (countdownTime <= 0) {
        // countdownTimer->stop();
        game.game_info_->end_reason_ = SurakartaEndReason::TIMEOUT;
        showEndDialog();
    }
}

void MainWindow_PVE::updateCountdownDisplay() {
    QString color = "black";
    if (countdownTime <= 10) {
        color = "red";
    }
    countdownLabel->setStyleSheet((QString("color:%1").arg(color)));
    countdownLabel->setText(tr("Remaining Time: %1").arg(countdownTime));
    // countdownLabel->show();
}

void MainWindow_PVE::showEndDialog() {
    // chessBoard->close();
    countdownTimer->stop();
    std::shared_ptr<SurakartaGameInfo> info = game.GetGameInfo();
    endDialog *enddialog = new endDialog(this);
    // QString winnerColor = (game.game_info_->winner_ == PieceColor::BLACK) ? "Black" : "White";
    QString winnerColor;
    if (game.game_info_->winner_ == PieceColor::BLACK) {
        winnerColor = "Black Player";
    } else if (game.game_info_->winner_ == PieceColor::WHITE) {
        winnerColor = "White Player";
    } else {
        winnerColor = "Unknown";
    }

    QString message = QString("<div style='text-align: center;'>"
                              "<p><span style='font-size:12pt; color:red;'>GAME OVER!</span></p>"
                              "<p><span style='font-size:10pt; color:blue;'>获胜者：%1</span>.</p>"
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
    connect(enddialog, &endDialog::restartGame, this, &MainWindow_PVE::restartGame);
    connect(enddialog, &endDialog::backToStart, this, &MainWindow_PVE::backToStartDialog);
    enddialog->exec();
}

void MainWindow_PVE::restartGame() {
    delete chessBoard;
    Initialize();
}

void MainWindow_PVE::giveUp() {
    game.game_info_->end_reason_ = SurakartaEndReason::RESIGN;
    showEndDialog();
}

MainWindow_PVE::~MainWindow_PVE() {
    // delete ui;
}

void MainWindow_PVE::paintEvent(QPaintEvent */*event*/) {
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
