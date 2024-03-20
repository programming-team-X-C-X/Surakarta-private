#pragma once

#include <memory>
#include <vector>
#include "my_piece.h"

#define BOARD_SIZE 6

class SurakartRow : public std::vector<std::shared_ptr<SurakartaPiece>> {
public:
    SurakartRow(unsigned int board_size)
        : std::vector<std::shared_ptr<SurakartaPiece>>(board_size) {}
};

class circle: public std::vector<std::shared_ptr<SurakartaPosition>>
{
public:
    circle()
        : std::vector<std::shared_ptr<SurakartaPosition>>(0) {}
};


class SurakartaBoard : public std::vector<SurakartRow>{
public:
    unsigned int board_size_;
    std::vector<circle> Cir;
    SurakartaBoard(unsigned int board_size)
        : board_size_(board_size),Cir(board_size_)
    {
        for (unsigned int i = 0; i < board_size_; i++) {
            this->push_back(SurakartRow(board_size_));
        }
        for (unsigned int i = 0; i < board_size_/2 - 1; i++)
        {
            for(unsigned int j = 1; j <= 4 ; j++)
            {

                for(unsigned int k = 0; k < board_size_; k++)
                {
                    std::shared_ptr<SurakartaPosition> rt;
                    if     ( j == 1)
                        rt = std::make_shared<SurakartaPosition>(k, i+1);
                    else if( j == 2)
                        rt = std::make_shared<SurakartaPosition>(board_size_ - 2 - i,k);
                    else if( j == 3)
                        rt = std::make_shared<SurakartaPosition>(board_size_ - k - 1,board_size_ - 2 - i);
                    else if( j == 4)
                        rt = std::make_shared<SurakartaPosition>(i+1,board_size_ - k - 1);
                    Cir[i].push_back(rt);
                }
            }
        }

    }

    bool IsInside(const SurakartaPosition& position) const {
        return position.x < board_size_ && position.y < board_size_;
    }




    friend inline std::ostream& operator<<(std::ostream& os, const SurakartaBoard& board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
                os << (*board[x][y]) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend inline std::istream& operator>>(std::istream& is, SurakartaBoard& board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
                char ch;
                is >> ch;
                PieceColor color;
                switch (ch) {
                case 'B':
                    color = PieceColor::BLACK;
                    break;
                case 'W':
                    color = PieceColor::WHITE;
                    break;
                default:
                    color = PieceColor::NONE;
                    break;
                }
                board[x][y] = std::make_shared<SurakartaPiece>(x, y, color);
            }
        }
        return is;
    }
};
