#include <QApplication>
#include <QPointer>
#include "info_game.h"
#include "start_dialog.h"
#include "game_mainwindow.h"
#include "mainwindow.h"

// 这个函数接受游戏模式，并创建 Main Game Window
void startGame(/*QApplication &app, */StartDialog *startDialog, QPointer<GameMainWindow>gameMainWindow, SurakartaGameMode mode) {
    if (!startDialog) return; // 检查 startDialog 是否仍然存在
    startDialog->close();



    gameMainWindow = new GameMainWindow(mode);
    gameMainWindow->setAttribute(Qt::WA_DeleteOnClose);
    gameMainWindow->show();

    QObject::connect(gameMainWindow, &GameMainWindow::backToStartDialog, gameMainWindow, [startDialog, gameMainWindow]() {
        gameMainWindow->close();
        startDialog->show();
    });

}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StartDialog *startDialog = new StartDialog();
    QPointer<GameMainWindow> gameMainWindow /*= nullptr*/;
    startDialog->show();


    // 连接开始对话框的信号到一个具体的游戏模式启动函数
    QObject::connect(startDialog, &StartDialog::startComputerGame,/* &app,*/ [startDialog, gameMainWindow]() {
        startGame(startDialog, gameMainWindow, SurakartaGameMode::EVE);
    });
    QObject::connect(startDialog, &StartDialog::startMultiPlayerGame, [startDialog, gameMainWindow]() {
        startGame(startDialog, gameMainWindow, SurakartaGameMode::PVP);
    });
    QObject::connect(startDialog, &StartDialog::startSinglePlayerGame, [startDialog, gameMainWindow]() {
        startGame(startDialog, gameMainWindow, SurakartaGameMode::PVE);
    });
    QObject::connect(startDialog, &StartDialog::onlineGameSelected, [startDialog, gameMainWindow]() {
        if (!startDialog) return; // 检查 startDialog 是否仍然存在
        startDialog->close();
        MainWindow *w = new MainWindow();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

        QObject::connect(w,&MainWindow::back_start,[=](){
            w->close();
            startDialog->show();
        });
    });
    return app.exec();
}
