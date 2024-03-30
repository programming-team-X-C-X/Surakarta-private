#include <QApplication>
#include "start_dialog.h"
#include "mainwindow_eve.h"
#include "mainwindow_pvp.h"
#include "mainwindow_pve.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StartDialog *startDialog = new StartDialog();
    startDialog->show();
    QObject::connect(startDialog, &StartDialog::startComputerGame, &app, [startDialog]() {
        startDialog->close();
        // 在堆上创建 MainWindow_PVE 并设置父对象
        MainWindow_EVE* mainWindow_EVE = new MainWindow_EVE();
        mainWindow_EVE->setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时会删除对象
        mainWindow_EVE->show();
    });
    QObject::connect(startDialog, &StartDialog::startMultiPlayerGame, &app, [startDialog]() {
        startDialog->close();
        // MainWindow_PVP mainWindow_PVP;
        MainWindow_PVP* mainWindow_PVP = new MainWindow_PVP();
        mainWindow_PVP->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow_PVP->show();
    });
    QObject::connect(startDialog, &StartDialog::startSinglePlayerGame, &app, [startDialog]() {
        startDialog->close();
        // MainWindow_PVE mainWindow_PVE;
        MainWindow_PVE* mainWindow_PVE = new MainWindow_PVE();
        mainWindow_PVE->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow_PVE->show();

        QObject::connect(mainWindow_PVE, &MainWindow_PVE::backToStartDialog, mainWindow_PVE, [startDialog, mainWindow_PVE](){
            mainWindow_PVE->close();
            startDialog->show();
        });
    });


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
