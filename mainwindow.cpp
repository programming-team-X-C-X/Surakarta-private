#include "mainwindow.h"
#include "settings.h"

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
    }
}

MainWindow::~MainWindow()
{
    // delete ui;
}
