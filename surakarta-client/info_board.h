#pragma once

#include <memory>
#include <vector>
#include "info_piece.h"
#include "settings.h"

class SurakartaRow : public std::vector<std::shared_ptr<SurakartaPiece>> {
public:
    SurakartaRow(unsigned int board_size)
        : std::vector<std::shared_ptr<SurakartaPiece>>(board_size) {}
};

class circle: public std::vector<std::shared_ptr<SurakartaPosition>>
{
public:
    circle()
        : std::vector<std::shared_ptr<SurakartaPosition>>(0) {}
};


class SurakartaBoard : public std::vector<SurakartaRow>{
public:
    unsigned int board_size_;
    std::vector<circle> Cir;
    SurakartaBoard(unsigned int board_size)
        : board_size_(board_size),Cir(board_size_)
    {
        for (unsigned int i = 0; i < board_size_; i++) {
            this->push_back(SurakartaRow(board_size_));
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
};

// 存储棋盘状态
class MiniBoard{
public:
    MiniBoard()
    {
        board.resize(BOARD_SIZE);
    }
    std::vector<std::vector<PieceColor>> board;

    void operator=(const SurakartaBoard& bd)
    {
        for(unsigned i = 0;i < BOARD_SIZE;++i)
        {
            for(unsigned j = 0;j < BOARD_SIZE;++j)
            {
                board[i].push_back(bd[i][j]->GetColor());
            }
        }
    }
};
