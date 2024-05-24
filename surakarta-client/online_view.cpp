#include "online_view.h"
#include "ui_online_view.h"
#include <QDebug>
#include "online_end_dialog.h"
#include "AI_task.h"

GameView::GameView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game_view)
    , aiThread(nullptr)
{
    // 初始化
    left_time = TIME_LIMIT;
    gameround = 1;
    RIGHT_COLOR = 1;
    IsAi = 1;

    ui->setupUi(this);
    setWindowTitle(tr("苏拉卡尔塔棋 --programming-team-X-C-X --Powered by Qt 6.8.0"));
    setFixedSize(WINDOW_SIZE*1.5, WINDOW_SIZE);

    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);

    ui->giveup_button->raise();
    ui->useAi->raise();

    connect(chessBoard, &ChessBoardWidget::playerMove, this, &GameView::Move);
    connect(chessBoard, &ChessBoardWidget::animationFinished, this, &GameView::AfterAnimationFinished);
    connect(chessBoard, &ChessBoardWidget::requestHints, this, &GameView::provideHints);
    connect(this, &GameView::sendCaptureHints, chessBoard, &ChessBoardWidget::receiveCaptureHints);
    connect(this, &GameView::sendNONCaptureHints, chessBoard, &ChessBoardWidget::receiveNONCaptureHints);
    connect(agentMine.get(), &SurakartaAgentMine::updateProgress, this, &GameView::updateProgress);
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

void GameView::updateProgress(int progress) {
    ui->AIprogressBar->setValue(progress);
}

void GameView::startAIThread() {
    // 创建线程和任务对象
    aiThread = new QThread();
    // AITask* aiTask = new AITask(agentMine);
    AITask* aiTask = new AITask(agentMine);

    // 移动任务对象到新线程
    aiTask->moveToThread(aiThread);

    // 连接信号和槽
    connect(aiThread, &QThread::started, aiTask, &AITask::doWork);
    connect(aiTask, &AITask::resultReady, this, &GameView::onAIMoveComputed);

    // 清理
    connect(aiTask, &AITask::resultReady, aiTask, &AITask::deleteLater);
    connect(aiThread, &QThread::finished, aiThread, &QThread::deleteLater);

    // 启动线程
    aiThread->start();
}

void GameView::computerMove() {
    if (!game.IsEnd() && PLAYER_COLOR == RIGHT_COLOR) {
        if (!aiThread || aiThread->isFinished()) {
            startAIThread(); // 启动或重新启动AI线程
        }
    }
}

void GameView::onAIMoveComputed(const SurakartaMove& move) {
    // 用 AI 计算的结果来更新游戏
    emit AskMove(move);
    // 停止和清理 AI 线程
    if(aiThread) {
        aiThread->quit();
        aiThread->wait();
        // delete aiThread;
        aiThread->deleteLater();
        aiThread = nullptr;
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
    // qDebug() << left_time ;
    ui->Ltime->setText( QString::number(left_time--));
    if(left_time < 0)
    {
        left_time = 0;
    }
}

void GameView::provideHints(SurakartaPosition pos) {
    auto captureHints = game.rule_manager_->GetAllLegalCaptureTarget(pos);
    std::vector<SurakartaPosition> captureHintVector = *captureHints;
    emit sendCaptureHints(captureHintVector);
    auto NONcaptureHints = game.rule_manager_->GetAllLegalNONCaptureTarget(pos);
    std::vector<SurakartaPosition> NONcaptureHintVector = *NONcaptureHints;
    emit sendNONCaptureHints(NONcaptureHintVector);
}

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

void GameView::AfterAnimationFinished() {
    emit AnimationFinished();
}

void GameView::on_useAi_clicked()
{
    if(ui->useAi->text() == "AI托管"){
        ui->useAi->setText("取消托管");
        IsAi = 1;
        emit AskComputerMove();
    }
    else {
        ui->useAi->setText("AI托管");
        IsAi = 0;
    }
}
