#pragma once
#include <memory>
#include "info_common.h"
#include "rule_manager.h"
#include <QObject>

using MoveList = std::vector<std::pair<SurakartaPosition, std::unique_ptr<std::vector<SurakartaPosition>>>>;

class SurakartaAgentMine : public QObject {
    Q_OBJECT
public:


    SurakartaAgentMine(std::shared_ptr<SurakartaBoard> board,
                       std::shared_ptr<SurakartaGameInfo> game_info,
                       std::shared_ptr<SurakartaRuleManager> rule_manager)
        : board_size_(board->board_size_),
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)),
        rule_manager_(rule_manager) {}

    SurakartaAgentMine(const SurakartaAgentMine& other)
        : QObject(other.parent()), /* // 如果QObject的父子关系需要保持，传递parent()*/
        board_size_(other.board_size_),
        board_(std::make_shared<const SurakartaBoard>(*other.board_)),
        game_info_(std::make_shared<const SurakartaGameInfo>(*other.game_info_)),
        rule_manager_(std::make_shared<SurakartaRuleManager>(*other.rule_manager_)),
        progress(other.progress),
        averangeNode(other.averangeNode),
        totalNode(other.totalNode),
        totalPiece(other.totalPiece),
        updateInterval(other.updateInterval) {
        // 连接信号和槽，如果需要
    }

    void SetRuleManager(std::shared_ptr<SurakartaRuleManager> rule_manager) {  // For testing
        rule_manager_ = rule_manager;
    }
    SurakartaMove CalculateMove();
    MoveList getLegalMoves(SurakartaRuleManager rule_manager, int& averangeCnt);
    MoveList getLegalMoves(SurakartaRuleManager rule_manager);
    SurakartaMove MinimaxRoot(SurakartaRuleManager rule_manager, int depth);
    int EvaluateBoardFor(const SurakartaBoard& board /*PieceColor currentPlayer*/);
    int Minimax(SurakartaBoard& board, int depth, bool maximizingPlayer,
                int alpha, int beta, PieceColor currentPlayer);

// private:
    unsigned int board_size_;
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;
    std::shared_ptr<SurakartaRuleManager> rule_manager_;
    int progress;
    double averangeNode;
    int totalNode;
    int totalPiece;
    int updateInterval; // 更新间隔

signals:
    void updateProgress(int progress);
};
