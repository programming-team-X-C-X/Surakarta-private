#pragma once
#include "agent_base.h"

class SurakartaAgentRandom : public SurakartaAgentBase {
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;
};
