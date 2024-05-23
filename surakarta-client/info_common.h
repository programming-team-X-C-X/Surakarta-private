#pragma once

#include "info_piece.h"
#include "info_reason.h"
#include "settings.h"

struct SurakartaMove {
    SurakartaPosition from;
    SurakartaPosition to;
    SurakartaPlayer player;
    bool isLoopMove; // 表示是否是旋吃动作
    std::vector<std::shared_ptr<SurakartaPosition>> pathPoints; // 存储旋吃路径上的关键点

    SurakartaMove()
        : from(0, 0), to(0, 0), player(SurakartaPlayer::NONE), isLoopMove(false) {}
    SurakartaMove(unsigned int from_x, unsigned int from_y, unsigned int to_x, unsigned int to_y, SurakartaPlayer player)
        : from(from_x, from_y), to(to_x, to_y), player(player), isLoopMove(false)  {}
    SurakartaMove(SurakartaPosition from, SurakartaPosition to, SurakartaPlayer player)
        : from(from), to(to), player(player), isLoopMove(false) {}
};

struct SurakartaGameInfo {
    SurakartaPlayer current_player_;
    SurakartaPlayer player_color_;
    SurakartaPlayer computer_color_;
    unsigned int num_round_;
    unsigned int last_captured_round_;
    SurakartaEndReason end_reason_;
    SurakartaPlayer winner_;
    unsigned int max_no_capture_round_;

    SurakartaGameInfo(/*unsigned int max_no_capture_round = 40*/)
        : current_player_(SurakartaPlayer::BLACK),
        player_color_(PLAYER_COLOR ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE),
        computer_color_(PLAYER_COLOR ? SurakartaPlayer::WHITE : SurakartaPlayer::BLACK),
        num_round_(1),
        last_captured_round_(0),
        end_reason_(SurakartaEndReason::NONE),
        winner_(SurakartaPlayer::NONE),
        max_no_capture_round_(MAX_NO_CAPTURE_ROUND) {}

    SurakartaGameInfo(std::shared_ptr<SurakartaPlayer> current_player)
        : current_player_(*current_player),
        player_color_(PLAYER_COLOR ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE),
        computer_color_(PLAYER_COLOR ? SurakartaPlayer::WHITE : SurakartaPlayer::BLACK),
        num_round_(1),
        last_captured_round_(0),
        end_reason_(SurakartaEndReason::NONE),
        winner_(SurakartaPlayer::NONE),
        max_no_capture_round_(MAX_NO_CAPTURE_ROUND) {}

    void Reset() {
        current_player_ = SurakartaPlayer::BLACK;
        player_color_ = PLAYER_COLOR ? SurakartaPlayer::BLACK : SurakartaPlayer::WHITE;
        computer_color_ = PLAYER_COLOR ? SurakartaPlayer::WHITE : SurakartaPlayer::BLACK;
        num_round_ = 1;
        last_captured_round_ = 0;
        end_reason_ = SurakartaEndReason::NONE;
        winner_ = SurakartaPlayer::NONE;
    }

    bool IsEnd() const { return IsEndReason(end_reason_); }
    SurakartaPlayer Winner() const { return winner_; }
};

class SurakartaException : public std::exception {
public:
    SurakartaException(const std::string& message)
        : message_(message) {}
    virtual const char* what() const throw() { return message_.c_str(); }

private:
    std::string message_;
};
