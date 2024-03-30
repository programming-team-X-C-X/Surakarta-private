#include <QMouseEvent>
#include "chess_board_widght.h"
#include "my_board.h"
#include "my_piece.h"
#include <QPainter>
#include "settings.h"
#include <cmath>
#include <QDebug>
// #include <QScrollBar>

ChessBoardWidget::ChessBoardWidget() :
    currentMode(BoardMode), hasFirstClick(false)
{
    board = std::make_shared<SurakartaBoard>(BOARD_SIZE);
    // setFixedSize(WINDOW_SIZE * 1.5, WINDOW_SIZE);
    // setStyleSheet("background-color: grey;");

    pieceItems.resize(BOARD_SIZE, std::vector<std::shared_ptr<SurakartaPiece>>(BOARD_SIZE));

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    ChessBoardGraphicsItem *chessBoardItem = new ChessBoardGraphicsItem();
    // view->setFixedSize(WINDOW_SIZE, WINDOW_SIZE);
    // view->setSceneRect(chessBoardItem->boundingRect());

    // QRectF sceneRect = QRectF(0, 0, WINDOW_SIZE/**0.9*/, WINDOW_SIZE/**0.9*/);
    // scene->setSceneRect(sceneRect);

    // view->setBackgroundBrush(QBrush(Qt::transparent));
    scene->addItem(chessBoardItem);

    for (unsigned int y = 0; y < BOARD_SIZE; y++) {
        for (unsigned int x = 0; x < BOARD_SIZE; x++) {
            if (y < 2) {
                (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::BLACK);
                pieceItems[x][y] = std::make_shared<SurakartaPiece>(x, y, (*board)[x][y]->color_);
                SurakartaPiece *pieceItem = pieceItems[x][y].get();
                int pos_x = (arcNum + x + 1) * gridSize;
                int pos_y = (arcNum + y + 1) * gridSize;
                pieceItem->setPos(pos_x-PIECE_SIZE/2, pos_y-PIECE_SIZE/2);
                scene->addItem(pieceItem);
            } else if (y >= BOARD_SIZE - 2) {
                (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::WHITE);
                pieceItems[x][y] = std::make_shared<SurakartaPiece>(x, y, (*board)[x][y]->color_);
                SurakartaPiece *pieceItem = pieceItems[x][y].get();
                int pos_x = (arcNum + x + 1) * gridSize;
                int pos_y = (arcNum + y + 1) * gridSize;
                pieceItem->setPos(pos_x-PIECE_SIZE/2, pos_y-PIECE_SIZE/2);
                scene->addItem(pieceItem);
            } else {
                (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::NONE);
            }
        }
    }
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
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event) {
    scene = view->scene();
    QPointF scenePos = view->mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
    // QList items = scene->selectedItems();
    // QList views = scene->views();
    if (item) {
        SurakartaPiece *piece = qgraphicsitem_cast<SurakartaPiece*>(item);
        if (hasFirstClick) {
            SurakartaPosition secondClickPos;
            if (piece) {
                secondClickPos = piece->GetPosition();
            }
            else {
                    // 计算格子坐标前先转换为浮点数
                    float fx = (event->position().x() - gridSize * arcNum) / (float)gridSize;
                    float fy = (event->position().y() - gridSize * arcNum) / (float)gridSize;
                    // 使用四舍五入找到最接近的整数格子坐标
                    unsigned int x = static_cast<unsigned int>(round(fx)) - 1;
                    unsigned int y = static_cast<unsigned int>(round(fy)) - 1;
                    // SurakartaPosition *pos = new SurakartaPosition(fx, fy);
                    secondClickPos.x = x;
                    secondClickPos.y = y;
            }

            hasFirstClick = false;

            // piece->position_ = secondClickPos;
            // pieceItems[firstClickPos.x][firstClickPos.y] = pieceItems[secondClickPos.x][secondClickPos.y];

            // pieceItems[firstClickPos.x][firstClickPos.y] = nullptr;
            // pieceItems[secondClickPos.x][secondClickPos.y] = std::make_shared<SurakartaPiece>(firstClickPiece->GetPosition().x, firstClickPiece->GetPosition().y, firstClickPiece->GetColor());
            emit playerMove(firstClickPos, secondClickPos);
            qDebug() << "second:" << secondClickPos.x << "," << secondClickPos.y << '\n';
        }
        else if (piece) {
            SurakartaPosition pos = piece->GetPosition();
            // 第一次点击
            firstClickPos = pos;
            firstClickPiece = piece;
            hasFirstClick = true;
            qDebug() << "first:" << pos.x << "," << pos.y << '\n';
        }
    }
}

void ChessBoardWidget::movePiece(const SurakartaPosition& from, const SurakartaPosition& to) {
    std::shared_ptr<SurakartaPiece>piece = pieceItems[from.x][from.y];
    if(piece) {
        // float fx = (to.x - gridSize * arcNum) / (float)gridSize;
        // float fy = (event->position().y() - gridSize * arcNum) / (float)gridSize;
        QPointF newPos((arcNum + to.x + 1) * gridSize - PIECE_SIZE / 2, (arcNum + to.y + 1) * gridSize - PIECE_SIZE / 2);
        piece->setPos(newPos);
        // Update the pieceItems array
        pieceItems[from.x][from.y].reset();
        piece->position_.x = to.x;
        piece->position_.y = to.y;
        pieceItems[to.x][to.y] = piece;
        // (*board)[from.x][from.y] = nullptr;
        // pieceItems[from.x][from.y] = nullptr;
    }
}

void ChessBoardWidget::setMode(DrawMode mode) {
    currentMode = mode;
}
