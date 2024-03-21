#include "chessboardwidght.h"
#include <QWidget>
#include "my_board.h"
#include <QPainter>
#include "drawpoint.h"
#include "settings.h"

ChessBoardWidget::ChessBoardWidget() {
    board = std::make_shared<SurakartaBoard>(BOARD_SIZE);
    setFixedSize(WINDOW_SIZE * 1.5, WINDOW_SIZE);
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

ChessBoardWidget::ChessBoardWidget(std::shared_ptr<SurakartaBoard> board_) {
    board = board_;
    setFixedSize(WINDOW_SIZE * 1.5, WINDOW_SIZE);
    setStyleSheet("background-color: grey;");
    // QPushButton *buttonClose = new QPushButton("close");
    // QHBoxLayout *box = nullptr;
    // box->addWidget(chessBoard);
    // box->addWidget(buttonClose);
    // setLayout(box);
}

void ChessBoardWidget::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int arcNum = (BOARD_SIZE - 2) / 2;//环数
    int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
    int gridSize = WINDOW_SIZE / rawNum;
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
                my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), PIECE_SIZE, Qt::black);
            else if ((*board)[x][y]->color_ == PieceColor::WHITE)
                my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), PIECE_SIZE, Qt::white);

        }
    }

}






