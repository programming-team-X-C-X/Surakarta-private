#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "my_board.h"
#include <QPainter>

class ChessBoardWidget : public QWidget {
public:
    std::shared_ptr<SurakartaBoard> board;
    ChessBoardWidget(std::shared_ptr<SurakartaBoard> board_);
    ChessBoardWidget();

protected:
    void paintEvent(QPaintEvent */*event*/);
};

#endif
