#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkdata.h"
#include "networkserver.h"
#include <QTcpSocket>
#include "surakarta_game.h"
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

//存储用户信息
class UserInfo
{
public:
    QString color; // 颜色
    QString name;  // 用户名
    QString room;  // 房间号
    QTcpSocket* client;
    UserInfo():color{},name{},client{nullptr}
    {}

    // 清除客户端的信息
    void clean()
    {
        color = "";
        name  = "";
        client = nullptr;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 当前连接的客户端的数量
    static int cnt;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private slots:
    // 点击监听后的槽函数
    void on_listen_button_clicked();

    // 收到客户端ready_op时的槽函数
    void ready(QTcpSocket* client, NetworkData data);

    // 收到客户端leave_op时的槽函数
    void leave_op(QTcpSocket* client, NetworkData data);

    // 收到客户端move_op时的槽函数
    void move_op(QTcpSocket* client, NetworkData data);

    // 将传入的 编码 转换为 move
    SurakartaMove  backmove(NetworkData data);

    // 记录网络信息
    void Rmarkdown(NetworkData data);
    void Smarkdown(NetworkData data);

    // 设置发给客户端的时间限制
    void on_TIMELIMIT_clicked();

private:
    Ui::MainWindow *ui;
    // 1个server
    NetworkServer * server;
    // 2个user
    UserInfo* user1;
    UserInfo* user2;

    SurakartaGame * game;
    QTimer *timer;
    QDateTime currentDateTime;

    // 记录网络日志的文件
    QFile * movefile;
    QFile * clientfile;



};
#endif // MAINWINDOW_H
