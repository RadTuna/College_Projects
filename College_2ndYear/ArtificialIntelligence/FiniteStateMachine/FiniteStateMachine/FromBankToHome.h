#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromBankToHome final : public Transition<MinerAttribute>
{
public:
    FromBankToHome() = default;
    ~FromBankToHome() override = default;

    bool IsTransition() override
    {
        return mAttribute->Gold == mAttribute->MaxGold;
    }
};
