#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
// 超时时间
int TIME = 10;
int Lefttime = TIME + 2;
extern QStringList arg;
bool istart = 0;
int MainWindow::cnt = 0;
bool fi{1};


QString GetInfo(NetworkData data);


void MainWindow::Rmarkdown(NetworkData data)
{
    currentDateTime = QDateTime::currentDateTime();
    ui->game_text->append(currentDateTime.toString() +" server recieve : " + GetInfo(data));
    //clientfile->open(QIODevice::Append); // 打开文件以追加模式写入

    QTextStream clientout(clientfile);
    clientout <<  currentDateTime.toString() +" server recieve : " + GetInfo(data) + "\n";
}

void MainWindow::Smarkdown(NetworkData data)
{
    currentDateTime = QDateTime::currentDateTime();
    ui->game_text->append(currentDateTime.toString() +" server send : " + GetInfo(data));
    //clientfile->open(QIODevice::Append); // 打开文件以追加模式写入
    QTextStream clientout(clientfile);
    clientout <<  currentDateTime.toString() +" server send : " + GetInfo(data)+ "\n";
}

QString GetInfo(NetworkData data)
{

    QString rt = "OP : ";
    switch(data.op)
    {
    case OPCODE::READY_OP:
        rt += " READY_OP ";
        break;
    case OPCODE::MOVE_OP:
        rt += " MOVE_OP ";
        break;

    case OPCODE::GIVEUP_OP:
        rt += " GIVEUP_OP ";
        break;
    case OPCODE::LEAVE_OP:
        rt += " LEAVE_OP ";
        break;
    case OPCODE::END_OP:
        rt += "END_OP";
        break;
    default:
        rt += " ";
        break;

    }
    rt += (" | " + data.data1+ " | " + data.data2 + " | " + data.data3);
    return rt;
}

SurakartaMove MainWindow::backmove(NetworkData data)
{
    SurakartaMove rt;

    rt.from.x = data.data1[0].unicode() - 64 -1;
    rt.to.x   = data.data2[0].unicode() - 64 -1;
    rt.from.y = data.data1[1].digitValue() - 1;
    rt.to.y   = data.data2[1].digitValue() - 1;

    rt.player = game->game_info_->current_player_;

    qDebug() << "player :" << ((rt.player == PieceColor::BLACK) ? "BLACK" : "WHITE");
    return rt;
}
// 判断用户名是否合法
bool isValidUsername(const QString& username) {
    QRegularExpression regex("^[a-zA-Z0-9_]*$");
    return regex.match(username).hasMatch();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , color{"BLACK"}
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    server = new NetworkServer(this);
    ui->cli1_info->setReadOnly(true);
    ui->cli2_info->setReadOnly(true);
    // ui->stop->setDisabled(true);
    ui->game_text->setReadOnly(true);
    //timer = new QTimer(this);
    game = new SurakartaGame;
    user1 = new UserInfo;
    user2 = new UserInfo;

    movefile = new QFile("./record/server/Team_04.txt");
    clientfile = new QFile("info.txt");

    if (!movefile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing";
    }
    if (!clientfile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing";
    }

    QTextStream moveout(movefile);
    QTextStream clientout(clientfile);
    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,this,[=](){
        Lefttime--;
        qDebug() << "LeftTime" << Lefttime;
        if(Lefttime <= 0) // 超时
        {
            istart = 0;
            timer->stop();
            //  白棋的回合  超时   0 黑棋胜利
            if(game->game_info_->current_player_ == PieceColor::WHITE)
            {
                server->send(user1->client,NetworkData(OPCODE::END_OP,"","5","0"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,"","5","0"));
                Smarkdown(NetworkData(OPCODE::END_OP,"","5","0"));
            }
            else
            {
                server->send(user1->client,NetworkData(OPCODE::END_OP,"","5","1"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,"","5","1"));
                Smarkdown(NetworkData(OPCODE::END_OP,"","5","1"));
            }

        }
    });

    connect(server,&NetworkServer::receive,this,[=](QTcpSocket* client, NetworkData data){

        // 处理准备信号
        Rmarkdown(data);
        if(data.op == OPCODE::READY_OP)
        {
            ready(client,data);
        }
        // 处理离开信号
        else if(data.op == OPCODE::LEAVE_OP)
        {
            leave_op(client,data);
        }

        // 处理移动信号

        else if(data.op == OPCODE::MOVE_OP)
        {

            move_op(client,data);
        }

        else if(data.op == OPCODE::GIVEUP_OP)
        {
            QString endcolor;
            QString rightcolor = (game->game_info_->current_player_ == PieceColor::BLACK) ? "BLACK" :"WHITE";

            if(client == user1->client) endcolor = user1->color;
            else endcolor = user2->color;

            if(endcolor != rightcolor ) return;  // 当前请求的人不是行棋者忽略请求

            // 由黑棋发起拒绝请求  那么获胜的是白棋
            if(endcolor == "BLACK") endcolor = "1";
            else endcolor = "0";

            server->send(user1->client,NetworkData(OPCODE::END_OP,"","4",endcolor));
            server->send(user2->client,NetworkData(OPCODE::END_OP,"","4",endcolor));
            game->game_info_->end_reason_ = SurakartaEndReason::RESIGN;
            Smarkdown(NetworkData(OPCODE::END_OP,"","4",endcolor));
        }

    });

    if(arg.size() == 2){
    ui->port_line->setText(arg[1]);
    on_listen_button_clicked();

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listen_button_clicked()
{
    if (ui->listen_button->text() == "开始监听") {
        // 获取端口
        ui->listen_button->setText("停止监听");
        QString port = ui->port_line->text();
        // 有端口
        if(port != "")
        {
            server->listen(QHostAddress::Any,port.toUShort());
            // ui->listen_button->setDisabled(true);
            // ui->stop->setDisabled(false);
            ui->port_line->setReadOnly(true);
        }
    }
    else {
        ui->listen_button->setText("开始监听");
        server->close();
        user1->clean();
        user2->clean();
        ui->cli1_info->setText("");
        ui->cli2_info->setText("");
        // ui->listen_button->setDisabled(false);
        // ui->stop->setDisabled(true);
        cnt = 0;
        color = "BLACK";
        ui->game_text->setText("");
        ui->port_line->setReadOnly(false);
        ui->TIMELIMIT->setDisabled(false);
    }
}


// void MainWindow::on_stop_clicked()
// {
//     server->close();
//     user1->clean();
//     user2->clean();
//     ui->cli1_info->setText("");
//     ui->cli2_info->setText("");
//     ui->listen_button->setDisabled(false);
//     ui->stop->setDisabled(true);
//     cnt = 0;
//     color = "BLACK";
//     ui->game_text->setText("");
//     ui->port_line->setReadOnly(false);
//     ui->TIMELIMIT->setDisabled(false);
// }

void MainWindow::ready(QTcpSocket *client, NetworkData data)
{

    if(cnt == 0)  //  表示没有人连接
    {
        // 用户名合法时

        if(isValidUsername(data.data1))
        {
            // 无人连接默认为第一个用户
            user1->client = client;
            user1->name = data.data1;
            user1->room = data.data3;
            if(data.data2 == "") user1->color = "BLACK"; // 当第一个人未指定颜色时
            else user1->color = data.data2;

            cnt = 1; // 记录有一个人了

            ui->cli1_info->setText("| " +user1->name + " | " + user1->color + " | " + data.data3);

        }

        // 返回拒绝信息
        else
        {
            server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
            Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
        }
    }
    else if(cnt == 1) // 表示已经有一个
    {


        if(user1->client == client || user2->client == client ) // 表示是两个相同的客户端
        {
            // 给客户端发出同一客户端的信号
            server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
            Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
            return;
        }

        else if((data.data2 != "") && (data.data2 == user1->color ||  data.data2 == user2->color) )  // 表示执棋同色
        {
            // 给客户端发出reject的信号
            server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
            Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
            return;
        }
        else
        {
            if(user1->client == nullptr)  // 表示要给第一个user填信息
            {
                if(isValidUsername(data.data1))
                {
                    user1->client = client;
                    user1->name = data.data1;
                    user1->room = data.data3;
                    user1->color =  (user2->color == "BLACK") ? "WHITE" : "BLACK"; // 记录cilent的颜色

                    cnt = 2;

                    ui->cli1_info->setText("| " +user1->name + " | " + user1->color + " | " + data.data3);

                    // 超时时间设置
                    server->send(user1->client,NetworkData(OPCODE::SETTIME_OP,QString::number(ui->spinBox->value()),"",""));
                    server->send(user2->client,NetworkData(OPCODE::SETTIME_OP,QString::number(ui->spinBox->value()),"",""));

                    // 返回客户端已经准备就绪，游戏开始
                    server->send(user1->client,NetworkData(OPCODE::READY_OP,user2->name,user1->color,user1->room));
                    server->send(user2->client,NetworkData(OPCODE::READY_OP,user1->name,user2->color,user2->room));


                    Smarkdown(NetworkData(OPCODE::READY_OP,user1->name,user2->color,user1->room));
                    Smarkdown(NetworkData(OPCODE::READY_OP,user2->name,user1->color,user2->room));

                    game->StartGame();
                    istart = 1;
                    // 连接倒计时
                    timer->start(1000);

                    ui->TIMELIMIT->setDisabled(true);

                }

                // 返回拒绝信息
                else{
                    server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
                    Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
                }
            }
            else   // 给第二个填信息
            {
                if(isValidUsername(data.data1))
                {
                    user2->client = client;
                    user2->name = data.data1;
                    user2->room = data.data3;
                    qDebug() << user1->color;
                    user2->color =  (user1->color == "BLACK") ? "WHITE" : "BLACK"; // 记录cilent的颜色
                    qDebug() << "给第二个填信息";
                    cnt = 2;

                    ui->cli2_info->setText("| " +user2->name + " | " + user2->color+ " | " + data.data3);

                    // 超时时间设置
                    server->send(user1->client,NetworkData(OPCODE::SETTIME_OP,QString::number(ui->spinBox->value()),"",""));
                    server->send(user2->client,NetworkData(OPCODE::SETTIME_OP,QString::number(ui->spinBox->value()),"",""));

                    // 返回客户端已经准备就绪，游戏开始
                    server->send(user1->client,NetworkData(OPCODE::READY_OP,user2->name,user1->color,user1->room));
                    server->send(user2->client,NetworkData(OPCODE::READY_OP,user1->name,user2->color,user2->room));


                    Smarkdown(NetworkData(OPCODE::READY_OP,user1->name,user2->color,user1->room));
                    Smarkdown(NetworkData(OPCODE::READY_OP,user2->name,user1->color,user2->room));

                    game->StartGame();
                    istart = 1;

                    // 游戏开始，开始计时

                    timer->start(1000);

                    ui->TIMELIMIT->setDisabled(true);

                }

                // 返回拒绝信息
                else{
                    server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
                    Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
                }
            }
        }
    }
    else // 表示已经满了
    {
        // 传一个 已经满的信号
        server->send(client,NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
        Smarkdown(NetworkData(OPCODE::REJECT_OP,data.data1,"",""));
        return;
    }
}


void MainWindow::leave_op(QTcpSocket *client, NetworkData data)
{
    // 先判断游戏是否进行

    // if()  // 在进行时要做的额外操作
    // {

    // }

    // 清除cilent操作
    Q_UNUSED(data);

    if(istart  && !game->IsEnd()) // 表示游戏在进行
    {
        timer->stop();
        Lefttime = TIME + 2;
        qDebug() << Lefttime;
        game->game_info_->end_reason_ = SurakartaEndReason::RESIGN;
        QString endcolor;
        //
        if(client == user2->client)
        {
            // 发起离开请求的人的颜色为黑色    则胜者为 1 白色
            endcolor = (user2->color == "BLACK" ? "1" : "0");

            server->send(user1->client,NetworkData(OPCODE::END_OP,"","4",endcolor));
            server->send(user2->client,NetworkData(OPCODE::END_OP,"","4",endcolor));

            user2->clean();
            ui->cli2_info->setText("");

            Smarkdown(NetworkData(OPCODE::END_OP,"","4",endcolor));

        }
        else if(client == user1->client)
        {
           endcolor = (user1->color == "BLACK" ? "1" : "0");

           server->send(user1->client,NetworkData(OPCODE::END_OP,"","4",endcolor));
           server->send(user2->client,NetworkData(OPCODE::END_OP,"","4",endcolor));
           user1->clean();
           ui->cli1_info->setText("");

            Smarkdown(NetworkData(OPCODE::END_OP,"","4",endcolor));
        }
        cnt--;
    }

    else
    {
        timer->stop();
        Lefttime = TIME + 2;

        if(client == user1->client)
        {

            user1->clean();
            ui->cli1_info->setText("");
            cnt--;

        }
        else if(client == user2->client)
        {

            user2->clean();
            ui->cli2_info->setText("");
            cnt--;

        }

    }

}

void MainWindow::move_op(QTcpSocket *client, NetworkData data)
{
    timer->stop();
    Lefttime = TIME + 2;
    // 转发行棋    判断游戏是否结束   结束则转发endop
    SurakartaMove mv = backmove(data);

    //再转发给客户端        感觉有点病？
    server->send(user1->client,data);
    server->send(user2->client,data);
    // 开始计时等待下一次
    timer->start(1000);
    Smarkdown(data);
    //记录行棋
    QString info;
    if(client == user1->client)
         info = user1->name + " | " + data.data1 + " -> " + data.data2 ;
    else if (client == user2->client)
         info = user2->name + " | " + data.data1 + " -> " + data.data2 ;
    QTextStream out(movefile);
    if(fi)
    {
        fi = false;
        out << data.data1 + '-'  + data.data2;
    }
    else out << " "<< data.data1 + '-'  + data.data2;

    ui->game_text->append(info);

    switch(game->rule_manager_->JudgeMove(mv))
    {
    case SurakartaIllegalMoveReason::NOT_PLAYER_TURN:
        move_reason = "4";
        break;
    case SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE:
        move_reason = "1";
        break;
    case SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE:
        move_reason = "2";
        break;
    case SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE:
        move_reason = "8";
        break;
    case SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE:
        move_reason = "9";
        break;
    case SurakartaIllegalMoveReason::NOT_PLAYER_PIECE:
        move_reason = "7";
        break;
    case SurakartaIllegalMoveReason::NOT_PIECE:
        move_reason = "6";
        break;
    default:
        move_reason = "";
    }

    game->Move(mv);

    if(game->IsEnd())  // 是否结束
    {
        // 这个时候 一方吃完子了
        timer->stop();
        Lefttime = TIME + 2;
        istart = 0;
        if(game->game_info_->end_reason_ == SurakartaEndReason::CHECKMATE )
        {
            if(game->game_info_->current_player_ == PieceColor::BLACK)
            {
                server->send(user1->client,NetworkData(OPCODE::END_OP,"","2","0"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,"","2","0"));
                Smarkdown(NetworkData(OPCODE::END_OP,"","2","0"));
            }
            else
            {
                server->send(user1->client,NetworkData(OPCODE::END_OP,"","2","1"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,"","2","1"));
                Smarkdown(NetworkData(OPCODE::END_OP,"","2","1"));
            }
            return;
        }

        else if(game->game_info_->end_reason_ == SurakartaEndReason::ILLIGAL_MOVE)
        {
            if(game->game_info_->current_player_ == PieceColor::WHITE) // data3 指的是什么?
            {

                server->send(user1->client,NetworkData(OPCODE::END_OP,move_reason,"6","0"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,move_reason,"6","0"));
                Smarkdown(NetworkData(OPCODE::END_OP,"","6","0"));

            }
            else
            {
                server->send(user1->client,NetworkData(OPCODE::END_OP,move_reason,"6","1"));
                server->send(user2->client,NetworkData(OPCODE::END_OP,move_reason,"6","1"));
                Smarkdown(NetworkData(OPCODE::END_OP,move_reason,"6","1"));
            }
            return;
        }
    }
}

void MainWindow::on_TIMELIMIT_clicked()
{
    TIME = ui->spinBox->value();
}
