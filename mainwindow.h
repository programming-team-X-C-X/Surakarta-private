#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "chessboardwidght.h"

// #define ANSI_CLEAR_SCREEN "\033[2J"
// #define ANSI_MOVE_TO_START "\033[H"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



// 在绘制事件中调用此函数来绘制实心点

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // std::shared_ptr<ChessBoardWidget> chessBoard;
    ChessBoardWidget *chessBoard;
    SurakartaGame game;
    std::shared_ptr<SurakartaAgentRandom> agent     = std::make_shared<SurakartaAgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    std::shared_ptr<SurakartaAgentMine>   agentMine = std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());


public slots:
    void Initialize();
    void updateGame();




private:
    // Ui::MainWindow *ui;
    QPushButton *buttonClose;
    QPushButton *buttonStart;
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
//     void startGame() {


//     }
//     void updateBoard() {

//     }
};



#endif // MAINWINDOW_H
