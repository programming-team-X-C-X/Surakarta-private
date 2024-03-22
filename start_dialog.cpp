#include "start_dialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include "settings.h"

StartDialog::StartDialog(QWidget *parent) : QDialog(parent) {

    setStyleSheet("StartDialog {"
                  "background-image: url(:/new/prefix2/img/board.jpg);"
                  "background-repeat: no-repeat;"
                  "background-position: center;"
                  "}");

    QPixmap background(":/new/prefix2/img/board.jpg");
    QSize imageSize = background.size();
    resize(imageSize);

    QVBoxLayout *vLayout = new QVBoxLayout();//no this
    //  QHBoxLayout和QVBoxLayout不应该直接用this（指向窗口的指针）初始化，因为这样会导致它们试图设置窗口的布局两次
    QHBoxLayout *hLayout = new QHBoxLayout();

    QPushButton *singlePlayerButton = new QPushButton(tr("单人游戏"), this);
    QPushButton *multiPlayerButton = new QPushButton(tr("双人游戏"), this);
    QPushButton *computerGameButton = new QPushButton(tr("电脑对战"), this);
    // singlePlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // multiPlayerButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    // computerGameButton->setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*5/6, 100, 30);
    singlePlayerButton->setFixedSize(100, 30); // 将按钮的大小设置为100x30
    multiPlayerButton->setFixedSize(100, 30);
    computerGameButton->setFixedSize(100, 30);

    // QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // sizePolicy.setHorizontalStretch(1);
    // sizePolicy.setVerticalStretch(1);
    // singlePlayerButton->setSizePolicy(sizePolicy);
    // multiPlayerButton->setSizePolicy(sizePolicy);
    // computerGameButton->setSizePolicy(sizePolicy);

    // setGeometry(WINDOW_SIZE*7/6, WINDOW_SIZE*4.5/6, 100, 30);

    // vLayout->setAlignment(Qt::AlignBottom);
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
