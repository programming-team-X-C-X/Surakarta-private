#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow_pve.h"
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
extern QString name;
extern bool PLAYER_COLOR;
extern QStringList arg;


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

SurakartaMove MainWindow::backmove(NetworkData data)
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    socket = new NetworkSocket(new QTcpSocket(this),this);


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


    });



    if(arg.size() == 3)
    {
        ui->ip_line->setText(arg[1]);
        ui->port_line->setText(arg[2]);
        QString ip = ui->ip_line->text();
        QString port = ui->port_line->text();
        if( ip != "" && port != "")  // 输入了有效的ip和port
        {
            socket->hello(ip,port.toUShort());
            this->socket->base()->waitForConnected(2000);
            socket->send(NetworkData(OPCODE::READY_OP,"","",""));

        }
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rec_ready(NetworkData& data)
{
    // 表示双方准备就绪   游戏开始

    // 设置本用户的执棋颜色
    PLAYER_COLOR = (data.data2 == "BLACK") ? 1 : 0;

    mycolor = (data.data2 == "BLACK") ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE;
    // 开始打开游戏界面  关闭自己的界面
    Game = new game_view();

    isfirst = true;
    // 能关闭吗 ?
    // this->close();

    // 行棋记录
    movefile = new QFile("./record/client/Team_04.txt");
    if (!movefile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing";
    }

    // 连接请求移动 和 认输
    connect(Game,&game_view::AskMove,this,[=](SurakartaMove move){
        // 向服务端发送这个信息
        // 先转换move
        socket->send(NetworkData(OPCODE::MOVE_OP,FormatPos(move.from),FormatPos(move.to),""));
    });



    if(!Game->game.IsEnd() && Game->game.game_info_->current_player_ == mycolor)
            Game->computerMove();

}

void MainWindow::rec_move(NetworkData& data)
{
    // 得到移动的信息
    SurakartaMove move = backmove(data);

    // 进行移动
    Game->game.Move(move);

    // 记录行棋
    mark_move(data);

    if(!Game->game.IsEnd() && Game->game.game_info_->current_player_ == mycolor)
        Game->computerMove();


}

void MainWindow::rec_end(NetworkData& data)
{
    QTextStream out(movefile);

    if(data.data2 == "1") out << "S";

    else if(data.data2 == "2") out << "C";

    else if(data.data2 == "4") out << "R";

    else if(data.data2 == "5") out << "T";

    else if(data.data2 == "6") out << "I";



    socket->send(NetworkData(OPCODE::LEAVE_OP,"","","")); // 发出离开信号
    socket->bye();
    QCoreApplication::quit();
}

void MainWindow::mark_move(NetworkData& data)
{
    QTextStream out(movefile);

    // fi 记录是否为第一次写入   ---->   为保证最后结尾处没有 " "
    out << data.data1 + '-'  + data.data2 << " ";

}

// void MainWindow::on_disconnect_button_clicked()
// {
//     socket->send(NetworkData(OPCODE::LEAVE_OP, "", "", ""));
//     socket->bye();
//     ui->connect_button->setDisabled(false);
//     ui->disconnect_button->setDisabled(true);
//     ui->readyButton->setDisabled(false);
// }

