#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromBankToMine final : public Transition<MinerAttribute>
{
public:
    FromBankToMine() = default;
    ~FromBankToMine() override = default;

    bool IsTransition() override
    {
        return mAttribute->Gold < mAttribute->MaxGold;
    }
};
