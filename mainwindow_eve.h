#ifndef MAINWINDOW_EVE_H
#define MAINWINDOW_EVE_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include "my_game.h"
#include "my_agent_mine.h"
#include "my_agent_random.h"
#include "my_game.h"
#include "chess_board_widght.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// 在绘制事件中调用此函数来绘制实心点

class MainWindow_EVE : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow_EVE(QWidget *parent = nullptr);
    ~MainWindow_EVE();
    void handlePlayerMove(SurakartaPosition from, SurakartaPosition to);
    ChessBoardWidget *chessBoard;
    SurakartaGame game;
    std::shared_ptr<SurakartaAgentRandom> agent     = std::make_shared<SurakartaAgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    std::shared_ptr<SurakartaAgentMine>   agentMine = std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    void showEndDialog();


public slots:
    void Initialize();
    void updateGame();

private:
    QPushButton *buttonClose;
    QPushButton *buttonStart;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
};


#endif // MAINWINDOW_EVE_H
