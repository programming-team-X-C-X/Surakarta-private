#include "online_mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
extern QString name;
extern bool PLAYER_COLOR;

// timer 什么时候动
// 收到 readyop 启动timer
// 收到行棋请求 --------->  重置时间 继续减值
// 收到endop   --------->  timer stop


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

SurakartaMove OnlineMainWindow::backmove(NetworkData data)
{
    SurakartaMove rt;

    rt.from.x = data.data1[0].unicode() - 64 -1;
    rt.to.x   = data.data2[0].unicode() - 64 -1;
    rt.from.y = data.data1[1].digitValue() - 1;
    rt.to.y   = data.data2[1].digitValue() - 1;

    rt.player = Game->game.game_info_->current_player_;

    qDebug() << "player :" << ((rt.player == PieceColor::BLACK) ? "BLACK" : "WHITE");
    return rt;
}

OnlineMainWindow::OnlineMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    socket = new NetworkSocket(new QTcpSocket(this),this);
    aiuser = new QTimer(this);
    aiuser->start(5000);




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
    connect(socket,&NetworkSocket::receive,this,[=](NetworkData data){
        if(data.op == OPCODE::READY_OP)
        {
            // 处理 ready op
            rec_ready(data);
        }

        else if(data.op == OPCODE::MOVE_OP)
        {
            rec_move(data);
        }

        else if(data.op == OPCODE::END_OP)
        {
            rec_end(data);
        }

        else if(data.op == OPCODE::SETTIME_OP){
            // 处理时间设置
            TIME_LIMIT = data.data1.toUInt();
        }


    });






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

void OnlineMainWindow::rec_ready(NetworkData& data)
{
    // 表示双方准备就绪   游戏开始

    // 设置本用户的执棋颜色
    PLAYER_COLOR = (data.data2 == "BLACK") ? 1 : 0;
    mycolor = (data.data2 == "BLACK") ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE;

    name = ui->ready_name->text();
    // 开始打开游戏界面  关闭自己的界面
    Game = new GameView();
    Game->setAttribute(Qt::WA_DeleteOnClose);
    isfirst = true;
    // 能关闭吗 ?
    // this->close();

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

    connect(aiuser,&QTimer::timeout,this,[=](){
        if(IsAi) Game->computerMove();
    });

    // 连接请求移动 和 认输
    connect(Game,&GameView::AskMove,this,[=](SurakartaMove move){
        // 向服务端发送这个信息
        // 先转换move
        socket->send(NetworkData(OPCODE::MOVE_OP,FormatPos(move.from),FormatPos(move.to),""));
    });

    connect(Game,&GameView::Resign,this,[=](){
        socket->send(NetworkData(OPCODE::GIVEUP_OP,"","","")); // 发出认输信号
    });

    // 连接 再来一局
    connect(Game,&GameView::restart_game,this,[=](){
        // 初始化游戏   打开这个界面

        Game->close();
        //socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号

        // 再发出准备信号
        ui->readyButton->setText("准备");
        ui->readyButton->setDisabled(false);
        //on_readyButton_clicked();

    });

    connect(Game->chessBoard,&ChessBoardWidget::animationFinished,this,[=](){
        gameround++;
        Game->update_gameinfo();
        Game->left_time = TIME_LIMIT;
        RIGHT_COLOR = !RIGHT_COLOR;
    });


    connect(Game,&GameView::return_start,this,&OnlineMainWindow::back_start);
    connect(Game,&GameView::return_start,this,[=](){
        Game->close();
    });

    // Game->computerMove();
    Game->update_gameinfo();
}

void OnlineMainWindow::rec_move(NetworkData& data)
{
    // 得到移动的信息
    SurakartaMove move = backmove(data);

    // 进行移动
    Game->game.Move(move);
    Game->chessBoard->movePiece(move);

    // 记录行棋
    mark_move(data);


    //  动画结束后 更新本地的玩家信息等

}

void OnlineMainWindow::rec_end(NetworkData& data)
{
    // 计时器停
    timer->stop();

    // 展示结束画面  进入结束的逻辑
    socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号
    socket->bye();
    Game->endShow(backReason(data),backColor(data),QString::number(gameround));




    // 直接断开连接 ?
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

}


