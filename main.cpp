#include <QApplication>
#include "start_dialog.h"
#include "mainwindow_eve.h"
#include "mainwindow_pvp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StartDialog startDialog;
    MainWindow_EVE mainWindow_EVE;
    MainWindow_PVP mainWindow_PVP;

    QObject::connect(&startDialog, &StartDialog::startComputerGame, &app, [&]() {
        startDialog.close();
        mainWindow_EVE.show();
    });
    QObject::connect(&startDialog, &StartDialog::startMultiPlayerGame, &app, [&]() {
        startDialog.close();
        mainWindow_PVP.show();
    });

    startDialog.show();

    // // 显示初始对话框，并等待用户做出选择
    // if (startDialog.exec() == QDialog::Accepted) {
    //     std::cout<<"hello"<<std::endl;
    //     // 根据用户选择的模式来执行不同的操作
    //     switch (startDialog.selectedMode()) {
    //     case StartDialog::SinglePlayer:
    //         // 启动单人游戏
    //         break;
    //     case StartDialog::MultiPlayer:
    //         // 启动双人游戏
    //         break;
    //     case StartDialog::ComputerGame:
    //         // 显示主窗口，开始电脑对战
    //         mainWindow.show();
    //         break;
    //     default:
    //         // 用户没有选择任何模式或者点击了取消
    //         break;
    //     }
    // }

    return app.exec();
}
