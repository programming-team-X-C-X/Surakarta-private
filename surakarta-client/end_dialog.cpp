#include "end_dialog.h"

endDialog::endDialog(QWidget *parent) : QDialog(parent), label(new QLabel(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    label->setTextFormat(Qt::RichText); // 设置文本格式为富文本
    layout->addWidget(label);

    QPushButton *buttonOK = new QPushButton(tr("Ok"), this);
    QPushButton *buttonRE = new QPushButton(tr("再来一局"),this);
    QPushButton *buttonBA = new QPushButton(tr("回到主界面"), this);
    layout->addWidget(buttonOK);
    layout->addWidget(buttonRE);
    layout->addWidget(buttonBA);
    connect(buttonOK, &QPushButton::clicked, this, &endDialog::accept);
    connect(buttonRE, &QPushButton::clicked, this, &endDialog::onREButtonClicked);
    connect(buttonBA, &QPushButton::clicked, this, &endDialog::onBAButtonClicked);
}

void endDialog::onREButtonClicked() {
    emit restartGame();//
    accept();//close dialog
}

void endDialog::onBAButtonClicked() {
    emit backToStart();
    accept();
}

void endDialog::setText(const QString &text) {
    label->setAlignment(Qt::AlignCenter);
    label->setText(text);
}
