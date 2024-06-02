#include "chess_board_widght.h"
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QDebug>
#include <QHBoxLayout>
#include "info_board.h"
#include "info_piece.h"

ChessBoardWidget::ChessBoardWidget() :
    currentMode(BoardMode), hasFirstClick(false)
{
    board = std::make_shared<SurakartaBoard>(BOARD_SIZE);

    pieceItems.resize(BOARD_SIZE, std::vector<std::shared_ptr<SurakartaPiece>>(BOARD_SIZE));

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    ChessBoardGraphicsItem *chessBoardItem = new ChessBoardGraphicsItem();

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->addItem(chessBoardItem);
    view->setBackgroundBrush(QBrush(Qt::transparent));

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

    if(PLAYER_COLOR != RIGHT_COLOR) return;

    if (item) {
        SurakartaPiece *piece = qgraphicsitem_cast<SurakartaPiece*>(item);

        if (hasFirstClick) {
            SurakartaPosition secondClickPos;
            if (piece) {
                if(piece->color_ == ((PLAYER_COLOR ) ? PieceColor::BLACK: PieceColor::WHITE))
                {
                    clearHints();
                    SurakartaPosition pos = piece->GetPosition();

                    // 两次点击一样的  清除提示
                    if(pos == firstClickPos) {
                        hasFirstClick = false;
                        return;
                    }
                    emit requestHints(pos);
                    firstClickPos = pos;
                    firstClickPiece = piece;
                }
                else {
                    secondClickPos = piece->GetPosition();
                    hasFirstClick = false;
                    clearHints();
                    emit playerMove(firstClickPos, secondClickPos);
                }
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
                hasFirstClick = false;
                clearHints();
                emit playerMove(firstClickPos, secondClickPos);
            }

            // qDebug() << "second:" << secondClickPos.x << "," << secondClickPos.y << '\n';
        }
        else if (piece && piece->color_ == ((PLAYER_COLOR ) ? PieceColor::BLACK: PieceColor::WHITE)) { // 第一次点击
            SurakartaPosition pos = piece->GetPosition();
            emit requestHints(pos);
            firstClickPos = pos;
            firstClickPiece = piece;
            hasFirstClick = true;
            // qDebug() << "first:" << pos.x << "," << pos.y << '\n';
        }

    }
}

void ChessBoardWidget::receiveCaptureHints(const std::vector<SurakartaPosition>& hints) {
    for (const auto& hintPos : hints) {
        drawCaptureHint(hintPos);
    }
}

void ChessBoardWidget::receiveDangerousHints(const std::vector<SurakartaPosition>& hints) {
    for (const auto& hintPos : hints) {
        drawDangerousHint(hintPos);
    }
}

void ChessBoardWidget::receiveNONCaptureHints(const std::vector<SurakartaPosition>& hints) {
    for (const auto& hintPos : hints) {
        drawNONCaptureHint(hintPos);
    }
}

// 成员函数以在指定位置绘制提示
void ChessBoardWidget::drawCaptureHint(const SurakartaPosition& position) {
    drawNONCaptureHint(position);
    auto hintItem = new QGraphicsEllipseItem(convertPositionToQPointF(position).x()+0.75, convertPositionToQPointF(position).y()+0.75, PIECE_SIZE-1.5, PIECE_SIZE-1.5);

    QPen pen(Qt::NoPen);
    hintItem->setPen(pen);

    QColor fillColor(193, 255, 193, 127); // 半透明的绿色
    hintItem->setBrush(QBrush(fillColor));

    scene->addItem(hintItem);
    hintItems.push_back(hintItem);
}

void ChessBoardWidget::drawDangerousHint(const SurakartaPosition& position) {
    auto hintItem = new QGraphicsEllipseItem(convertPositionToQPointF(position).x()+0.75, convertPositionToQPointF(position).y()+0.75, PIECE_SIZE-1.5, PIECE_SIZE-1.5);

    QPen pen(Qt::NoPen);
    hintItem->setPen(pen);

    QColor fillColor(255, 0, 0, 127); // 半透明的红色
    hintItem->setBrush(QBrush(fillColor));

    scene->addItem(hintItem);
    hintItems.push_back(hintItem);
}

void ChessBoardWidget::drawNONCaptureHint(const SurakartaPosition& position) {
    auto hintItem = new QGraphicsEllipseItem(convertPositionToQPointF(position).x(), convertPositionToQPointF(position).y(), PIECE_SIZE, PIECE_SIZE);

    QPen pen(QColor(67, 44, 216)); // 蓝色
    pen.setWidth(3);
    hintItem->setPen(pen);
    hintItem->setBrush(QBrush(Qt::transparent));
    scene->addItem(hintItem);
    hintItems.push_back(hintItem);
}

// 成员函数以清除所有之前的提示
void ChessBoardWidget::clearHints() {
    for (auto& hintItem : hintItems) {
        scene->removeItem(hintItem);
        delete hintItem;
    }
    hintItems.clear(); // 清空列表
}

void ChessBoardWidget::movePiece(const SurakartaMove& move) {
    auto piece = pieceItems[move.from.x][move.from.y];
    if (piece) {
        QPainterPath animationPath;
        double pathLength = 0.0;
        animationPath.moveTo(convertPositionToQPointF(move.from));

        QPointF oldPoint = convertPositionToQPointF(move.from);
        for (const std::shared_ptr<SurakartaPosition>& pos : move.pathPoints) {
            QPointF currentPoint = convertPositionToQPointF(*pos);
            if (oldPoint != currentPoint)
                pathLength += QLineF(oldPoint, currentPoint).length();
            oldPoint = currentPoint;
        }
        for (size_t i = 0; i< move.pathPoints.size(); i++) {
            const auto currentPos = *move.pathPoints[i];
            QPointF currentPoint = convertPositionToQPointF(currentPos);
            if (i == 0 || !move.isLoopMove)
                animationPath.lineTo(currentPoint);
            else {
                const auto lastPos = *move.pathPoints[i - 1];
                QPointF lastPoint = convertPositionToQPointF(lastPos);
                if (lastPoint.x() == currentPoint.x() || lastPoint.y() == currentPoint.y())
                    animationPath.lineTo(currentPoint);
                else {
                    // QPointF start = convertPositionToQPointF(lastPoint);

                    if (currentPos.x < BOARD_SIZE / 2 && currentPos.y < BOARD_SIZE / 2) {
                        double radius = (currentPos.x + currentPos.y) * gridSize;
                        QPointF center = convertPositionToQPointF(SurakartaPosition(0, 0));
                        // 创建一个矩形，表示圆的外接矩形
                        QRectF rectangle(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
                        if (lastPos.x == 0) {//shun
                            qreal startAngle = 270;
                            qreal spanAngle = -270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                        if (lastPos.y == 0) {//ni
                            qreal startAngle = 0;
                            qreal spanAngle = 270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                    }
                    else if (currentPos.x >= BOARD_SIZE / 2 && currentPos.y < BOARD_SIZE / 2) { //右上
                        double radius = (std::max(currentPos.y, lastPos.y)) * gridSize;
                        QPointF center = convertPositionToQPointF(SurakartaPosition(BOARD_SIZE - 1, 0));
                        QRectF rectangle(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
                        if (lastPos.y == 0) {//shun
                            qreal startAngle = 180;
                            qreal spanAngle = -270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                        if (currentPos.y == 0) {//ni
                            qreal startAngle = 270;
                            qreal spanAngle = 270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                    }
                    else if (currentPos.x < BOARD_SIZE / 2 && currentPos.y >= BOARD_SIZE / 2) { //左下
                        double radius = (std::max(currentPos.x, lastPos.x)) * gridSize;
                        QPointF center = convertPositionToQPointF(SurakartaPosition(0, BOARD_SIZE - 1));
                        QRectF rectangle(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
                        if (lastPos.x == 0) {//ni
                            qreal startAngle = 90;
                            qreal spanAngle = 270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                        if (currentPos.x == 0) {//shun
                            qreal startAngle = 0;
                            qreal spanAngle = -270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                    }
                    else if (currentPos.x >= BOARD_SIZE / 2 && currentPos.y >= BOARD_SIZE / 2) {    //右下
                        double radius = (2 * BOARD_SIZE - 2 - currentPos.x - lastPos.x) * gridSize;
                        QPointF center = convertPositionToQPointF(SurakartaPosition(BOARD_SIZE - 1, BOARD_SIZE - 1));
                        QRectF rectangle(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
                        if (lastPos.y == BOARD_SIZE - 1) {//ni
                            qreal startAngle = 180;
                            qreal spanAngle = 270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                        if (currentPos.y == BOARD_SIZE - 1) {//shun
                            qreal startAngle = 90;
                            qreal spanAngle = -270;
                            animationPath.arcTo(rectangle, startAngle, spanAngle);
                        }
                    }
                }
            }
        }
        auto animation = new QVariantAnimation(this);
        int duration = static_cast<int>(pathLength / PIECE_SPEED * 1000);
        animation->setDuration(duration); // 动画时间
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        connect(animation, &QVariantAnimation::valueChanged, [animationPath, piece](const QVariant &value) {
            qreal t = value.toReal();
            QPointF newPos = animationPath.pointAtPercent(t); // 获取路径上的位置
            piece->setPos(newPos); // 移动棋子到这个位置
        });
        connect(animation, &QVariantAnimation::finished, this, [this, move, animation, piece]() {
            animation->deleteLater(); // 删除动画对象
            emit animationFinished();
            pieceItems[move.from.x][move.from.y] = nullptr;
            piece->position_ = move.to; // 更新棋子的最终位置
            pieceItems[move.to.x][move.to.y] = piece;
        });
        animation->start(); // 开始动画
    }
}

QPointF ChessBoardWidget::convertPositionToQPointF(const SurakartaPosition& position) {
    return QPointF((arcNum + position.x + 1) * gridSize - PIECE_SIZE / 2,
                   (arcNum + position.y + 1) * gridSize - PIECE_SIZE / 2);
}

void ChessBoardWidget::setMode(DrawMode mode) {
    currentMode = mode;
}


ChessBoardWidget::ChessBoardWidget(const MiniBoard& board_)
{
    // 创建新的场景和视图
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    ChessBoardGraphicsItem *chessBoardItem = new ChessBoardGraphicsItem();
    scene->addItem(chessBoardItem);
     board = std::make_shared<SurakartaBoard>(BOARD_SIZE);
     pieceItems.resize(BOARD_SIZE, std::vector<std::shared_ptr<SurakartaPiece>>(BOARD_SIZE));

    // 设置视图属性
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setBackgroundBrush(QBrush(Qt::transparent));
    view->setRenderHint(QPainter::Antialiasing);

    // 设置视图和场景的大小和位置
    view->setFixedSize(WINDOW_SIZE, WINDOW_SIZE);
    scene->setSceneRect(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    // 添加棋子
    for (unsigned int y = 0; y < BOARD_SIZE; y++) {
        for (unsigned int x = 0; x < BOARD_SIZE; x++) {
            if (board_.board[x][y] == PieceColor::NONE) {
                (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::NONE);
            } else {
                (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, board_.board[x][y]);
                pieceItems[x][y] = std::make_shared<SurakartaPiece>(x, y, (*board)[x][y]->color_);
                SurakartaPiece *pieceItem = pieceItems[x][y].get();
                int pos_x = (arcNum + x + 1) * gridSize;
                int pos_y = (arcNum + y + 1) * gridSize;
                pieceItem->setPos(pos_x - PIECE_SIZE / 2, pos_y - PIECE_SIZE / 2);
                scene->addItem(pieceItem);
            }
        }
    }
}
