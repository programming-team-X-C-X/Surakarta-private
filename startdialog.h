#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

class StartDialog : public QDialog {
    Q_OBJECT

public:
    enum GameMode {
        None,
        SinglePlayer,
        MultiPlayer,
        ComputerGame
    };

    explicit StartDialog(QWidget *parent = nullptr);
    GameMode selectedMode() const { return mode; }

signals:
    void startSinglePlayerGame();
    void startMultiPlayerGame();
    void startComputerGame();

private:
    GameMode mode = None; // 默认没有选择

public slots:
    void selectSinglePlayerGame();
    void selectMultiPlayerGame();
    void selectComputerGame();

};

#endif // STARTDIALOG_H
