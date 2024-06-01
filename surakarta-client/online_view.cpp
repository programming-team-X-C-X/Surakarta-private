#include "online_view.h"
#include "ui_online_view.h"
#include <QDebug>
#include "online_end_dialog.h"
#include "AI_task.h"
#include <QDebug>

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
    connect(this, &GameView::sendDengerousHints, chessBoard, &ChessBoardWidget::receiveDangerousHints);
    connect(agentMine.get(), &SurakartaAgentMine::updateProgress, this, &GameView::updateProgress);
    game.StartGame();
    show();



    // 移动任务对象到新线程
    // deadCalcutor->moveToThread(DEADCalculateThread);

    // // 连接信号和槽
    // connect(DEADCalculateThread, &QThread::started, deadCalcutor, &DEADCalculator::doWork);
    // connect(deadCalcutor, &DEADCalculator::resultReady, this, &GameView::onDEADCalculateComputed);

    // // 在对象销毁前输出调试信息
    // connect(deadCalcutor, &QObject::destroyed, this, [](QObject* obj){
    //     qDebug() << "DEADCalculator object" << obj << "is being deleted" << Qt::endl;
    // });

    // // 清理
    // connect(deadCalcutor, &DEADCalculator::resultReady, deadCalcutor, &DEADCalculator::deleteLater);
    // connect(DEADCalculateThread, &QThread::finished, DEADCalculateThread, &QThread::deleteLater);

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

void GameView::startDEADCalculatorThread(SurakartaPosition fromPos, std::vector<SurakartaPosition> toPos) {
    // 创建线程和任务对象
    if (DEADCalculateThread && DEADCalculateThread->isRunning()) {
        deadCalcutor->requestStop(); // 请求停止工作对象
        DEADCalculateThread->quit(); // 请求线程退出
        DEADCalculateThread->wait(); // 等待线程结束
        deadCalcutor->deleteLater(); // 安排删除工作对象
        DEADCalculateThread->deleteLater(); // 安排删除线程对象
        qDebug() << "delete" << Qt::endl;
    }

    DEADCalculateThread = new QThread();
    // AITask* aiTask = new AITask(agentMine);
    /*DEADCalculator* */deadCalcutor = new DEADCalculator(game, fromPos, toPos, DEPTH);

    // // 移动任务对象到新线程
    deadCalcutor->moveToThread(DEADCalculateThread);

    // 断开之前的连接（如果有）
    disconnect(DEADCalculateThread, &QThread::started, deadCalcutor, &DEADCalculator::doWork);
    disconnect(deadCalcutor, &DEADCalculator::resultReady, this, &GameView::onDEADCalculateComputed);
    // 连接信号和槽
    connect(DEADCalculateThread, &QThread::started, deadCalcutor, &DEADCalculator::doWork);
    connect(deadCalcutor, &DEADCalculator::resultReady, this, &GameView::onDEADCalculateComputed);

    // 在对象销毁前输出调试信息
    connect(deadCalcutor, &QObject::destroyed, this, [](QObject* obj){
        qDebug() << "DEADCalculator object" << obj << "is being deleted" << Qt::endl;
    });

    // 清理
    connect(deadCalcutor, &DEADCalculator::resultReady, deadCalcutor, &DEADCalculator::deleteLater);
    connect(DEADCalculateThread, &QThread::finished, DEADCalculateThread, &QThread::deleteLater);

    // 启动线程
    DEADCalculateThread->start();
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
        aiThread->deleteLater();
        aiThread = nullptr;
    }
}

void GameView::onDEADCalculateComputed(std::vector<SurakartaPosition> pos) {
    // 用 AI 计算的结果来更新游戏

    qDebug() << "finish" << Qt::endl;
    if (pos.empty()) {
        qDebug() << "nothing" << Qt::endl;
    } else {
        emit sendDengerousHints(pos);
        qDebug() << "Positions:";
        for (const auto& position : pos) {
            // 假设SurakartaPosition有一个输出友好的方式，例如重载了<<运算符
            qDebug() << position.x << position.y << Qt::endl;
        }
    }
    // 停止和清理 AI 线程
    if(DEADCalculateThread) {
        DEADCalculateThread->quit();
        DEADCalculateThread->wait();
        DEADCalculateThread->deleteLater();
        DEADCalculateThread = nullptr;
        qDebug() << "delete" << Qt::endl;
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
    // startDEADCalculatorThread(pos, captureHintVector);
    emit sendCaptureHints(captureHintVector);

    auto NONcaptureHints = game.rule_manager_->GetAllLegalNONCaptureTarget(pos);
    std::vector<SurakartaPosition> NONcaptureHintVector = *NONcaptureHints;
    // startDEADCalculatorThread(pos, NONcaptureHintVector);
    emit sendNONCaptureHints(NONcaptureHintVector);

    std::vector<SurakartaPosition> combinedHintVector = *captureHints;
    combinedHintVector.insert(combinedHintVector.end(), NONcaptureHints->begin(), NONcaptureHints->end());

    // 只启动一个线程来处理合并后的提示
    if (!DEADCalculateThread || DEADCalculateThread->isFinished())
    startDEADCalculatorThread(pos, combinedHintVector);
    else qDebug() << "deadthread is running" << Qt::endl;
}

void GameView::on_giveup_button_clicked()
{
    // 点击就发出信号
    emit Resign();
}

void GameView::Move(SurakartaPosition from, SurakartaPosition to)
{
    // 如果DEADCalculator线程正在运行，尝试结束它
    if (DEADCalculateThread && DEADCalculateThread->isRunning()) {
        // 请求线程退出
        // DEADCalculateThread->requestInterruption();
        // 设置退出标志
        // dynamic_cast<DEADCalculator*>(DEADCalculateThread->workerObject())->shouldExit = true
        // 获取线程的工作对象并安全地请求线程停止
        // auto worker = dynamic_cast<DEADCalculator*>(DEADCalculateThread->workerObject());
        // if (worker) {
            // worker->requestStop();
        // }

        deadCalcutor->requestStop();
        // DEADCalculateThread->wait(); // 等待线程结束

        // delete deadCalcutor;
        // deadCalcutor = nullptr;

        // delete DEADCalculateThread;
        // DEADCalculateThread = nullptr;


        // 等待线程响应退出请求并结束
        // if (!DEADCalculateThread->wait(500)) { // 等待500毫秒
            // 如果线程没有在指定时间内结束，强制终止它
            // DEADCalculateThread->terminate();
            // DEADCalculateThread->wait(); // 等待线程完全结束
        // }

        // 清理线程对象
        // DEADCalculateThread->deleteLater();
        // DEADCalculateThread = nullptr;
    }

    // 创建并发出移动请求
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
