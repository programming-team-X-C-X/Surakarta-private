#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <vector>
#include <memory>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "info_board.h"
#include "info_common.h"
#include "settings.h"

class ChessBoardWidget : public QWidget {
    Q_OBJECT
public:
    enum DrawMode { BoardMode, PieceMode };
    DrawMode currentMode;
    std::shared_ptr<SurakartaBoard> board;
    std::vector<std::vector<std::shared_ptr<SurakartaPiece>>> pieceItems;
    ChessBoardWidget();
    void drawHint(const SurakartaPosition& position);
    void clearHints();
    void setMode(DrawMode newMode);
    QPointF convertPositionToQPointF(const SurakartaPosition& position);

    int arcNum = (BOARD_SIZE - 2) / 2;//环数
    int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
    int gridSize = WINDOW_SIZE / rawNum;

public slots:
    void movePiece(/*const SurakartaPosition& from, const SurakartaPosition& to*/const SurakartaMove& move);
    void receiveHints(const std::vector<SurakartaPosition>& hints);

signals:
    void playerMove(SurakartaPosition from, SurakartaPosition to);
    void animationFinished();
    void requestHints(SurakartaPosition pos);

protected:
    void paintEvent(QPaintEvent */*event*/) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    SurakartaPosition firstClickPos;
    SurakartaPiece *firstClickPiece;
    bool hasFirstClick;
    std::vector<QGraphicsEllipseItem*> hintItems;
    QGraphicsScene *scene;
    QGraphicsView *view;
};

class ChessBoardGraphicsItem : public QGraphicsItem {
public:

    static const int Type = QGraphicsItem::UserType + 1;

    int type() const override {
        return Type;
    }

    QRectF boundingRect() const override {
        // 返回棋盘的边界矩形
        return QRectF(0, 0, WINDOW_SIZE/* * 1.5*/, WINDOW_SIZE);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        // QPainter painter(this);
        painter->setRenderHint(QPainter::Antialiasing);
        int arcNum = (BOARD_SIZE - 2) / 2;//环数
        int rawNum = BOARD_SIZE + arcNum * 2 + 1;//总行数
        int gridSize = WINDOW_SIZE / rawNum;
        // 绘制棋盘网格
        painter->setPen(Qt::black);
        for (int i = arcNum; i < rawNum - arcNum - 1; ++i) {
            for (int j = arcNum; j < rawNum - arcNum - 1; ++j) {
                QLineF line1((i + 1) * gridSize, (j + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum), (j + 1) * gridSize);
                QLineF line2((i + 1) * gridSize, (j + 1) * gridSize, (i + 1) * gridSize, gridSize * (BOARD_SIZE + arcNum));
                painter->drawLine(line1);
                painter->drawLine(line2);
            }
        }
        for (int i = 0; i < arcNum; i++)
            painter->drawArc(gridSize*(arcNum-i), gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 0, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter->drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*(arcNum-i), gridSize*2*(i+1), gridSize*2*(i+1), 16*270, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter->drawArc(gridSize*(arcNum-i), (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*90, 16*270);
        for (int i = 0; i < arcNum; i++)
            painter->drawArc((BOARD_SIZE + arcNum - 1 - i)*gridSize, (BOARD_SIZE + arcNum - 1 - i)*gridSize, gridSize*2*(i+1), gridSize*2*(i+1), 16*180, 16*270);

    }
};

#endif
