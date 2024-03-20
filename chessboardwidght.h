#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "my_board.h"
#include <QPainter>
#include "drawpoint.h"

class ChessBoardWidget : public QWidget {
public:
    std::shared_ptr<SurakartaBoard> board;
    // ChessBoardWidget(QWidget *parent = nullptr) : QWidget(parent) {
    ChessBoardWidget(std::shared_ptr<SurakartaBoard> board_) {
        board = board_;
        int boardSize = 600;
        setFixedSize(boardSize * 1.5, boardSize);
        setStyleSheet("background-color: grey;");
        // QPushButton *buttonClose = new QPushButton("close");
        // QHBoxLayout *box = nullptr;
        // box->addWidget(chessBoard);
        // box->addWidget(buttonClose);
        // setLayout(box);
    }
    ChessBoardWidget() {
        // board = board_;
        // board = new SurakartaBoard();
        board = std::make_shared<SurakartaBoard>(BOARD_SIZE);
        int boardSize = 600;
        setFixedSize(boardSize * 1.5, boardSize);
        setStyleSheet("background-color: grey;");
        for (unsigned int y = 0; y < BOARD_SIZE; y++) {
            for (unsigned int x = 0; x < BOARD_SIZE; x++) {
                if (y < 2) {
                    (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::BLACK);
                } else if (y >= BOARD_SIZE - 2) {
                    (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::WHITE);
                } else {
                    (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::NONE);
                }
            }
        }
    }

protected:
    void paintEvent(QPaintEvent *event)  {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        int boardSize = 600;
        int arcNum = (BOARD_SIZE - 2) / 2;//环数
        int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
        int gridSize = boardSize / rawNum;
        // 绘制棋盘网格
        painter.setPen(Qt::black);
        for (int i = arcNum; i < rawNum - arcNum - 1; ++i) {
            for (int j = arcNum; j < rawNum - arcNum - 1; ++j) {
                QLineF line1((i + 1) * gridSize, (j + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum), (j + 1) * gridSize);
                QLineF line2((i + 1) * gridSize, (j + 1) * gridSize, (i + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum));
                painter.drawLine(line1);
                painter.drawLine(line2);
            }
        }

        for (int i = 0; i < arcNum; i++)
            painter.drawArc(gridSize*(arcNum-i), gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 0, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter.drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 16*270, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter.drawArc(gridSize*(arcNum-i), (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*90, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter.drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*180, 16*270);

        for (unsigned int y = 0; y < BOARD_SIZE; y++) {
            for (unsigned int x = 0; x < BOARD_SIZE; x++) {
                if ((*board)[x][y]->color_ == PieceColor::BLACK)
                    my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), 10, Qt::black);
                else if ((*board)[x][y]->color_ == PieceColor::WHITE)
                    my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), 10, Qt::white);

            }
        }

    }
};

#endif
