#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "my_board.h"
#include <QPainter>

class ChessBoardWidget : public QWidget {
    Q_OBJECT
public:
    enum DrawMode { BoardMode, PieceMode };
    DrawMode currentMode;
    std::shared_ptr<SurakartaBoard> board;
    ChessBoardWidget(std::shared_ptr<SurakartaBoard> board_);
    ChessBoardWidget();
    void setMode(DrawMode newMode);

signals:
    void playerMove(SurakartaPosition from, SurakartaPosition to);

protected:
    void paintEvent(QPaintEvent */*event*/) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // QPoint firstClickPos;
    SurakartaPosition firstClickPos;
    bool hasFirstClick;
};

#endif
