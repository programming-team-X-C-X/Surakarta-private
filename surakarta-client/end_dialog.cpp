#include "end_dialog.h"

EndDialog::EndDialog(QWidget *parent) : QDialog(parent), label(new QLabel(this)) {
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
    connect(buttonOK, &QPushButton::clicked, this, &EndDialog::accept);
    connect(buttonRE, &QPushButton::clicked, this, &EndDialog::onREButtonClicked);
    connect(buttonBA, &QPushButton::clicked, this, &EndDialog::onBAButtonClicked);
}

void EndDialog::onREButtonClicked() {
    emit restartGame();//
    accept();//close dialog
}

void EndDialog::onBAButtonClicked() {
    emit backToStart();
    accept();
}

void EndDialog::setText(const QString &text) {
    label->setAlignment(Qt::AlignCenter);
    label->setText(text);
}
