#include "mainwindow.h"
#include "game_view.h"
#include <QApplication>

QStringList arg;

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    for(int i = 0; argv[i] ;i++) arg.append(argv[i]);

    MainWindow w;
    //w.show();
    return a.exec();
}
