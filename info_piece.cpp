#include "info_piece.h"
#include "settings.h"
#include <QPainter>

QRectF SurakartaPiece::boundingRect() const {
    //i'm sb
    return QRectF(0, 0, PIECE_SIZE, PIECE_SIZE);
}

void SurakartaPiece::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // 设置画刷颜色
    QColor color = (this->color_ == PieceColor::BLACK) ? Qt::black : Qt::white;
    painter->setBrush(color);

    // 绘制一个圆形来代表棋子
    painter->drawEllipse(boundingRect());
    // std::cout<<"yes";
}
