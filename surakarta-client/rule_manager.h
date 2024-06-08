#pragma once

#include <memory>
#include <vector>
#include "info_common.h"
#include "info_board.h"
#include "settings.h"

class SurakartaRuleManager {
public:
    SurakartaRuleManager() = default;

    SurakartaRuleManager(std::shared_ptr<SurakartaBoard> board,
                         std::shared_ptr<SurakartaGameInfo> game_info)
        :
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)) {}
    SurakartaRuleManager(std::shared_ptr<SurakartaBoard> board,
                         std::shared_ptr<SurakartaPlayer> current_player)
        :
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(new SurakartaGameInfo(current_player)) {}

    SurakartaRuleManager(const SurakartaRuleManager& other)
        : board_(std::make_shared<const SurakartaBoard>(*other.board_)),
        game_info_(std::make_shared<const SurakartaGameInfo>(*other.game_info_)) {}

    SurakartaRuleManager& operator=(const SurakartaRuleManager& other) {
        if (this != &other) {
            board_ = std::make_shared<const SurakartaBoard>(*other.board_);
            game_info_ = std::make_shared<const SurakartaGameInfo>(*other.game_info_);
        }
        return *this;
    }

    unsigned int GetBoardSize() { return BOARD_SIZE; }
    std::shared_ptr<const SurakartaBoard> GetBoard() { return board_; }
    std::shared_ptr<const SurakartaGameInfo> GetGameInfo() { return game_info_; }

    virtual void OnUpdateBoard();
    virtual SurakartaIllegalMoveReason JudgeMove(/*const*/ SurakartaMove& move);
    virtual std::pair<SurakartaEndReason, SurakartaPlayer> JudgeEnd(const SurakartaIllegalMoveReason reason);
    virtual std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalCaptureTarget(const SurakartaPosition postion);
    virtual std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalNONCaptureTarget(const SurakartaPosition postion);

protected:
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;

public:
    bool Eat(const SurakartaMove& move, std::vector<std::shared_ptr<SurakartaPosition>>& path);

    bool IsN_C_M(const SurakartaMove& move);

    bool EatCircle(const SurakartaMove& move, const circle& circle, unsigned int& i, std::vector<std::shared_ptr<SurakartaPosition>>& path);

    // int bye_world_ = 0;
};
