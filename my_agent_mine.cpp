#include "my_agent_mine.h"

SurakartaMove SurakartaAgentMine::CalculateMove() {
    // TODO: Implement your own ai here.

    // 1. Get All posible ways to go
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> Go = GetAllPositions();

    // 2. Choose one step that has the biggest mark

    // 3.
    SurakartaMove rt = GetBestMove(Go);


    return rt;
}


std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> SurakartaAgentMine::GetAllPositions() const
{
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> rt;    //  store from and all its legal to
    SurakartaPosition _from{};                                                                       //  store from
    std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>> tmp;
    for(unsigned i = 0; i < board_size_; i++)
    {
        for(unsigned j = 0; j < board_size_; j++)
        {
            if((*board_)[i][j]->GetColor() == game_info_->current_player_)                           //  Find own qizi
            {
                _from = {i,j};
                std::unique_ptr<std::vector<SurakartaPosition>> AllTo = rule_manager_->GetAllLegalTarget(_from);    //  store all to
                tmp = std::make_pair(_from, std::move(AllTo));
                rt.push_back(std::move(tmp));
            }
        }
    }
    return rt;
}

int SurakartaAgentMine::CalculateMarks(SurakartaBoard& _board) const
{
    int mark{};

    for(unsigned i = 0; i < board_size_; i++ )
    {
        for(unsigned j = 0; j < board_size_; j++)
        {
            if(_board[i][j]->GetColor() == game_info_->current_player_ )
            {
                if(game_info_->current_player_ == PieceColor::WHITE)
                {
                    if((i == board_size_/2 - 1 ) || (j == board_size_/2 - 1) )
                    {
                        mark += 8;
                    }
                    else if(j >= board_size_ - 2 )  continue;
                    else mark += 2;
                }
                else
                {
                    if((i == board_size_/2 + 1 ) || (j == board_size_/2 + 1 ) )
                    {
                        mark += 8;
                    }
                    else if(j <= 1 )  continue;
                    else mark += 2;
                }
            }
            else if(_board[i][j]->GetColor() ==  PieceColor::NONE) continue;
            else  mark -= 20;

        }
    }

    return mark;
}

SurakartaMove SurakartaAgentMine::GetBestMove(std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>>& uin) const
{
    int MaxP{-999};
    int CurP{};
    SurakartaMove tmp;
    SurakartaMove best;
    SurakartaBoard _board{*board_};
    PieceColor ls1,ls2;
    //CopyBoard(_board);

    for(unsigned i = 0;i < uin.size(); i++ )    // number of from
    {
        for(unsigned j = 0;j < uin[i].second->size(); j++)
        {
            tmp = {uin[i].first,uin[i].second->at(j),game_info_->current_player_};
            //   move
            ls1 = _board[tmp.from.x][tmp.from.y]->color_;
            ls2 = _board[tmp.to.x][tmp.to.y]->color_ ;
            _board[tmp.from.x][tmp.from.y]->color_ = PieceColor::NONE;
            _board[tmp.to.x][tmp.to.y]->color_     = game_info_->current_player_;


            CurP = CalculateMarks(_board);
            if(CurP > MaxP)
            {
                MaxP = CurP;
                best = tmp;
            }

            //   back

            _board[tmp.from.x][tmp.from.y]->color_ = ls1 ;
            _board[tmp.to.x][tmp.to.y]->color_     = ls2;

        }
    }

    return best;
}

void SurakartaAgentMine::CopyBoard(SurakartaBoard& ls) const
{
    for(unsigned i = 0;i < board_size_; i++)
    {
        for(unsigned j = 0;j < board_size_; j++)
        {
            ls[i][j]->color_ = (*board_)[i][j]->GetColor();
        }
    }
}

