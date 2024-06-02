#include "online_end_dialog.h"
#include "settings.h"
#include "ui_online_end_dialog.h"
extern bool PLAYER_COLOR;

OnlineEndDialog::OnlineEndDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

void OnlineEndDialog::SetInfo(SurakartaEndReason rea, QString color, QString round)
{
    ui->roundnum->setText(round);
    ui->winner->setText(color);
    QString your_color = (PLAYER_COLOR) ? "BLACK" : "WHITE";
    ui->UserName->setText(name);

    // 处理结束信息
    if(rea == SurakartaEndReason::CHECKMATE){

        if(your_color == color) ui->LoseWin->setText("恭喜你获得胜利!");
        else ui->LoseWin->setText("很遗憾你失败了!");
    }
    else if(rea == SurakartaEndReason::RESIGN)
    {
        if(your_color == color) ui->LoseWin->setText("对方认输,恭喜你获得胜利!");
        else ui->LoseWin->setText("很遗憾你投降认输了!");
    }
    else if(rea == SurakartaEndReason::TIMEOUT)
    {
        if(your_color == color) ui->LoseWin->setText("对方超时,恭喜你获得胜利!");
        else ui->LoseWin->setText("很遗憾你超时了!");
    }
    else if(rea == SurakartaEndReason::ILLIGAL_MOVE){
        if(your_color == color) ui->LoseWin->setText("对方做出不合法步,恭喜你获得胜利!");
        else ui->LoseWin->setText("很遗憾你进行了非法移动!");
    }

    else if(rea == SurakartaEndReason::STALEMATE)
    {
        ui->LoseWin->setText("平局!");
        ui->winner->setText("NONE");
    }



    // 处理  再来一局  or   返回主菜单

}

OnlineEndDialog::~OnlineEndDialog()
{
    delete ui;
}

// 处理  再来一局
void OnlineEndDialog::on_one_more_game_clicked()
{
    // 发出信号
    // 转到准备的界面 将本地游戏数据初始化
    emit restart_game();
    this->close();
}


void OnlineEndDialog::on_return_start_clicked()
{
    emit return_start();
    this->close();
}

