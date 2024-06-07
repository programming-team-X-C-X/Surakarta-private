#pragma once

#include "agent_mine.h"
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
    SurakartaGame(unsigned board_size = BOARD_SIZE/*, unsigned int max_no_capture_round = MAX_NO_CAPTURE_ROUND*/)
        : board_size_(board_size),
        board_(std::make_shared<SurakartaBoard>(board_size)),
        game_info_(std::make_shared<SurakartaGameInfo>()),
        rule_manager_(std::make_shared<SurakartaRuleManager>(board_, game_info_)),
        agent_(std::make_shared<SurakartaAgentMine>(board_, game_info_, rule_manager_)) {}

    SurakartaGame(/*unsigned BOARD_SIZE = BOARD_SIZE,*/ SurakartaGameMode gameMode)
        : game_mode_(gameMode),
        board_size_(BOARD_SIZE),
        board_(std::make_shared<SurakartaBoard>(BOARD_SIZE)),
        game_info_(std::make_shared<SurakartaGameInfo>()),
        rule_manager_(std::make_shared<SurakartaRuleManager>(board_, game_info_)),
        agent_(std::make_shared<SurakartaAgentMine>(board_, game_info_, rule_manager_)) {}

    // 拷贝构造函数
    SurakartaGame(SurakartaGame& other)
        : game_mode_(other.game_mode_),
        white_player_(other.white_player_),
        black_player_(other.black_player_),
        board_size_(other.board_size_),
        board_(std::make_shared<SurakartaBoard>(*other.board_)), // 假设SurakartaBoard有拷贝构造函数
        game_info_(std::make_shared<SurakartaGameInfo>(*other.game_info_)), // 假设SurakartaGameInfo有拷贝构造函数
        rule_manager_(std::make_shared<SurakartaRuleManager>(*other.rule_manager_)), // 假设SurakartaRuleManager有拷贝构造函数
        agent_(std::make_shared<SurakartaAgentMine>(*other.agent_)) // 假设SurakartaAgentBase有拷贝构造函数
    {}

    // 拷贝赋值运算符
    SurakartaGame& operator=(const SurakartaGame& other) {
        if (this != &other) { // 防止自赋值
            game_mode_ = other.game_mode_;
            white_player_ = other.white_player_;
            black_player_ = other.black_player_;
            board_size_ = other.board_size_;
            board_ = std::make_shared<SurakartaBoard>(*other.board_);
            game_info_ = std::make_shared<SurakartaGameInfo>(*other.game_info_);
            rule_manager_ = std::make_shared<SurakartaRuleManager>(*other.rule_manager_);
            agent_ = std::make_shared<SurakartaAgentMine>(*other.agent_);
        }
        return *this;
    }

    void StartGame(/*std::string file_name = ""*/);
    void UpdateGameInfo(SurakartaIllegalMoveReason move_reason, SurakartaEndReason end_reason, SurakartaPlayer winner);
    SurakartaMoveResponse Move(/*const*/ SurakartaMove& move);
    void SetAgent(std::shared_ptr<SurakartaAgentMine> agent) { agent_ = agent; }

    unsigned int GetBoardSize() const { return board_size_; }
    std::shared_ptr<SurakartaBoard> GetBoard() const { return board_; }
    std::shared_ptr<SurakartaGameInfo> GetGameInfo() const { return game_info_; }
    std::shared_ptr<SurakartaAgentMine> GetAgent() const { return agent_; }
    bool IsEnd() const { return game_info_->IsEnd(); }
    SurakartaGameMode GetGameMode() const { return game_mode_; }
    SurakartaRealPlayer GetWhitePlayer() const { return white_player_; }
    SurakartaRealPlayer GetBlackPlayer() const { return black_player_; }
    std::shared_ptr<SurakartaRuleManager> GetRuleManager() const { return rule_manager_; }  // For testing.

    void SetRuleManager(std::shared_ptr<SurakartaRuleManager> rule_manager) {  // For testing.
        rule_manager_ = rule_manager;
    }
    void SetGameMode(SurakartaGameMode game_mode) { game_mode_ = game_mode; }
    void SetWhitePlayer(SurakartaRealPlayer white_player) { white_player_ = white_player; }
    void SetBlackPlayer(SurakartaRealPlayer black_player) { black_player_ = black_player; }

private:
    SurakartaGameMode game_mode_;
    SurakartaRealPlayer white_player_;
    SurakartaRealPlayer black_player_;
    unsigned int board_size_;
    std::shared_ptr<SurakartaBoard> board_;
    std::shared_ptr<SurakartaGameInfo> game_info_;
    std::shared_ptr<SurakartaRuleManager> rule_manager_;
    std::shared_ptr<SurakartaAgentMine> agent_;
};


