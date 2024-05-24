#pragma once
#include "agent_base.h"
#include <QObject>

class SurakartaAgentMine : public SurakartaAgentBase {
    Q_OBJECT
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;

    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> getLegalMoves(SurakartaRuleManager rule_manager, int& averangeCnt);
    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> getLegalMoves(SurakartaRuleManager rule_manager);
    SurakartaMove MinimaxRoot(SurakartaRuleManager rule_manager, int depth);

    int EvaluateBoardFor(const SurakartaBoard& board, PieceColor currentPlayer);
    int Minimax(SurakartaBoard& board, int depth, bool maximizingPlayer,
                int alpha, int beta, PieceColor currentPlayer);


private:
    int progress;
    double averangeNode;
    int totalNode;
    int totalPiece;
    int updateInterval; // 更新间隔

signals:
    void updateProgress(int progress);
};
