#include "start_dialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include "History_MainWindow.h"
#include "settings_dialog.h"

StartDialog::StartDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("苏拉卡尔塔 --programming-team-X-C-X --Powered by Qt 6.8.0"));
    setStyleSheet("StartDialog {"
                  "background-image: url(:/new/prefix2/img/board.jpg);"
                  "background-repeat: no-repeat;"
                  "background-position: center;"
                  "}");

    QPixmap background(":/new/prefix2/img/board.jpg");
    QSize imageSize = background.size();
    resize(imageSize);

    vLayout = new QVBoxLayout();//no this
    //  QHBoxLayout和QVBoxLayout不应该直接用this（指向窗口的指针）初始化，因为这样会导致它们试图设置窗口的布局两次
    hLayout = new QHBoxLayout();
    singlePlayerButton = new QPushButton(tr("单人游戏"), this);
    multiPlayerButton = new QPushButton(tr("双人游戏"), this);
    computerGameButton = new QPushButton(tr("电脑对战"), this);
    localButton = new QPushButton(tr("本地游戏"), this);
    backButton = new QPushButton(tr("返回"), this);
    onlineButton = new QPushButton(tr("联机游戏"), this);
    settingsButton = new QPushButton(tr("游戏设置"), this);
    historyButton = new QPushButton(tr("历史记录"), this);
    singlePlayerButton->hide();
    multiPlayerButton->hide();
    computerGameButton->hide();
    backButton->hide();
    settingsButton->hide();

    // singlePlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // multiPlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // computerGameButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    localButton->setFixedSize(100, 30);
    onlineButton->setFixedSize(100, 30);
    singlePlayerButton->setFixedSize(100, 30); // 将按钮的大小设置为100x30
    multiPlayerButton->setFixedSize(100, 30);
    computerGameButton->setFixedSize(100, 30);
    settingsButton->setFixedSize(100, 30);
    backButton->setFixedSize(100, 30);
    historyButton->setFixedSize(100,30);

    // vLayout->setAlignment(Qt::AlignBottom);
    vLayout->addWidget(settingsButton);
    vLayout->addStretch();
    vLayout->addWidget(singlePlayerButton);
    vLayout->addWidget(multiPlayerButton);
    vLayout->addWidget(computerGameButton);
    vLayout->addWidget(backButton);
    vLayout->addWidget(localButton);
    vLayout->addWidget(onlineButton);
    vLayout->addWidget(historyButton);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    this->setLayout(hLayout);// 将水平布局设置为窗口的主布局

    connect(localButton, &QPushButton::clicked, this, &StartDialog::showLocalGameOptions);
    connect(onlineButton, &QPushButton::clicked, this, &StartDialog::showOnlineGameOptions);
    connect(singlePlayerButton, &QPushButton::clicked, this, &StartDialog::startSinglePlayerGame);
    connect(multiPlayerButton, &QPushButton::clicked, this, &StartDialog::startMultiPlayerGame);
    connect(computerGameButton, &QPushButton::clicked, this, &StartDialog::startComputerGame);
    connect(settingsButton, &QPushButton::clicked, this, &StartDialog::openSettingsDialog);
    connect(backButton, &QPushButton::clicked, this, &StartDialog::backToModeSelect);
    connect(historyButton, &QPushButton::clicked, this, &StartDialog::showHistory);
}

void StartDialog::showLocalGameOptions() {
    localButton->hide();
    onlineButton->hide();

    singlePlayerButton->show();
    multiPlayerButton->show();
    computerGameButton->show();
    settingsButton->show();
    backButton->show();
    // vLayout->addWidget(singlePlayerButton);
    // vLayout->addWidget(multiPlayerButton);
    // vLayout->addWidget(computerGameButton);

    // 不需要重新设置布局，因为是在现有的布局中添加按钮
}

void StartDialog::showOnlineGameOptions() {
    emit onlineGameSelected(); // 发出联机游戏选项被选中的信号
    // 或者执行其他必要的联机设置代码
}

void StartDialog::selectSinglePlayerGame() {
    mode = SinglePlayer;
    accept(); // 关闭对话框并返回 QDialog::Accepted
}

void StartDialog::selectMultiPlayerGame() {
    mode = MultiPlayer;
    accept();
}

void StartDialog::selectComputerGame() {
    mode = ComputerGame;
    this->accept();
    // this->close();
}

void StartDialog::openSettingsDialog() {
    SettingsDialog settingsDialog(this); // 使用 this 作为 parent 以确保设置面板居中显示
    settingsDialog.exec(); // 显示设置面板
}

void StartDialog::backToModeSelect() {
    localButton->show();
    onlineButton->show();

    singlePlayerButton->hide();
    multiPlayerButton->hide();
    computerGameButton->hide();
    settingsButton->hide();
    backButton->hide();
}

void StartDialog::showHistory()
{
    History_MainWindow* w = new History_MainWindow();
    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
}
