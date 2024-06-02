#ifndef ONLINE_END_DIALOG_H
#define ONLINE_END_DIALOG_H

#include "info_reason.h"
#include <QDialog>

namespace Ui {
class Dialog;
}

class OnlineEndDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineEndDialog(QWidget *parent = nullptr);
    void SetInfo(SurakartaEndReason rea,QString color,QString round);
    ~OnlineEndDialog();

private slots:
    void on_one_more_game_clicked();

    void on_return_start_clicked();

signals:
    void restart_game();
    void return_start();

private:
    Ui::Dialog *ui;
};

#endif // ONLINE_END_DIALOG_H
