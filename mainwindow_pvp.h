#ifndef MAINWINDOW_PVP_H
#define MAINWINDOW_PVP_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include "my_game.h"
#include "my_game.h"
#include "chess_board_widght.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// 在绘制事件中调用此函数来绘制实心点

class MainWindow_PVP : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow_PVP(QWidget *parent = nullptr);
    ~MainWindow_PVP();
    SurakartaMove handlePlayerMove(SurakartaPosition from, SurakartaPosition to);
    ChessBoardWidget *chessBoard;
    SurakartaGame game;

public slots:
    void Initialize();
    void updateGame(SurakartaPosition from, SurakartaPosition to);


private:
    QPushButton *buttonClose;
    QPushButton *buttonStart;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
};

#endif // MAINWINDOW_PVP_H
