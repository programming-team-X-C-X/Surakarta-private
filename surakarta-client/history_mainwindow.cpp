#include "history_mainwindow.h"
#include "ui_history_mainwindow.h"
#include "info_piece.h"
#include <QDir>
#include <QToolBar>

History_MainWindow::History_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::History_MainWindow)
{
    ui->setupUi(this);
    QString directoryPath = "../history";

    // 打开该路径下
    QDir directory(directoryPath);

    // 找到对应文件
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList fileList = directory.entryList();

    // 加入下拉框
    ui->comboBox->addItems(fileList);
}

History_MainWindow::~History_MainWindow()
{
    delete ui;
}

void History_MainWindow::on_pushButton_clicked()
{
    // 点击确定开始载入游戏
    qDebug() << "Failed to open file";

    // 初始化
    //std::vector<SurakartaBoard> boards;

    game_info = "";
    moves.clear();
    //boards.clear();
    step = 0;
    game = new SurakartaGame;
    game->StartGame();
    //boards.push_back(*game->board_.get());

    // 绑定 打开 对应文件
    QFile file("../history/" + ui->comboBox->currentText());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
    }

    QTextStream read_game(&file);

    game_info = read_game.readLine();

    // 将文件存储变成对应步数
    QStringList moveList = game_info.split(' ');

    for(unsigned i = 0; i < moveList.size();i++)
    {
        SurakartaMove move;
        SurakartaBoard board_{BOARD_SIZE};
        move.from = SurakartaPosition(moveList[i][0].unicode() - 65,moveList[i][1].digitValue() - 1);
        move.to   = SurakartaPosition(moveList[i][3].unicode() - 65,moveList[i][4].digitValue() - 1);
        move.player = static_cast<SurakartaPlayer> (i%2);
        moves.push_back(move);
        game->Move(move);
       // boards.push_back(*game->board_.get());
    }

    // 创建对局
    chessBoard = new ChessBoardWidget();
    setCentralWidget(chessBoard);


    QToolBar* toolBar = addToolBar("Tools");

    QAction* startPauseAction = new QAction("开始");
    QAction* preAction = new QAction("上一步");
    QAction* nextAction = new QAction("下一步");

    toolBar->addAction(startPauseAction);
    toolBar->addAction(preAction);
    toolBar->addAction(nextAction);

    // 初始点击状态
    preAction->setDisabled(true);
    nextAction->setDisabled(true);


    connect(nextAction, &QAction::triggered, this, &History_MainWindow::onNextButtonClicked);
    connect(preAction, &QAction::triggered, this, &History_MainWindow::onPreButtonClicked);
    connect(startPauseAction, &QAction::triggered, this, [=](){
        if(startPauseAction->text() == "开始")
        {

            startPauseAction->setText("暂停");

            preAction->setDisabled(true);
            nextAction->setDisabled(true);
        }
        else
        {
            startPauseAction->setText("开始");

            preAction->setDisabled(false);
            nextAction->setDisabled(false);
        }
    });


}

void History_MainWindow::onNextButtonClicked()
{
    qDebug() << "Next button clicked";
    // Add your handling code here
    loadGame(++step);
}

void History_MainWindow::onPreButtonClicked()
{
    qDebug() << "Previous button clicked";
    // Add your handling code here
    loadGame(--step);
}

void History_MainWindow::loadGame(unsigned cur_step)
{
    // 清空棋盘
   // chessBoard->clearScene(boards[cur_step]);

}
