#include "mainwindow_eve.h"
#include "settings.h"
#include "end_dialog.h"

MainWindow_EVE::MainWindow_EVE(QWidget *parent)
    : QMainWindow(parent)
// , ui(new Ui::MainWindow)
{
    // ui->setupUi(this);
    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);
    setWindowTitle(tr("surakarta"));

    buttonClose = new QPushButton("close", this);
    buttonStart = new QPushButton("start", this);
    buttonClose->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    buttonStart->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*4.5/6, 100, 30);
    // vbox->addWidget(buttonClose);
    // vbox->addWidget(buttonStart);
    // this->setLayout(vbox);

    connect(buttonClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(buttonStart, SIGNAL(clicked(bool)), this, SLOT(Initialize()));
    // connect(chessBoard, &ChessBoardWidget::playerMove, this, &MainWindow_EVE::handlePlayerMove);
}

void MainWindow_EVE::Initialize() {
    // SurakartaGame game;
    game.StartGame();
    game.chessBoardWidghtInit(game.chessboardwight_, game.board_);
    chessBoard->board = game.board_;

    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow_EVE::updateGame);
    timer->start(SLEEP_TIME);
}

void MainWindow_EVE::updateGame() {
    // 处理游戏逻辑
    SurakartaMove move;
    if(game.game_info_->current_player_ == PieceColor::BLACK)
        move = agent->CalculateMove();
    else
        move = agentMine->CalculateMove();

    game.Move(move);

    chessBoard->board = game.board_;
    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();

    if (game.IsEnd()) {
        // 如果游戏结束，停止定时器
        QTimer *timer = qobject_cast<QTimer*>(sender());
        if (timer) {
            timer->stop();
        }
        showEndDialog();
    }
}

void MainWindow_EVE::showEndDialog() {
    //实现结局弹窗
    // game.game_info_->end_reason_
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

    enddialog->setText(message);
    connect(enddialog, &endDialog::restartGame, this, &MainWindow_EVE::Initialize);
    enddialog->exec();
}

MainWindow_EVE::~MainWindow_EVE()
{
    // delete ui;
}
