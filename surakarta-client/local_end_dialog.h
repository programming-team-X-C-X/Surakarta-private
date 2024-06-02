#ifndef LOCAL_END_DIALOG_H
#define LOCAL_END_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class LocalEndDialog :public QDialog
{
    Q_OBJECT
public:
    explicit LocalEndDialog(QWidget *parent = nullptr);
    void setText(const QString &text);

signals:
    void restartGame();
    void backToStart();

private slots:
    void onREButtonClicked();
    void onBAButtonClicked();

private:
    QLabel *label;
};

#endif // LOCAL_END_DIALOG_H
