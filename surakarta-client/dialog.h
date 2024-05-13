#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "info_game.h"
#include "settings.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void SetInfo(SurakartaEndReason rea,QString color,QString round);
    ~Dialog();

private slots:
    void on_one_more_game_clicked();

    void on_return_start_clicked();

signals:
    void restart_game();
    void return_start();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
