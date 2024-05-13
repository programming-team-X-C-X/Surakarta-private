#include "local_end_dialog.h"

LocalEndDialog::LocalEndDialog(QWidget *parent) : QDialog(parent), label(new QLabel(this)) {
    setWindowTitle(tr("苏拉卡尔塔棋"));
    QVBoxLayout *layout = new QVBoxLayout(this);
    label->setTextFormat(Qt::RichText); // 设置文本格式为富文本
    layout->addWidget(label);

    QPushButton *buttonOK = new QPushButton(tr("关闭窗口"), this);
    QPushButton *buttonRE = new QPushButton(tr("再来一局"),this);
    QPushButton *buttonBA = new QPushButton(tr("回到主界面"), this);
    layout->addWidget(buttonRE);
    layout->addWidget(buttonBA);
    layout->addWidget(buttonOK);
    connect(buttonOK, &QPushButton::clicked, this, &LocalEndDialog::accept);
    connect(buttonRE, &QPushButton::clicked, this, &LocalEndDialog::onREButtonClicked);
    connect(buttonBA, &QPushButton::clicked, this, &LocalEndDialog::onBAButtonClicked);
}

void LocalEndDialog::onREButtonClicked() {
    emit restartGame();//
    accept();//close dialog
}

void LocalEndDialog::onBAButtonClicked() {
    emit backToStart();
    accept();
}

void LocalEndDialog::setText(const QString &text) {
    label->setAlignment(Qt::AlignCenter);
    label->setText(text);
}
