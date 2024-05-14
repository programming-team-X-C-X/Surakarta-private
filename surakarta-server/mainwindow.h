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
    static int cnt;
    QString move_reason;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private slots:
    void on_listen_button_clicked();

    void on_stop_clicked();

    void ready(QTcpSocket* client, NetworkData data);
    void leave_op(QTcpSocket* client, NetworkData data);
    // void end(QTcpSocket* client, NetworkData data);
    // void timeout(QTcpSocket* client, NetworkData data);
     void move_op(QTcpSocket* client, NetworkData data);
    // void resign_op(QTcpSocket* client, NetworkData data);


    SurakartaMove  backmove(NetworkData data);
    void Rmarkdown(NetworkData data);
    void Smarkdown(NetworkData data);
    void on_TIMELIMIT_clicked();

private:
    QString color;
    Ui::MainWindow *ui;
    // 1个server
    NetworkServer * server;
    // 2个user
    UserInfo* user1;
    UserInfo* user2;
    SurakartaGame * game;
    QTimer *timer;
    QDateTime currentDateTime;

    QFile * movefile;
    QFile * clientfile;



};
#endif // MAINWINDOW_H
