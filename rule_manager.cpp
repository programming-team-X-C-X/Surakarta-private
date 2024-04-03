#include "rule_manager.h"

void SurakartaRuleManager::OnUpdateBoard() {
    // TODO:
    // Every time the board and game_info is updated to the next round version, this function will be called.
    // You don't need to implement this function if you don't need it.
    // A more delicate way is to use Qt's signal and slot mechanism, but that's the advanced part.
}

SurakartaIllegalMoveReason SurakartaRuleManager::JudgeMove(/*const*/ SurakartaMove& move) {
    // TODO: Implement this function.
    SurakartaPlayer current_player = game_info_->current_player_;

    // 0. judge from piece
    if(!board_->IsInside(move.from) || !board_->IsInside(move.to))
        return SurakartaIllegalMoveReason::OUT_OF_BOARD;

    else if((*board_)[move.from.x][move.from.y]->GetColor() == PieceColor::NONE)
        return SurakartaIllegalMoveReason::NOT_PIECE;

    // move right piece
    else if((*board_)[move.from.x][move.from.y]->GetColor() == current_player)
    {

        // 2. move that doesn't capture
        if((*board_)[move.to.x][move.to.y]->GetColor() == PieceColor::NONE)
        {
            if(IsN_C_M(move)) {
                if (move.pathPoints.empty()){
                    std::shared_ptr<SurakartaPosition>from = std::make_shared<SurakartaPosition>(move.from.x, move.from.y);
                    std::shared_ptr<SurakartaPosition>to = std::make_shared<SurakartaPosition>(move.to.x, move.to.y);
                    move.pathPoints.push_back(from);
                    move.pathPoints.push_back(to);
                }
                return SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
            }
            return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
        }
        // 2. same color
        else if((*board_)[move.to.x][move.to.y]->GetColor() == current_player)
            return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;

        // 3. capture a opponent's piece
        else
        {
            if(Eat(move, move.pathPoints)) {
                move.isLoopMove = true;
                return SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
            }
            return  SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
        }
    }
    else return SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
    //return SurakartaIllegalMoveReason::LEGAL;
}

bool SurakartaRuleManager::Eat(const SurakartaMove& move, std::vector<std::shared_ptr<SurakartaPosition>>& path)
{
    for(unsigned int i = 0;i < BOARD_SIZE/2 - 1; i++)
    {
        for(unsigned int j = 0;j < 4 * BOARD_SIZE; j++)
        {
            if(move.from == *(*board_).Cir[i][j])
            {
                if(EatCircle(move, (*board_).Cir[i], j, path)) return true;
                else if (j == 1) j = 21;
                else if( j == (1 + BOARD_SIZE/2) || j == (1 + 3 * BOARD_SIZE/2) || j == (1 + 5 * BOARD_SIZE/2)) j = j + BOARD_SIZE/2 - 1;
                //else break; // I didn't understand why this is wrong

            }
        }
    }

    return false; //  剩下的表示不正确
}

bool SurakartaRuleManager::EatCircle(const SurakartaMove& move,const circle& circle,unsigned int& i, std::vector<std::shared_ptr<SurakartaPosition>>& path)
{
    unsigned index,Pass;
    Pass = 0;   // 默认没有转弯
    index = i ; // 检查的第一个点

    path.push_back(circle[index]);

    if((index == 4 * BOARD_SIZE - 1) || (index == 3 * BOARD_SIZE - 1) || (index == 2 * BOARD_SIZE - 1)  || (index == BOARD_SIZE - 1) ) {
        // 表示在拐弯的位置
        Pass  = 1;
        path.push_back(circle[index]);
    }

    index = (index + 1) % (4 * BOARD_SIZE);

    while(*circle[index] != move.to && index != i)
    {
        if( (circle[index]->x == move.from.x && circle[index]->y == move.from.y) ||  (*board_)[(*circle[index]).x][(*circle[index]).y]->GetColor() == PieceColor::NONE )
        {
            if(index == 0 ||  index == BOARD_SIZE || (index == 2 * BOARD_SIZE) ||  (index == 3 * BOARD_SIZE) |(index == 4 * BOARD_SIZE - 1) || (index == 3 * BOARD_SIZE - 1) || (index == 2 * BOARD_SIZE - 1)  || (index == BOARD_SIZE - 1 ) ) // 表示拐了弯
            {
                Pass  = 1 ;
                path.push_back(circle[index]);
            }

            index = (index + 1) % (4 * BOARD_SIZE);

        }  // 路径上为空地的时候往后移动
        else break;
        // 表示路径上有障碍,第一条路不通
    }

    if(*circle[index] == move.to && Pass) {
        //可以到达目的地且转了弯
        path.push_back(circle[index]);
        return true;
    }

    // 如果不行，表示这条路行不通，换另一条路
    path.clear();
    Pass = 0;
    index = i;
    path.push_back(circle[index]);
    if(index == 0 ||  index == BOARD_SIZE || index == 2 * BOARD_SIZE ||  index == 3 * BOARD_SIZE ) {
        // 表示在拐弯的位置
        path.push_back(circle[index]);
        Pass  = 1 ;
    }

    index = (i - 1 + 4 * BOARD_SIZE) % (4 * BOARD_SIZE); // 检查的第一个点

    while(*circle[index] != move.to && index != i)
    {
        if((circle[index]->x == move.from.x && circle[index]->y == move.from.y) ||(*board_)[(*circle[index]).x][(*circle[index]).y]->GetColor() == PieceColor::NONE )
        {
            if(index == 0 || index == BOARD_SIZE || (index == 2 * BOARD_SIZE) ||  (index == 3 * BOARD_SIZE) |(index == 4 * BOARD_SIZE - 1) || (index == 3 * BOARD_SIZE - 1) || (index == 2 * BOARD_SIZE - 1)  || (index == BOARD_SIZE - 1 ) ) // 表示拐了弯
            {
                Pass = 1 ;
                path.push_back(circle[index]);
            }
            index = (index - 1 + 4 * BOARD_SIZE) % (4 * BOARD_SIZE);
        }  // 路径上为空地的时候往前移动
        else break;
        // 表示路径上有障碍,第二条路不通
    }
    if(*circle[index] == move.to && Pass) {
        //可以到达目的地且转了弯
        path.push_back(circle[index]);
        return true;
    }

    path.clear();
    return false;
}

std::pair<SurakartaEndReason, SurakartaPlayer> SurakartaRuleManager::JudgeEnd(const SurakartaIllegalMoveReason reason) {

    SurakartaPlayer current_player = game_info_->current_player_;


    if(reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE)
    {

        if((game_info_->num_round_ - game_info_->last_captured_round_)>= game_info_->max_no_capture_round_ )
        {

            int cnt1{},cnt2{};
            for(unsigned int i = 0;i< BOARD_SIZE;i++)
            {
                for(unsigned int j = 0;j < BOARD_SIZE;j++)
                {

                    if((*board_)[i][j]->GetColor() == PieceColor::BLACK) cnt1++;
                    else if((*board_)[i][j]->GetColor() == PieceColor::WHITE) cnt2++;

                }
            }
            if(cnt1 == cnt2)
                return std::make_pair(SurakartaEndReason::STALEMATE, PieceColor::NONE);
            return cnt1 > cnt2 ? std::make_pair(SurakartaEndReason::STALEMATE, PieceColor::BLACK) : std::make_pair(SurakartaEndReason::STALEMATE, PieceColor::WHITE);
        }
        return std::make_pair(SurakartaEndReason::NONE, PieceColor::NONE);
    }

    else if(reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE)
    {
        int cnt{};
        for(unsigned int i = 0;i< BOARD_SIZE;i++)
        {
            for(unsigned int j =0 ;j< BOARD_SIZE;j++)
            {
                if((*board_)[i][j]->GetColor() != current_player && (*board_)[i][j]->GetColor() != PieceColor::NONE) cnt++;
                if(cnt > 1) return std::make_pair(SurakartaEndReason::NONE, PieceColor::NONE);
            }
        }
        return std::make_pair(SurakartaEndReason::CHECKMATE, current_player);
    }
    else //if(reason == SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE || reason == SurakartaIllegalMoveReason::NOT_PIECE ||  reason == SurakartaIllegalMoveReason::NOT_PLAYER_PIECE ||reason == SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE   )
        if(current_player == PieceColor::BLACK)
            return std::make_pair(SurakartaEndReason::ILLIGAL_MOVE, PieceColor::WHITE);
        else return std::make_pair(SurakartaEndReason::ILLIGAL_MOVE, PieceColor::BLACK);

}


std::unique_ptr<std::vector<SurakartaPosition>> SurakartaRuleManager::GetAllLegalTarget(const SurakartaPosition postion) {
    // TODO:
    // We don't test this function, you don't need to implement this function if you don't need it.
    SurakartaMove move;
    move.from = postion;
    std::unique_ptr<std::vector<SurakartaPosition>> rt = std::make_unique<std::vector<SurakartaPosition>>();
    for(unsigned i = 0; i < BOARD_SIZE ; i++)
    {
        for(unsigned j = 0; j < BOARD_SIZE ; j++)
        {
            move.to = {i , j};
            if(JudgeMove(move) == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE || JudgeMove(move) == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) // legal move
                rt->push_back({i,j});
        }
    }
    return rt;
}

bool SurakartaRuleManager::IsN_C_M(const SurakartaMove& move)
{
    if(((int)move.to.x  - (int)move.from.x >= -1 && (int)move.to.x  - (int)move.from.x <= 1 ) && ((int)move.to.y  - (int)move.from.y >= -1 && (int)move.to.y  - (int)move.from.y <= 1 ))
        return true;
    return false;
}

