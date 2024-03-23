#ifndef START_DIALOG_H
#define START_DIALOG_H

#include "qboxlayout.h"
#include "qpushbutton.h"
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>

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

    // QPixmap background;
    // QSize imageSize;

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

    QPushButton *singlePlayerButton;
    QPushButton *multiPlayerButton;
    QPushButton *computerGameButton;
    QPushButton *settingsButton;

    // QSpinBox *box;
    // QPushButton *saveButton;


public slots:
    void selectSinglePlayerGame();
    void selectMultiPlayerGame();
    void selectComputerGame();
    void openSettingsDialog();

    // void saveSettings();


};

#endif // START_DIALOG_H
