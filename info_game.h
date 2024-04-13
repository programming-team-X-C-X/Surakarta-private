#pragma once

#include "agent_base.h"
#include "rule_manager.h"
#include "settings.h"
#include "info_piece.h"
#include "info_reason.h"

enum class SurakartaGameMode : int { EVE,
                                     PVE,
                                     PVP,
                                     UNKNOWN
};

enum class SurakartaRealPlayer : int { COMPUTER,
                                       PLAYER,
                                       UNKNOWN
};

class SurakartaMoveResponse {
public:
    SurakartaMoveResponse(SurakartaIllegalMoveReason move_reason)
        : move_reason_(move_reason), end_reason_(SurakartaEndReason::NONE), winner_(SurakartaPlayer::NONE) {}

    SurakartaMoveResponse(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner)
        : move_reason_(move_reason), end_reason_(end_reason), winner_(winner) {}

    bool IsLegal() const {
        return IsLegalMoveReason(move_reason_);
    }

    bool IsEnd() const {
        return end_reason_ != SurakartaEndReason::NONE;
    }

    SurakartaIllegalMoveReason GetMoveReason() const {
        return move_reason_;
    }

    SurakartaEndReason GetEndReason() const {
        return end_reason_;
    }

    SurakartaPlayer GetWinner() const {
        return winner_;
    }

    //    private:
    SurakartaIllegalMoveReason move_reason_;
    SurakartaEndReason end_reason_;
    SurakartaPlayer winner_;
};

class SurakartaGame {
public:
    SurakartaGame(/*unsigned BOARD_SIZE = BOARD_SIZE,*/ SurakartaGameMode gameMode)
        : game_mode_(gameMode),
        board_size_(BOARD_SIZE),
        board_(std::make_shared<SurakartaBoard>(BOARD_SIZE)),
        game_info_(std::make_shared<SurakartaGameInfo>()),
        rule_manager_(std::make_shared<SurakartaRuleManager>(board_, game_info_)),
        agent_(std::make_shared<SurakartaAgentBase>(board_, game_info_, rule_manager_)) {}

    void StartGame(/*std::string file_name = ""*/);
    // void SaveGame(std::string file_name) const;
    void UpdateGameInfo(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner);
    SurakartaMoveResponse Move(/*const*/ SurakartaMove& move);
    void SetAgent(std::shared_ptr<SurakartaAgentBase> agent) { agent_ = agent; }

    unsigned int GetBoardSize() const { return board_size_; }
    std::shared_ptr<SurakartaBoard> GetBoard() const { return board_; }
    std::shared_ptr<SurakartaGameInfo> GetGameInfo() const { return game_info_; }
    std::shared_ptr<SurakartaAgentBase> GetAgent() const { return agent_; }
    bool IsEnd() const { return game_info_->IsEnd(); }

    void SetRuleManager(std::shared_ptr<SurakartaRuleManager> rule_manager) {  // For testing.
        rule_manager_ = rule_manager;
    }
    std::shared_ptr<SurakartaRuleManager> GetRuleManager() const { return rule_manager_; }  // For testing.

    //    private:
    SurakartaGameMode game_mode_;
    SurakartaRealPlayer white_player_;
    SurakartaRealPlayer black_player_;
    unsigned int board_size_;
    std::shared_ptr<SurakartaBoard> board_;
    std::shared_ptr<SurakartaGameInfo> game_info_;
    std::shared_ptr<SurakartaRuleManager> rule_manager_;
    std::shared_ptr<SurakartaAgentBase> agent_;
    // std::shared_ptr<ChessBoardWidget> chessboardwight_;

};


