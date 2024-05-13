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

#endif // LOCAL_END_DIALOG_H
