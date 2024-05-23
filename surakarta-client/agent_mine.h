#pragma once
#include "agent_base.h"

class SurakartaAgentMine : public SurakartaAgentBase {
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;

};
