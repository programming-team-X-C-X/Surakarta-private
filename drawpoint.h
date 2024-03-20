#ifndef DRAWPOINT_H
#define DRAWPOINT_H

#include <QPainter>
inline void my_drawSolidPoint(QPainter *painter, const QPoint &center, int radius, const QColor &color) {
    // 设置画刷颜色
    painter->setBrush(QBrush(color));
    // 绘制实心圆
    painter->drawEllipse(center, radius, radius);
}

#endif // DRAWPOINT_H
