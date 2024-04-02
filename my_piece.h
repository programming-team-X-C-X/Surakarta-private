#pragma once

#include <iostream>
#include <QGraphicsObject>
// #include <QRectF>

using PieceColorMemoryType = int;
enum class PieceColor : PieceColorMemoryType { BLACK,
                                               WHITE,
                                               NONE,
                                               UNKNOWN
};

using SurakartaPlayer = PieceColor;

inline PieceColor ReverseColor(PieceColor color) {
    switch (color) {
    case PieceColor::BLACK:
        return PieceColor::WHITE;
    case PieceColor::WHITE:
        return PieceColor::BLACK;
    default:
        return color;
    }
}
struct SurakartaPosition {
    unsigned int x;
    unsigned int y;
    SurakartaPosition(unsigned int x = 0, unsigned int y = 0)
        : x(x), y(y) {}
    friend std::ostream& operator<<(std::ostream& os, const SurakartaPosition& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
    bool operator==(const SurakartaPosition& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const SurakartaPosition& rhs) const {
        return !(*this == rhs);
    }
};

class SurakartaPiece : public QGraphicsObject {
public:

    static const int Type = QGraphicsItem::UserType + 2;

    int type() const override {
        return Type;
    }

    SurakartaPiece()
        : position_({0, 0}), color_(PieceColor::NONE) {}

    SurakartaPiece(unsigned int x, unsigned int y, PieceColor color)
        : position_({x, y}), color_(color) {}

    SurakartaPiece(SurakartaPosition position, PieceColor color)
        : position_(position), color_(color) {}

    void SetPosition(SurakartaPosition position) { position_ = position; }
    void SetColor(PieceColor color) { color_ = color; }
    void Set(SurakartaPosition position, PieceColor color) {
        position_ = position;
        color_ = color;
    }
    SurakartaPosition GetPosition() const { return position_; }
    PieceColor GetColor() const { return color_; }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    // void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    //    private:
    SurakartaPosition position_;
    PieceColor color_;
};
