#include "online_mainwindow.h"
#include "ui_online_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
extern QString NAME;
extern bool PLAYER_COLOR;

OnlineMainWindow::OnlineMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle(tr("苏拉卡尔塔棋 --programming-team-X-C-X --Powered by Qt 6.8.0"));
    ui->setupUi(this);
    socket = new NetworkSocket(new QTcpSocket(this),this);

    // 服务端主动终止收不到信息 ?
    connect(socket->base(),&QTcpSocket::disconnected,this,[=](){
        ui->readyButton->setDisabled(false);
        ui->readyButton->setText("准备");
    });

    connect(socket->base(),&QTcpSocket::connected,this,[=](){
        ui->readyButton->setDisabled(true);
        ui->readyButton->setText("等待玩家...");
    });
    // 处理接收到的信息
    connect(socket, &NetworkSocket::receive, this, [=](NetworkData data) {
        switch(data.op) {
        case OPCODE::READY_OP:
            rec_ready(data);
            break;

        case OPCODE::MOVE_OP:
            rec_move(data);
            break;

        case OPCODE::END_OP:
            rec_end(data);
            break;

        case OPCODE::SETTIME_OP:
            TIME_LIMIT = data.data1.toUInt();
            break;

        case OPCODE::REJECT_OP:
            rec_rej();
            break;
            // 可以添加更多的case处理其他操作码

        default:
            // 可以处理未知的操作码
            break;
        }
    });
}

QString FormatPos(const SurakartaPosition pos)
{
    QString rt = "";
    switch(pos.x)
    {
    case 0:
        rt += 'A';
        break;
    case 1:
        rt += 'B';
        break;
    case 2:
        rt += 'C';
        break;
    case 3:
        rt += 'D';
        break;
    case 4:
        rt += 'E';
        break;
    case 5:
        rt += 'F';
        break;
    }

    rt += QString::number(pos.y + 1);
    return rt;
}

SurakartaEndReason backReason(NetworkData data)
{
    if(data.data2 == "1") return SurakartaEndReason::STALEMATE;

    else if(data.data2 == "2") return SurakartaEndReason::CHECKMATE;

    else if(data.data2 == "4") return SurakartaEndReason::RESIGN;

    else if(data.data2 == "5") return SurakartaEndReason::TIMEOUT;

    else if(data.data2 == "6") return SurakartaEndReason::ILLIGAL_MOVE;

    else return SurakartaEndReason::NONE;
}

QString backColor(NetworkData data)
{
    if(data.data3 == "0") return "BLACK";
    else if(data.data3 == "1") return "WHITE";

    else return "PieceColor::NONE";
}

SurakartaMove OnlineMainWindow::backMove(NetworkData data)
{
    SurakartaMove rt;

    rt.from.x = data.data1[0].unicode() - 64 -1;
    rt.to.x   = data.data2[0].unicode() - 64 -1;
    rt.from.y = data.data1[1].digitValue() - 1;
    rt.to.y   = data.data2[1].digitValue() - 1;

    rt.player = Game->game.GetGameInfo()->current_player_;

    qDebug() << "player :" << ((rt.player == PieceColor::BLACK) ? "BLACK" : "WHITE");
    return rt;
}

OnlineMainWindow::~OnlineMainWindow()
{
    delete ui;
}

void OnlineMainWindow::on_readyButton_clicked()
{
    // 请求连接
    QString ip = ui->ip_line->text();
    QString port = ui->port_line->text();
    if( ip != "" && port != "")  // 输入了有效的ip和port
    {
        socket->hello(ip,port.toUShort());
        qDebug() << "CONNECTED";
    }

    // 发送信息
    QString name = ui->ready_name->text();
    QString color = ui->choose_color->currentText();
    QString room = ui->choose_room->currentText();

    this->socket->base()->waitForConnected(2000);

    socket->send(NetworkData(OPCODE::READY_OP,name,color,room));

}

void OnlineMainWindow::rec_rej(){

    QMessageBox::warning(0,"REJECTED", "服务器拒绝连接!请检查你的设置信息!");

    ui->readyButton->setDisabled(false);
    ui->readyButton->setText("准备");
}

void OnlineMainWindow::rec_ready(NetworkData& data)
{
    // 初始化历史记录相关
    store_filename = "../history/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_game_history.txt";
    store_gameinfo = "";

    // 表示双方准备就绪   游戏开始
    // 设置本用户的执棋颜色
    PLAYER_COLOR = (data.data2 == "BLACK") ? 1 : 0;
    MY_COLOR = (data.data2 == "BLACK") ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE;
    
    NAME = ui->ready_name->text();
    // 开始打开游戏界面  关闭自己的界面
    this->hide();
    Game = new GameView();
    Game->setAttribute(Qt::WA_DeleteOnClose);
    isfirst = true;

    // 设置 按钮字样
    ui->readyButton->setText("游戏中...");

    // 行棋记录
    movefile = new QFile("./record/client/Team_04.txt");

    if (!movefile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing";
    }

    // 倒计时 | 不一定会用到
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,Game,&GameView::update_time); // 更新计时器
    timer->start(1000);

    // 连接请求移动 和 认输
    connect(Game,&GameView::AskMove,this,[=](SurakartaMove move){
        // 向服务端发送这个信息
        Game->game.Move(move);
        Game->chessBoard->movePiece(move);
        // 先转换move
        socket->send(NetworkData(OPCODE::MOVE_OP,FormatPos(move.from),FormatPos(move.to),""));
    });

    connect(Game, &GameView::AskComputerMove, this, [=](){
        if(IsAi) Game->computerMove();
    });

    connect(Game->chessBoard,&ChessBoardWidget::animationFinished,this,[=](){
        GAME_ROUND++;
        Game->update_gameinfo();
        Game->left_time = TIME_LIMIT;
        RIGHT_COLOR = !RIGHT_COLOR;
        if(IsAi) Game->computerMove();
    });

    connect(Game,&GameView::Resign,this,[=](){
        socket->send(NetworkData(OPCODE::GIVEUP_OP,"","","")); // 发出认输信号
    });

    // 连接 再来一局
    connect(Game,&GameView::restart_game,this,[=](){
        // 初始化游戏   打开这个界面

        Game->close();
        //socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号
        IsAi = 1;
        this->show();
        // 再发出准备信号
        ui->readyButton->setText("准备");
        ui->readyButton->setDisabled(false);
        //on_readyButton_clicked();

    });


    connect(Game,&GameView::return_start,this,&OnlineMainWindow::back_start);

    connect(Game,&GameView::return_start,this,[=](){
        Game->close();
        //socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号
    });


    Game->update_gameinfo();
    if(IsAi) Game->computerMove();
}

void OnlineMainWindow::rec_move(NetworkData& data)
{
    // 得到移动的信息
    SurakartaMove move = backMove(data);

    // 进行移动
    Game->game.Move(move);
    Game->chessBoard->movePiece(move);

    // 记录行棋
    mark_move(data);
    //  动画结束后 更新本地的玩家信息等

}

void OnlineMainWindow::rec_end(NetworkData& data)
{
    // 保存游戏信息
    Save_game();

    // 展示结束画面
    if(data.data2 == "1" || data.data2 == "2") // 有移动的最后一步
        // 最后一步倒计时不会走
        connect(Game->chessBoard,&ChessBoardWidget::animationFinished,this,[=](){
            timer->stop();
            Game->endShow(backReason(data),backColor(data),QString::number(GAME_ROUND));
        });
    else {
        timer->stop();
        Game->endShow(backReason(data),backColor(data),QString::number(GAME_ROUND));
    }
    // 直接断开连接 ?

    // 进入结束的逻辑
    socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号
    socket->bye();
}

void OnlineMainWindow::mark_move(NetworkData& data)
{
    QTextStream out(movefile);

    // fi 记录是否为第一次写入   ---->   为保证最后结尾处没有 " "
    if(isfirst)
    {
        isfirst = false;
        out << data.data1 + '-'  + data.data2;
    }
    else out << " "<< data.data1 + '-'  + data.data2;
    store_gameinfo += data.data1 + "-" + data.data2 + " ";
}

void OnlineMainWindow::Save_game()
{
    QFile file(store_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << store_gameinfo;
        file.close();
    }
}

