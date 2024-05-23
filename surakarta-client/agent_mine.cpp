#include "agent_mine.h"

// 计算当前棋局评分
int EvaluateBoardFor(const SurakartaBoard& board, PieceColor currentPlayer) {
    int score = 0;
    // ... 实现评分逻辑
    for(unsigned i = 0; i < board.board_size_; i++ )
    {
        for(unsigned j = 0; j < board.board_size_; j++)
        {
            if(board[i][j]->GetColor() == currentPlayer)
            {
                if(currentPlayer == PieceColor::WHITE)
                {
                    if((i == board.board_size_/2 - 1 ) || (j == board.board_size_/2 - 1) )
                    {
                        score += 8;
                    }
                    else if(j >= board.board_size_ - 2 )  continue;
                    else score += 2;
                }
                else
                {
                    if((i == board.board_size_/2 + 1 ) || (j == board.board_size_/2 + 1 ) )
                    {
                        score += 8;
                    }
                    else if(j <= 1 )  continue;
                    else score += 2;
                }
            }
            else if(board[i][j]->GetColor() ==  PieceColor::NONE) continue;
            else  score -= 20;
        }
    }
    return score;
}

// 检查游戏是否结束
bool IsGameOver(const SurakartaBoard& board) {
    // ... 实现检查逻辑
    for(unsigned i = 0; i < board.board_size_; i++ )
        for(unsigned j = 0; j < board.board_size_; j++)
            if(board[i][j]->GetColor() == SurakartaPlayer::WHITE || board[i][j]->GetColor() == SurakartaPlayer::BLACK)
                return false;
    return true;
}

// minimax 的递归部分
int Minimax(SurakartaBoard& board, int depth, bool maximizingPlayer,
            int alpha, int beta, PieceColor currentPlayer) {
    if (depth == 0 || IsGameOver(board)) {
        return EvaluateBoardFor(board, currentPlayer);
    }

    // int bestScore;
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves;
    // std::shared_ptr<const SurakartaBoard> board_ = rule_manager.board_;
    // SurakartaBoard board_ = (*rule_manager.board_);

    //找到所有可行
    SurakartaRuleManager rule_manager(std::make_shared<SurakartaBoard>(board), std::make_shared<SurakartaPlayer>(currentPlayer));
    for(unsigned i = 0; i < board.board_size_; i++ ){
        for(unsigned j = 0; j < board.board_size_; j++){
            if((board)[i][j]->GetColor() == currentPlayer)
            // auto lagelMoves = rule_manager.GetAllLegalTarget(SurakartaPosition(i, j));
            // legalMoves->push_back(rule_manager.GetAllLegalTarget(SurakartaPosition(i, j)));
            {
                SurakartaPosition _from = {i,j};
                std::unique_ptr<std::vector<SurakartaPosition>> AllTo = rule_manager.GetAllLegalTarget(_from);    //  store all to
                std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>> tmp;
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));
            }
        }
    }
    if (maximizingPlayer) {
        int bestScore = std::numeric_limits<int>::min();
        // 遍历所有可能的移动，依次对每个走法调用 Minimax 确定最佳走法
        for (const auto& move : legalMoves) {
            // SurakartaGame game;
            // board.ExecuteMove(move);
            for (auto& move_ : *move.second)
            // SurakartaMove move_(move.first, (*move.second));
            {
                SurakartaBoard board_ = board;
                SurakartaMove temp_move(move.first, move_, rule_manager.game_info_->current_player_);
                SurakartaIllegalMoveReason move_reason = rule_manager.JudgeMove(temp_move/*, path*/);
                if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                    std::swap((board_)[temp_move.to.x][temp_move.to.y], (board_)[temp_move.from.x][temp_move.from.y]);
                    (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                    (board_)[temp_move.from.x][temp_move.from.y]->SetPosition(temp_move.from);
                } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                    (board_)[temp_move.to.x][temp_move.to.y] = (board_)[temp_move.from.x][temp_move.from.y];
                    (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                    (board_)[temp_move.from.x][temp_move.from.y] = std::make_shared<SurakartaPiece>(temp_move.from.x, temp_move.from.y, PieceColor::NONE);
                }

                // 通过Minimax递归调用评估这个走法
                int score;
                if (currentPlayer == SurakartaPlayer::BLACK)
                    score = Minimax(board_, depth - 1, false, alpha, beta, SurakartaPlayer::WHITE);
                else
                    score = Minimax(board_, depth - 1, false, alpha, beta, SurakartaPlayer::BLACK);
                // 撤销这个走法
                // board.UndoMove(temp_move);
                // 检查这个走法是否比当前已知的走法更好
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) { // beta剪枝
                    break;
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = std::numeric_limits<int>::max();
        // auto possibleMoves = board.GetLegalMoves(currentPlayer);
        for (const auto& move : legalMoves) {
            // SurakartaGame game;
            // board.ExecuteMove(move);
            for (auto& move_ : *move.second)
            // SurakartaMove move_(move.first, (*move.second));
            {
                SurakartaBoard board_ = board;
                SurakartaMove temp_move(move.first, move_, rule_manager.game_info_->current_player_);
                SurakartaIllegalMoveReason move_reason = rule_manager.JudgeMove(temp_move/*, path*/);
                if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                    std::swap((board_)[temp_move.to.x][temp_move.to.y], (board_)[temp_move.from.x][temp_move.from.y]);
                    (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                    (board_)[temp_move.from.x][temp_move.from.y]->SetPosition(temp_move.from);
                } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                    (board_)[temp_move.to.x][temp_move.to.y] = (board_)[temp_move.from.x][temp_move.from.y];
                    (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                    (board_)[temp_move.from.x][temp_move.from.y] = std::make_shared<SurakartaPiece>(temp_move.from.x, temp_move.from.y, PieceColor::NONE);
                }

                // 通过Minimax递归调用评估这个走法
                int score;
                if (currentPlayer == SurakartaPlayer::BLACK)
                    score = Minimax(board_, depth - 1, false, alpha, beta, SurakartaPlayer::WHITE);
                else
                    score = Minimax(board_, depth - 1, false, alpha, beta, SurakartaPlayer::BLACK);
                // 撤销这个走法
                // board.UndoMove(temp_move);
                // 检查这个走法是否比当前已知的走法更好
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) { // alpha剪枝
                    break;
                }
            }
        }
        return bestScore;
    }
}

// Minimax 根部分，选择评分最高的走法
SurakartaMove MinimaxRoot(SurakartaRuleManager rule_manager, int depth) {
    int bestScore = std::numeric_limits<int>::min();
    SurakartaMove bestMove;
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves;
    // std::shared_ptr<const SurakartaBoard> board_ = rule_manager.board_;
    SurakartaBoard board_ = (*rule_manager.board_);

    for(unsigned i = 0; i < rule_manager.GetBoardSize(); i++ ){
        for(unsigned j = 0; j < rule_manager.GetBoardSize(); j++){
            if((board_)[i][j]->GetColor() == rule_manager.game_info_->current_player_)
            // auto lagelMoves = rule_manager.GetAllLegalTarget(SurakartaPosition(i, j));
            // legalMoves->push_back(rule_manager.GetAllLegalTarget(SurakartaPosition(i, j)));
            {
                SurakartaPosition _from = {i,j};
                std::unique_ptr<std::vector<SurakartaPosition>> AllTo = rule_manager.GetAllLegalTarget(_from);    //  store all to
                std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>> tmp;
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));
            }
        }
    }
    // 遍历所有可能的移动，依次对每个走法调用 Minimax 确定最佳走法
    for (const auto& move : legalMoves) {
        // SurakartaGame game;
        // board.ExecuteMove(move);
        for (auto& move_ : *move.second)
        // SurakartaMove move_(move.first, (*move.second));
        {
            SurakartaMove temp_move(move.first, move_, rule_manager.game_info_->current_player_);
            SurakartaIllegalMoveReason move_reason = rule_manager.JudgeMove(temp_move/*, path*/);
            if (move_reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                std::swap((board_)[temp_move.to.x][temp_move.to.y], (board_)[temp_move.from.x][temp_move.from.y]);
                (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                (board_)[temp_move.from.x][temp_move.from.y]->SetPosition(temp_move.from);
            } else if (move_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                (board_)[temp_move.to.x][temp_move.to.y] = (board_)[temp_move.from.x][temp_move.from.y];
                (board_)[temp_move.to.x][temp_move.to.y]->SetPosition(temp_move.to);
                (board_)[temp_move.from.x][temp_move.from.y] = std::make_shared<SurakartaPiece>(temp_move.from.x, temp_move.from.y, PieceColor::NONE);
            }

            // 通过Minimax递归调用评估这个走法
            int score = Minimax(board_, depth - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), rule_manager.game_info_->current_player_);
            // 撤销这个走法
            // board.UndoMove(temp_move);
            // 检查这个走法是否比当前已知的走法更好
            if (score > bestScore) {
                bestScore = score;
                bestMove = temp_move;
            }
        }

    }
    // ... 在这里实现具体遍历移动的逻辑
    return bestMove;
}

// 你的 AI 算法调用 Minimax 的入口
SurakartaMove SurakartaAgentMine::CalculateMove() {
    int depth = 3; // 选择一个适当的搜索深度
    SurakartaMove bestMove = MinimaxRoot(*rule_manager_, depth);
    return bestMove;
}
