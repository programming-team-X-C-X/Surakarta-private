#ifndef END_DIALOG_H
#define END_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class endDialog :public QDialog
{
    Q_OBJECT
public:
    explicit endDialog(QWidget *parent = nullptr);
    void setText(const QString &text);

// public slots:
    // void reStart();

signals:
    void restartGame(); // 添加一个信号
    void backToStart();

private slots:
    void onREButtonClicked(); // 处理 OK 按钮点击的槽函数
    void onBAButtonClicked();

private:
    QLabel *label;
};

#endif // END_DIALOG_H
