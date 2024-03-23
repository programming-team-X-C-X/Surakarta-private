#include "start_dialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include "settings.h"
#include "settings_dialog.h"

StartDialog::StartDialog(QWidget *parent) : QDialog(parent) {

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
    settingsButton = new QPushButton(tr("settings"), this);
    // singlePlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // multiPlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // computerGameButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    singlePlayerButton->setFixedSize(100, 30); // 将按钮的大小设置为100x30
    multiPlayerButton->setFixedSize(100, 30);
    computerGameButton->setFixedSize(100, 30);
    settingsButton->setFixedSize(100, 30);

    // box = new QSpinBox(this);
    // box->setRange(4, 20);
    // box->setValue(BOARD_SIZE);

    // saveButton = new QPushButton(tr("save"), this);
    // vLayout->addWidget(box);
    // vLayout->addWidget(saveButton);

    // vLayout->setAlignment(Qt::AlignBottom);
    vLayout->addWidget(settingsButton);
    vLayout->addStretch();
    vLayout->addWidget(singlePlayerButton);
    vLayout->addWidget(multiPlayerButton);
    vLayout->addWidget(computerGameButton);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    this->setLayout(hLayout);// 将水平布局设置为窗口的主布局

    connect(singlePlayerButton, &QPushButton::clicked, this, &StartDialog::startSinglePlayerGame);
    connect(multiPlayerButton, &QPushButton::clicked, this, &StartDialog::startMultiPlayerGame);
    connect(computerGameButton, &QPushButton::clicked, this, &StartDialog::startComputerGame);
    connect(settingsButton, &QPushButton::clicked, this, &StartDialog::openSettingsDialog);

    // connect(saveButton, &QPushButton::clicked, this, &StartDialog::saveSettings);

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

// void StartDialog::saveSettings() {
//     BOARD_SIZE = box->value();
// }
