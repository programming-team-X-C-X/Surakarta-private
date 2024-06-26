#include "agent_mine.h"
#include "settings.h"
#include <QRandomGenerator>


// 计算当前棋局评分
int SurakartaAgentMine::EvaluateBoardFor(const SurakartaBoard& board) {
    int score = 0;
    int myPieces = 0;
    int opponentPieces = 0;
    int centerBonus = 5; // 中心位置的额外分值
    int edgePenalty = -2; // 边缘位置的分值惩罚

    //计算进度
    progress++;
    if (progress % updateInterval == 0)
        emit updateProgress(progress / totalNode);

    // 遍历棋盘，统计棋子数并计算位置分值
    for(unsigned i = 0; i < board.board_size_; i++) {
        for(unsigned j = 0; j < board.board_size_; j++) {
            PieceColor pieceColor = board[i][j]->GetColor();
            if (pieceColor == rule_manager_->GetGameInfo()->current_player_) {
                myPieces++;
                // 检查是否在中心位置
                if ((i == board.board_size_/2 - 1 || i == board.board_size_/2) &&
                    (j == board.board_size_/2 - 1 || j == board.board_size_/2)) {
                    score += centerBonus;
                } else if (i == 0 || i == board.board_size_ - 1 || j == 0 || j == board.board_size_ - 1) {
                    score += edgePenalty;
                }
            } else if (pieceColor != PieceColor::NONE) {
                opponentPieces++;
                // 检查对方棋子是否在中心位置
                if ((i == board.board_size_/2 - 1 || i == board.board_size_/2) &&
                    (j == board.board_size_/2 - 1 || j == board.board_size_/2)) {
                    score -= centerBonus;
                } else if (i == 0 || i == board.board_size_ - 1 || j == 0 || j == board.board_size_ - 1) {
                    score -= edgePenalty;
                }
            }
        }
    }
    // 计算剩余棋子数的差异
    if (opponentPieces == 0)
        score = 10000;
    else
        score += (myPieces - opponentPieces) * 50; // 每个棋子差异的分值
    return score;
}


// 检查游戏是否结束
bool IsGameOver(const SurakartaBoard& board) {
    int whitecnt = 0;
    int blackcnt = 0;
    for(unsigned i = 0; i < board.board_size_; i++ )
        for(unsigned j = 0; j < board.board_size_; j++)
            if(board[i][j]->GetColor() == SurakartaPlayer::WHITE)
                whitecnt++;
            else if ( board[i][j]->GetColor() == SurakartaPlayer::BLACK)
                // return false;
                blackcnt++;
    if (!whitecnt || !blackcnt)
        return true;
    else
        return false;
}

MoveList SurakartaAgentMine::getLegalMoves(SurakartaRuleManager rule_manager) {
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves;
    for(unsigned i = 0; i < rule_manager.GetBoardSize(); i++ ){
        for(unsigned j = 0; j < rule_manager.GetBoardSize(); j++){
            if((*rule_manager.GetBoard())[i][j]->GetColor() == rule_manager.GetGameInfo()->current_player_)
            {
                SurakartaPosition _from = {i,j};
                std::unique_ptr<std::vector<SurakartaPosition>> AllTo = rule_manager.GetAllLegalNONCaptureTarget(_from);
                std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>> tmp;
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));
                AllTo = rule_manager.GetAllLegalCaptureTarget(_from);
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));
            }
        }
    }
    return legalMoves;
}
// 重载函数，调用时有细微区别，用于顺便记录和计算平均节点数，用于估算工作
MoveList SurakartaAgentMine::getLegalMoves(SurakartaRuleManager rule_manager, int& averangeCnt) {
    int totalMoves = 0;
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves;
    for(unsigned i = 0; i < rule_manager.GetBoardSize(); i++ ){
        for(unsigned j = 0; j < rule_manager.GetBoardSize(); j++){
            if((*rule_manager.GetBoard())[i][j]->GetColor() == rule_manager.GetGameInfo()->current_player_) {
                totalPiece++;
                SurakartaPosition _from = {i,j};
                std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>> tmp;
                std::unique_ptr<std::vector<SurakartaPosition>> AllTo = rule_manager.GetAllLegalNONCaptureTarget(_from);
                if (AllTo)
                    totalMoves += (*AllTo).size();
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));

                AllTo = rule_manager.GetAllLegalCaptureTarget(_from);
                if (AllTo)
                    totalMoves += (*AllTo).size();
                tmp = std::make_pair(_from, std::move(AllTo));
                legalMoves.push_back(std::move(tmp));
            }
        }
    }
    averangeCnt = totalMoves / totalPiece;
    averangeNode = (double)totalMoves / totalPiece;
    return legalMoves;
}

// minimax 的递归部分
int SurakartaAgentMine::Minimax(SurakartaBoard& board, int depth, bool maximizingPlayer,
                                int alpha, int beta, PieceColor currentPlayer) {
    if (depth == 0 || IsGameOver(board)) {
        return EvaluateBoardFor(board) + depth;
    }

    //找到所有可行
    SurakartaRuleManager rule_manager(std::make_shared<SurakartaBoard>(board), std::make_shared<SurakartaPlayer>(currentPlayer));
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves = getLegalMoves(rule_manager);
    int bestScore = -1000;
    // 遍历所有可能的移动，依次对每个走法调用 Minimax 确定最佳走法
    for (const auto& move : legalMoves) {
        for (auto& move_ : *move.second) {
            SurakartaBoard board_ = board;
            SurakartaMove temp_move(move.first, move_, rule_manager.GetGameInfo()->current_player_);
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
            PieceColor nextPlayer = (currentPlayer == SurakartaPlayer::BLACK) ? SurakartaPlayer::WHITE : SurakartaPlayer::BLACK;
            int score = Minimax(board_, depth - 1, !maximizingPlayer, alpha, beta, nextPlayer);

            // 检查这个走法是否比当前已知的走法更好
            if (maximizingPlayer){
                bestScore = std::numeric_limits<int>::min();
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) { // beta剪枝
                    break;
                }
            }
            else {
                bestScore = std::numeric_limits<int>::max();
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) { // alpha剪枝
                    break;
                }
            }
        }
    }
    return bestScore;
}

// Minimax 根部分，选择评分最高的走法
SurakartaMove SurakartaAgentMine::MinimaxRoot(SurakartaRuleManager rule_manager, int depth) {
    int bestScore = std::numeric_limits<int>::min();
    SurakartaMove bestMove;
    int averangeNodeCnt = 0;
    totalNode = 0;
    totalPiece = 0;
    progress = 0;
    averangeNode = 0;
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> legalMoves = getLegalMoves(rule_manager, averangeNodeCnt);
    SurakartaBoard board_ = (*rule_manager.GetBoard());

    for (int i = 1, tmpNode = 1; i < depth - 1; i++) {
        tmpNode *= averangeNode * totalPiece * 1.15;
        totalNode += tmpNode;
    }
    // 遍历所有可能的移动，依次对每个走法调用 Minimax 确定最佳走法
    for (const auto& move : legalMoves) {
        for (auto& move_ : *move.second)
        {
            SurakartaBoard board_ = (*rule_manager.GetBoard());
            SurakartaMove temp_move(move.first, move_, rule_manager.GetGameInfo()->current_player_);
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
            int score = Minimax(board_, depth - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), rule_manager.GetGameInfo()->current_player_);

            // 检查这个走法是否比当前已知的走法更好
            if (score > bestScore) {
                bestScore = score;
                bestMove = temp_move;
            }else if (score == bestScore) { // 随机化
                int randomValue = QRandomGenerator::global()->bounded(2);
                if (randomValue) bestMove = temp_move;
            }
        }
    }
    return bestMove;
}

// 调用 Minimax 的入口
SurakartaMove SurakartaAgentMine::CalculateMove() {
    int depth = AI_SEARCH_DEPTH; // 选择一个适当的搜索深度
    updateInterval = depth * 10;
    SurakartaMove bestMove = MinimaxRoot(*rule_manager_, depth);
    emit updateProgress(100);
    return bestMove;
}
