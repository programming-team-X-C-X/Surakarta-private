#pragma once

#include <memory>
#include <vector>
#include "my_common.h"
#include "my_board.h"
#include "settings.h"

class SurakartaRuleManager {
public:
    SurakartaRuleManager() = default;

    SurakartaRuleManager(std::shared_ptr<SurakartaBoard> board,
                         std::shared_ptr<SurakartaGameInfo> game_info)
        : /*BOARD_SIZE(board->board_size_),*/
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)) {}

    unsigned int GetBoardSize() {
        return BOARD_SIZE;
    }

    virtual void OnUpdateBoard();


    /**
     * @brief Judge whether a move is legal.
     * @param move The move to be judged.
     */
    virtual SurakartaIllegalMoveReason JudgeMove(/*const*/ SurakartaMove& move);

    /**
     * @brief Judge whether the game is end.
     * @param reason SurakartaIllegalMoveReason of the last move.
     */
    virtual std::pair<SurakartaEndReason, SurakartaPlayer> JudgeEnd(const SurakartaIllegalMoveReason reason);

    // *
    //  * @brief Get all legal target positions of a piece.
    //  * @param postion The position of the piece.

    virtual std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalTarget(const SurakartaPosition postion);

    //    protected:
    // const unsigned int BOARD_SIZE;
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;

public:
    // TODO:
    //  define your own functions/variables here
    bool Eat(const SurakartaMove& move, std::vector<std::shared_ptr<SurakartaPosition>>& path);

    bool IsN_C_M(const SurakartaMove& move);

    bool EatCircle(const SurakartaMove& move, const circle& circle, unsigned int& i, std::vector<std::shared_ptr<SurakartaPosition>>& path);

    int bye_world_ = 0;
};
