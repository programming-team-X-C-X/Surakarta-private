#include "mainwindow_pve.h"
#include "settings.h"
#include "end_dialog.h"

MainWindow_PVE::MainWindow_PVE(QWidget *parent)
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
    connect(chessBoard, &ChessBoardWidget::playerMove, this, &MainWindow_PVE::playerMove);

    computerMoveTimer = new QTimer(this);
    computerMoveTimer->setSingleShot(true);
    connect(computerMoveTimer, &QTimer::timeout, this, &MainWindow_PVE::computerMove);

}

void MainWindow_PVE::Initialize() {
    // SurakartaGame game;
    game.StartGame();
    game.chessBoardWidghtInit(game.chessboardwight_, game.board_);
    chessBoard->board = game.board_;

    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();
}

void MainWindow_PVE::updateGame() {
    chessBoard->board = game.board_;
    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();
    if (game.IsEnd() && game.GetGameInfo()->end_reason_ != SurakartaEndReason::ILLIGAL_MOVE) {
        showEndDialog();
    }
}

void MainWindow_PVE::playerMove(SurakartaPosition from, SurakartaPosition to) {
    SurakartaMove move = handlePlayerMove(from, to);
    game.Move(move);
    if (game.game_info_->current_player_ == PieceColor::WHITE){
        updateGame();
        computerMoveTimer->start(computerMoveDelay);
    }
}

void MainWindow_PVE::computerMove() {
    SurakartaMove move = agentMine->CalculateMove();
    game.Move(move);
    updateGame();
}


SurakartaMove MainWindow_PVE::handlePlayerMove(SurakartaPosition from, SurakartaPosition to) {

    SurakartaRuleManager rule_manager_(game.GetBoard(), game.GetGameInfo());
    std::shared_ptr<SurakartaBoard> board = game.GetBoard();
    PieceColor color = (*board)[from.x][from.y]->GetColor();
    SurakartaMove move(from, to, color);
    SurakartaIllegalMoveReason reason = rule_manager_.JudgeMove(move);
    if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE || reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE)
        return move;
    else {
        // to = from;
        return SurakartaMove(from, from, color);
    }
}

void MainWindow_PVE::showEndDialog() {
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
    connect(enddialog, &endDialog::restartGame, this, &MainWindow_PVE::Initialize);
    enddialog->exec();
}

MainWindow_PVE::~MainWindow_PVE()
{
    // delete ui;
}
