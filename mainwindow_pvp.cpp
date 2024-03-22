#include "mainwindow_pvp.h"
#include "settings.h"
#include "end_dialog.h"
#include <sstream>

MainWindow_PVP::MainWindow_PVP(QWidget *parent)
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
    // connect(chessBoard, &ChessBoardWidget::playerMove, this, &MainWindow_PVP::handlePlayerMove);
    connect(chessBoard, &ChessBoardWidget::playerMove, this, &MainWindow_PVP::updateGame);
    // connect(chessBoard, &ChessBoardWidget::playerMove, this, SLOT(Initialize()));
}

void MainWindow_PVP::Initialize() {
    // SurakartaGame game;
    game.StartGame();
    game.chessBoardWidghtInit(game.chessboardwight_, game.board_);
    chessBoard->board = game.board_;

    // chessBoard->paintBoard();
    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();

    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &MainWindow_PVP::updateGame);
    // timer->start(SLEEP_TIME);
}

void MainWindow_PVP::updateGame(SurakartaPosition from, SurakartaPosition to) {
    // 处理游戏逻辑
    SurakartaMove move = handlePlayerMove(from, to);
    game.Move(move);

    chessBoard->board = game.board_;
    chessBoard->setMode(ChessBoardWidget::PieceMode);
    chessBoard->update();

    if (game.IsEnd() && game.GetGameInfo()->end_reason_ != SurakartaEndReason::ILLIGAL_MOVE) {
        // 如果游戏结束，停止定时器
        // QTimer *timer = qobject_cast<QTimer*>(sender());
        // if (timer) {
        //     timer->stop();
        // }

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
                                  "<p><span style='font-size:10pt; color:navy;'>最大无捕获回合数：%6</span>.</p></div>")
                              .arg(winnerColor)
                              .arg(info->num_round_)
                              .arg(info->last_captured_round_)
                              // .arg(endReasonToString(info->end_reason_)) // 假设有一个endReasonToString函数将结束原因转换为字符串
                              .arg(info->max_no_capture_round_);

        // std::stringstream ss;

        // // 使用重载的 operator<< 来输出 game_info
        // ss << *info;

        // // 从 stringstream 获取 string
        // std::string infoStr = ss.str();
        // // 将 std::string 转换为 QString
        // QString qInfoStr = QString::fromStdString(infoStr);

        // QString message = QString("<div style='text-align: center;'>"
        //                           "<p><span style='font-size:12pt; color:red;'>GAME OVER!</span></p>"
        //                           "<p><span style='font-size:10pt; color:blue;'>获胜者：%1</span>.</p>")
        //                       .arg(qInfoStr);

        // 显示或返回message
        enddialog->setText(message);
        connect(enddialog, &endDialog::restartGame, this, &MainWindow_PVP::Initialize);
        enddialog->exec();
    }
}

SurakartaMove MainWindow_PVP::handlePlayerMove(SurakartaPosition from, SurakartaPosition to) {

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

MainWindow_PVP::~MainWindow_PVP()
{
    // delete ui;
}
