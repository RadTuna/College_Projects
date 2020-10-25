#pragma once

#include "Transition.h"
#include "MinerAttribute.h"

class FromMineToBank final : public Transition<MinerAttribute>
{
public:
    FromMineToBank() = default;
    ~FromMineToBank() override = default;

    bool IsTransition() override
    {
        return mAttribute->OreNum == mAttribute->MaxOreNum;
    }
};
