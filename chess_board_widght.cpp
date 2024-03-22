#include "my_common.h"
#include <QMouseEvent>
#include "chess_board_widght.h"
#include <QWidget>
#include "my_board.h"
#include <QPainter>
#include "drawpoint.h"
#include "settings.h"
#include <cmath>

ChessBoardWidget::ChessBoardWidget() :
    hasFirstClick(false), currentMode(BoardMode)
{
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

    if (currentMode == DrawMode::PieceMode)
        for (unsigned int y = 0; y < BOARD_SIZE; y++) {
            for (unsigned int x = 0; x < BOARD_SIZE; x++) {
                if ((*board)[x][y]->color_ == PieceColor::BLACK)
                    my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), PIECE_SIZE, Qt::black);
                else if ((*board)[x][y]->color_ == PieceColor::WHITE)
                    my_drawSolidPoint(&painter, QPoint((arcNum + x + 1) * gridSize, (arcNum + y + 1) * gridSize), PIECE_SIZE, Qt::white);

            }
        }

}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event) {

    int arcNum = (BOARD_SIZE - 2) / 2;
    int rawNum = BOARD_SIZE + arcNum * 2 + 1;
    int gridSize = WINDOW_SIZE / rawNum;

    // 将鼠标点击位置转换为棋盘坐标
    // unsigned int x = (event->position().x() - gridSize * arcNum) / gridSize - 1;
    // unsigned int y = (event->position().y() - gridSize * arcNum) / gridSize - 1;
    // 计算格子坐标前先转换为浮点数
    float fx = (event->position().x() - gridSize * arcNum) / (float)gridSize;
    float fy = (event->position().y() - gridSize * arcNum) / (float)gridSize;

    // 使用四舍五入找到最接近的整数格子坐标
    unsigned int x = static_cast<unsigned int>(round(fx)) - 1;
    unsigned int y = static_cast<unsigned int>(round(fy)) - 1;

    if (x < BOARD_SIZE && y < BOARD_SIZE) {
        if (!hasFirstClick) {
            // 如果这是第一次点击，记录棋子的位置并设置标志
            firstClickPos = {x, y};
            hasFirstClick = true;
        } else {
            // 这是第二次点击，获取目的位置
            SurakartaPosition secondClickPos = {x, y};
            emit playerMove(firstClickPos, secondClickPos);

            // 重置状态，等待下一对点击
            hasFirstClick = false;
        }
    }
}

void ChessBoardWidget::setMode(DrawMode mode) {
    currentMode = mode;
}
