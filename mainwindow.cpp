#include "mainwindow.h"
#include "settings.h"
#include "enddialog.h"

MainWindow::MainWindow(QWidget *parent)
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
}

void MainWindow::Initialize() {
    // SurakartaGame game;
    game.StartGame();
    game.chessBoardWidghtInit(game.chessboardwight_, game.board_);
    chessBoard->board = game.board_;

    chessBoard->update();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(SLEEP_TIME);
}

void MainWindow::updateGame() {
    // 处理游戏逻辑
    SurakartaMove move;
    if(game.game_info_->current_player_ == PieceColor::BLACK)
        move = agent->CalculateMove();
    else
        move = agentMine->CalculateMove();

    game.Move(move);

    chessBoard->board = game.board_;
    chessBoard->update();

    if (game.IsEnd()) {
        // 如果游戏结束，停止定时器
        QTimer *timer = qobject_cast<QTimer*>(sender());
        if (timer) {
            timer->stop();
        }

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

        QString message = QString(
                              "<div style='text-align: center;'>"
                              "<p><span style='font-size:12pt; color:red;'>GAME OVER!</span>.</p>"
                              "<p><span style='font-size:10pt; color:blue;'>进行轮数：%1</span>.</p>"
                              "<p><span style='font-size:10pt; color:green;'>获胜者：%2</span>.</p>")
                              .arg(game.game_info_->num_round_)
                              .arg( winnerColor);
        enddialog->setText(message);
        connect(enddialog, &endDialog::restartGame, this, &MainWindow::Initialize);
        enddialog->exec();
    }
}

MainWindow::~MainWindow()
{
    // delete ui;
}
