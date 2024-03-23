#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <QDialog>

namespace Ui {
class dialog_settings;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

public slots:
    void saveSettings();


private:
    Ui::dialog_settings *ui;
};

#endif // DIALOG_SETTINGS_H
