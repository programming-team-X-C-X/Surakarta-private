#pragma once
#include <memory>
#include "agent_base.h"

class SurakartaAgentMine : public SurakartaAgentBase {
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;

    int CalculateMarks(SurakartaBoard& _board) const ;

    SurakartaMove GetBestMove(std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>>&) const;

    std::vector<std::pair<SurakartaPosition,std::unique_ptr<std::vector<SurakartaPosition>>>> GetAllPositions() const;

    void CopyBoard(SurakartaBoard& ls) const;
};
