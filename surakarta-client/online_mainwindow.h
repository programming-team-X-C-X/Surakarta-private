#ifndef ONLINE_MAINWINDOW_H
#define ONLINE_MAINWINDOW_H

#include <QMainWindow>
#include "networksocket.h"
#include "networkdata.h"
#include "online_view.h"
#include <QFile>
#include <QTimer>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;
}
QT_END_NAMESPACE

class OnlineMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    OnlineMainWindow(QWidget *parent = nullptr);
    ~OnlineMainWindow();
    SurakartaMove backMove(NetworkData data);
private slots:
    // void on_connect_button_clicked();

    // 连接信息
    void on_readyButton_clicked();

    // void on_disconnect_button_clicked();

    // 处理服务器信息
    void rec_ready(NetworkData& data);
    void rec_move(NetworkData& data);
    void rec_end(NetworkData& data);
    void rec_rej();

    // 记录行棋信息
    void mark_move(NetworkData& data);

    // 处理再来一局
    //void restart_game();
signals:
    void judge_end();
    void back_start();
private:
    Ui::MainWindow *ui;
    GameView* Game;
    NetworkSocket* socket;
    QFile * movefile;
    QTimer* timer;
    QTimer *aiuser;
    bool isfirst;
};
#endif // ONLINE_MAINWINDOW_H
