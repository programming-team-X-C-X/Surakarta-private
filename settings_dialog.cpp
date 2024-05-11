#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dialog_settings)
{
    ui->setupUi(this);
    connect(ui->saveBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(ui->saveBox, &QDialogButtonBox::rejected, this, &SettingsDialog::close);

}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::saveSettings() {
    BOARD_SIZE = ui->boardSizeBox->value();
    TIME_LIMIT = ui->timeLimitBox->value();
    PLAYER_COLOR = ui->blackButton->isChecked();
    PIECE_SPEED = ui->speedBox->value();
    accept();
}
